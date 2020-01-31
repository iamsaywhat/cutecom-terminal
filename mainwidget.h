#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QSerialPort>
#include "consolemode/consolewidget.h"
#include "hexmode/tableconsole.h"
#include "SerialForGUI.h"
#include "settingsdialog.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    Ui::MainWidget *ui;
    SettingsDialog       *settingsWindow;
    SerialForGUI         *serial;
    ConsoleWidget        *console;
    TableConsole         *tableConsole;

};
#endif // MAINWIDGET_H



