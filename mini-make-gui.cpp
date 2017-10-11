/* Copyright (C) 2017 Taylor Holberton
 *
 * This file is part of Mini Make.
 *
 * Mini Make is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mini Make is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Mini Make.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mini-make-gui.hpp"

#include <mini-make/error.h>
#include <mini-make/ihooks.h>
#include <mini-make/interpreter.h>

#include <iostream>

namespace {

int onTarget(void *controllerPtr, const make_target *target) {

  auto controller = (mini_make::Controller *) controllerPtr;

  controller->addTarget(target->path.data);

  return make_success;
}

} // namespace

int main(int argc, char **argv) {

  QApplication app(argc, argv);

  mini_make::Controller controller;

  return app.exec();
}

namespace mini_make {

TerminalWidget::TerminalWidget(QWidget *parent) : QTextEdit(parent) {
  setReadOnly(true);
  setText("Makefile output goes here");
}
TerminalWidget::~TerminalWidget() {

}

VariablesWidget::VariablesWidget(QWidget *parent) : QTableWidget(parent) {
  setColumnCount(2);
  QStringList headers;
  headers << "Name";
  headers << "Value";
  setHorizontalHeaderLabels(headers);
  horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
VariablesWidget::~VariablesWidget() {

}

TargetsWidget::TargetsWidget(QWidget *parent) : QTableWidget(parent) {
  setColumnCount(4);
  QStringList headers;
  headers << "Name";
  headers << "Expired";
  headers << "Phony";
  headers << "Exists";
  setHorizontalHeaderLabels(headers);
  horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  targetCount = 0;
}
TargetsWidget::~TargetsWidget() {

}
bool TargetsWidget::targetExists(const QString &targetName) {
  for (int i = 0; i < rowCount(); i++) {
    QTableWidgetItem *itemPtr = item(i, 0);
    if (itemPtr == nullptr)
      continue;
    else if (itemPtr->text() == targetName)
      return true;
  }
  return false;
}
void TargetsWidget::addTarget(const QString &targetName) {
  if (!targetExists(targetName)) {
    setRowCount(targetCount + 1);
    QTableWidgetItem *tableWidgetItem = new QTableWidgetItem();
    tableWidgetItem->setText(targetName);
    setItem(targetCount, 0, tableWidgetItem);
    targetCount++;
  }
}

CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent) {
  createTargetsWidget();
  createVariablesWidget();
  createTerminalWidget();
  createButtons();
  createLayout();
}
CentralWidget::~CentralWidget() {

}
void CentralWidget::addTarget(const QString &targetName) {
  targetsWidget->addTarget(targetName);
}
void CentralWidget::onBuildButtonClicked() {
  emit buildRequested();
}
void CentralWidget::createTargetsWidget() {
  targetsWidget = new TargetsWidget(this);
}
void CentralWidget::createVariablesWidget() {
  variablesWidget = new VariablesWidget(this);
}
void CentralWidget::createTerminalWidget() {
  terminalWidget = new TerminalWidget();
}
void CentralWidget::createButtons() {

  buildButton = new QPushButton(this);
  buildButton->setText(QObject::tr("Build"));
  connect(buildButton, &QPushButton::clicked,
          this, &CentralWidget::onBuildButtonClicked);

  cleanButton = new QPushButton(this);
  cleanButton->setText(QObject::tr("Clean"));

  testButton = new QPushButton(this);
  testButton->setText(QObject::tr("Test"));

  installButton = new QPushButton(this);
  installButton->setText(QObject::tr("Install"));
}
void CentralWidget::createLayout() {
  layout = new QGridLayout(this);
  // row, column, row span, column span
  layout->addWidget(variablesWidget, 0, 0, 1, 4);
  layout->addWidget(targetsWidget,   1, 0, 1, 4);
  layout->addWidget(terminalWidget,  0, 4, 2, 4);
  layout->addWidget(buildButton,     2, 0);
  layout->addWidget(cleanButton,     2, 1);
  layout->addWidget(testButton,      2, 2);
  layout->addWidget(installButton,   2, 3);
  setLayout(layout);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Mini Make");
  createCentralWidget();
}
MainWindow::~MainWindow() {

}
void MainWindow::addTarget(const QString &targetName) {
  centralWidget->addTarget(targetName);
}
void MainWindow::onBuildRequested() {
  emit buildRequested();
}
void MainWindow::createCentralWidget() {
  centralWidget = new CentralWidget(this);
  connect(centralWidget, &CentralWidget::buildRequested,
          this, &MainWindow::onBuildRequested);
  setCentralWidget(centralWidget);
}

Controller::Controller() {
  mainWindow = new MainWindow();
  mainWindow->showMaximized();
  connect(mainWindow, &MainWindow::buildRequested,
          this, &Controller::onBuildRequested);
}
Controller::~Controller() {
  if (mainWindow != nullptr) {
    delete mainWindow;
    mainWindow = nullptr;
  }
}
void Controller::addTarget(const QString &targetName) {
  mainWindow->addTarget(targetName);
}
void Controller::onBuildRequested() {

  make_ihooks hooks;
  make_ihooks_init(&hooks);
  hooks.data = this;
  hooks.on_target = onTarget;

  make_interpreter interpreter;

  make_interpreter_init(&interpreter);

  make_string make_key;
  make_key.data = "MAKE";
  make_key.size = 4;

  make_string make_value;
  make_value.data = "\"mini-make\"";
  make_value.size = sizeof("\"mini-make\"") - 1;

  make_interpreter_define(&interpreter, &make_key, &make_value);

  make_interpreter_read(&interpreter, "Makefile");

  make_interpreter_set_hooks(&interpreter, &hooks);

  make_interpreter_run(&interpreter);

  make_interpreter_free(&interpreter);
}

} // namespace mini_make
