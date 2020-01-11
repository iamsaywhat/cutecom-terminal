#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include "consolemode/ConsoleWindow.h"
#include "hexmode/tableconsole.h"
#include "serialsettings.h"
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

    void on_PortSettings_triggered();

private:
    Ui::MainWindow       *ui;
    SettingsDialog       *settingsWindow;
    SerialSettings       *serialsettings;
    ConsoleWindow        *console;
    TableConsole         *tableConsole;
};
#endif // MAINWINDOW_H
