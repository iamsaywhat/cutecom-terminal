#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QSerialPort>
#include "consolemode/consolewidget.h"
#include "hexmode/tableconsole.h"
#include "serialgui.h"
#include "decorator.h"
#include "converter.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    void setPropertiesToMainWidget(void);
    void setPropertiesToConsole(void);
    void setPropertiesToTable(void);
    void setPropertiesToConverter(void);
    void setPropertiesToSettingsWidget(void);

    void applyColorScheme(Decorator *scheme);
    void applyColorSchemeToMainWidget(Decorator *scheme);
    void applyColorSchemetoConsole(Decorator *scheme);
    void applyColorSchemeToTable(Decorator *scheme);
    void applyColorSchemeToConverter(Decorator *scheme);
    void applyColorSchemeToSettings(Decorator *scheme);

private:
    Ui::MainWidget    *ui;
    SerialGui         *serial;
    ConsoleWidget     *console;
    TableConsole      *tableConsole;
    Converter         *converter;
    Decorator         *darkTheme;

    enum quickMenu {
        quickIndexConsole   = 0,
        quickIndexTable     = 1,
        quickIndexConverter = 2,
        quickIndexSettings  = 3,
    };
    enum settingsMenu {
        settingsIndexConnection = 0,
        settingsIndexGeneral    = 1,
        settingsIndexConsole    = 2,
        settingsIndexTable      = 3,
        settingsIndexLogs       = 4,
        settingsIndexBinds      = 5,
    };
};
#endif // MAINWIDGET_H



