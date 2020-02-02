#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QSerialPort>
#include "consolemode/consolewidget.h"
#include "hexmode/tableconsole.h"
#include "serialgui.h"
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
    void applyTopPanelStyleSheet (void);
    void applyConsoleStyleSheet(void);
    void applyTableStyleSheet(void);
    void applyMenuStyleSheet(void);
private:
    Ui::MainWidget    *ui;
    SettingsDialog    *settingsWindow;
    SerialGui         *serial;
    ConsoleWidget     *console;
    TableConsole      *tableConsole;
    const int         indexConsole   = 0;
    const int         indexTable     = 1;
    const int         indexConverter = 2;

};
#endif // MAINWIDGET_H



