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

#ifndef MINI_MAKE_GUI_HPP
#define MINI_MAKE_GUI_HPP

#include <QApplication>
#include <QHeaderView>
#include <QGridLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>

namespace mini_make {

class TerminalWidget final : public QTextEdit {
  Q_OBJECT
public:
  TerminalWidget(QWidget *parent = nullptr);
  ~TerminalWidget();
  void addCommand(const QString &command);
};

class VariablesWidget final : public QTableWidget {
  Q_OBJECT
public:
  VariablesWidget(QWidget *parent = nullptr);
  ~VariablesWidget();
};

class TargetsWidget final : public QTableWidget {
  Q_OBJECT
public:
  TargetsWidget(QWidget *parent = nullptr);
  ~TargetsWidget();
  void addTarget(const QString &targetName);
  bool targetExists(const QString &targetName);
private:
  unsigned int targetCount;
};

class CentralWidget final : public QWidget {
  Q_OBJECT
public:
  CentralWidget(QWidget *parent = nullptr);
  ~CentralWidget();
  void addCommand(const QString &command);
  void addTarget(const QString &targetName);
signals:
  void buildRequested();
protected:
  void createTargetsWidget();
  void createVariablesWidget();
  void createTerminalWidget();
  void createButtons();
  void createLayout();
protected slots:
  void onBuildButtonClicked();
private:
  QPushButton *buildButton;
  QPushButton *cleanButton;
  QPushButton *testButton;
  QPushButton *installButton;
  QGridLayout *layout;
  TargetsWidget *targetsWidget;
  VariablesWidget *variablesWidget;
  TerminalWidget *terminalWidget;
};

class MainWindow final : public QMainWindow {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void addCommand(const QString &command);
  void addTarget(const QString &targetName);
signals:
  void buildRequested();
protected:
  void createCentralWidget();
protected slots:
  void onBuildRequested();
private:
  CentralWidget *centralWidget;
};

class Controller final : public QObject {
  Q_OBJECT
public:
  Controller();
  ~Controller();
  void addCommand(const QString &command);
  void addTarget(const QString &targetName);
public slots:
  void onBuildRequested();
private:
  MainWindow *mainWindow;
};

} // namespace mini_make

#endif // MINI_MAKE_GUI_HPP
