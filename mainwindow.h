#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include "consolemode/ConsoleWindow.h"
#include "hexmode/tableconsole.h"
#include "SerialForGUI.h"
#include "settingsdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


private:
    Ui::MainWindow       *ui;
    SettingsDialog       *settingsWindow;
    SerialForGUI         *serial;
    ConsoleWindow        *console;
    TableConsole         *tableConsole;
};
#endif // MAINWINDOW_H
