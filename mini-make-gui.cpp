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

#include <mini-make/command.h>
#include <mini-make/error.h>
#include <mini-make/ihooks.h>
#include <mini-make/interpreter.h>

#include <QClipboard>
#include <QKeySequence>

#include <iostream>

namespace {

int onTarget(void *controllerPtr,
             const make_target *target) {

  auto controller = (mini_make::Controller *) controllerPtr;

  controller->addTarget(target->path.data);

  return make_success;
}

int onCommand(void *controllerPtr,
              const make_target *target,
              const make_command *command) {

  (void) target;

  std::cout << "COMMAND: " << command->source->data << std::endl;

  auto controller = (mini_make::Controller *) controllerPtr;

  controller->addCommand(command->source->data);

  return make_success;
}

} // namespace

int main(int argc, char **argv) {

  QApplication app(argc, argv);

  mini_make::Controller controller;

  return app.exec();
}

namespace mini_make {

AboutWidget::AboutWidget(QWidget *parent) : QWidget(parent) {
  setWindowTitle("Mini Make - About");
  createTextEdit();
  createButtons();
  createLayout();
}
AboutWidget::~AboutWidget() {

}
void AboutWidget::createButtons() {
  okayButton = new QPushButton(this);
  okayButton->setText(tr("Okay"));
  connect(okayButton, &QPushButton::clicked,
          this, &AboutWidget::onOkayClicked);

  copyButton = new QPushButton(this);
  copyButton->setText(tr("Copy"));
  connect(copyButton, &QPushButton::clicked,
          this, &AboutWidget::onCopyClicked);
}
void AboutWidget::createTextEdit() {

  QString html;

  html += "Mini Make GUI<br>";

  html += "Version: ";
#ifdef MINI_MAKE_VERSION
  html += MINI_MAKE_VERSION;
#else
  html += "Unknown";
#endif
  html += "<br>";

  html += "Architecture: ";
#ifdef MINI_MAKE_ARCHITECTURE
  html += MINI_MAKE_ARCHITECTURE;
#else
  html += "Unknown";
#endif
  html += "<br>";

  html += "Build System: ";
#ifdef MINI_MAKE_BUILD_SYSTEM
  html += MINI_MAKE_BUILD_SYSTEM;
#else
  html += "Unknown";
#endif
  html += "<br>";

  html += "C Compiler: ";
#ifdef MINI_MAKE_C_COMPILER
  html += MINI_MAKE_C_COMPILER;
#else
  html += "Unknown";
#endif
  html += "<br>";

  html += "C++ Compiler: ";
#ifdef MINI_MAKE_CXX_COMPILER
  html += MINI_MAKE_CXX_COMPILER;
#else
  html += "Unknown";
#endif
  html += "<br>";

  textEdit = new QTextEdit(this);
  textEdit->setReadOnly(true);
  textEdit->setHtml(html);
}
void AboutWidget::createLayout() {
  layout = new QGridLayout(this);
  layout->addWidget(textEdit,   0, 0, 1, 2);
  layout->addWidget(okayButton, 1, 0, 1, 1);
  layout->addWidget(copyButton, 1, 1, 1, 1);
  setLayout(layout);
}
void AboutWidget::onOkayClicked() {
  close();
}
void AboutWidget::onCopyClicked() {
  QClipboard *clipboardPtr = QApplication::clipboard();
  clipboardPtr->setText(textEdit->toPlainText());
}

MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent) {
  createAboutWidget();
  createFileMenu();
  createHelpMenu();
}
MenuBar::~MenuBar() {

}
void MenuBar::onExitClicked() {
  emit exitRequested();
}
void MenuBar::onAboutClicked() {
  aboutWidget->show();
}
void MenuBar::createAboutWidget() {
  /* don't use a parent widget */
  aboutWidget = new AboutWidget();
}
void MenuBar::createFileMenu() {

  fileMenu = new QMenu(this);
  fileMenu->setTitle("File");
  fileMenu->addAction("Exit", this, &MenuBar::onExitClicked,
                      QKeySequence(Qt::CTRL + Qt::Key_Q));

  addMenu(fileMenu);
}
void MenuBar::createHelpMenu() {

  helpMenu = new QMenu(this);
  helpMenu->setTitle("Help");
  helpMenu->addAction("About", this, &MenuBar::onAboutClicked);

  addMenu(helpMenu);
}

TerminalWidget::TerminalWidget(QWidget *parent) : QTextEdit(parent) {
  setReadOnly(true);
  setLineWrapMode(QTextEdit::NoWrap);
  setPlaceholderText("Makefile output goes here.");
}
TerminalWidget::~TerminalWidget() {

}
void TerminalWidget::addCommand(const QString &command) {
  setPlainText(toPlainText() + command + "\n");
}

VariablesWidget::VariablesWidget(QWidget *parent) : QTableWidget(parent) {
  setColumnCount(2);
  QStringList headers;
  headers << "Name";
  headers << "Value";
  setHorizontalHeaderLabels(headers);
  horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  variableCount = 0;
}
VariablesWidget::~VariablesWidget() {

}
void VariablesWidget::addVariable() {
  setRowCount(variableCount + 1);
  variableCount++;
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
    tableWidgetItem->setFlags(tableWidgetItem->flags() ^ Qt::ItemIsEditable);
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
void CentralWidget::addCommand(const QString &command) {
  terminalWidget->addCommand(command);
}
void CentralWidget::onBuildButtonClicked() {
  emit buildRequested();
}
void CentralWidget::onAddVariableButtonClicked() {
  variablesWidget->addVariable();
}
void CentralWidget::onDelVariableButtonClicked() {

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

  addVariableButton = new QPushButton(this);
  addVariableButton->setText(QObject::tr("Add Variable"));
  connect(addVariableButton, &QPushButton::clicked,
          this, &CentralWidget::onAddVariableButtonClicked);

  delVariableButton = new QPushButton(this);
  delVariableButton->setText(QObject::tr("Delete Variable"));
}
void CentralWidget::createLayout() {
  layout = new QGridLayout(this);
  // row, column, row span, column span
  layout->addWidget(variablesWidget,   0, 0, 1, 6);
  layout->addWidget(targetsWidget,     2, 0, 1, 6);
  layout->addWidget(terminalWidget,    0, 6, 4, 4);
  layout->addWidget(addVariableButton, 1, 2);
  layout->addWidget(delVariableButton, 1, 3);
  layout->addWidget(buildButton,       3, 1);
  layout->addWidget(cleanButton,       3, 2);
  layout->addWidget(testButton,        3, 3);
  layout->addWidget(installButton,     3, 4);
  setLayout(layout);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Mini Make");
  createCentralWidget();
  createMenuBar();
}
MainWindow::~MainWindow() {

}
void MainWindow::addTarget(const QString &targetName) {
  centralWidget->addTarget(targetName);
}
void MainWindow::addCommand(const QString &command) {
  centralWidget->addCommand(command);
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
void MainWindow::createMenuBar() {
  menuBar = new MenuBar(this);
  connect(menuBar, &MenuBar::exitRequested,
          this, &MainWindow::onExitRequested);
  setMenuBar(menuBar);
}
void MainWindow::onExitRequested() {
  close();
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
void Controller::addCommand(const QString &command) {
  mainWindow->addCommand(command);
}
void Controller::onBuildRequested() {

  make_ihooks hooks;
  make_ihooks_init(&hooks);
  hooks.data = this;
  hooks.on_target = onTarget;
  hooks.on_command = onCommand;

  make_interpreter interpreter;

  make_interpreter_init(&interpreter);

  char make_key_data[] = "MAKE";

  make_string make_key;
  make_key.data = make_key_data;
  make_key.size = sizeof(make_key_data) - 1;

  char make_value_data[] = "\"mini-make\"";

  make_string make_value;
  make_value.data = make_value_data;
  make_value.size = sizeof(make_value_data) - 1;

  make_interpreter_define(&interpreter, &make_key, &make_value);

  make_interpreter_read(&interpreter, "Makefile");

  make_interpreter_set_hooks(&interpreter, &hooks);

  make_interpreter_run(&interpreter);

  make_interpreter_free(&interpreter);
}

} // namespace mini_make
