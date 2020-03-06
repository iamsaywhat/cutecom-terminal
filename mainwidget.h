#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "framelesswindow.h"
#include <QSerialPort>
#include "consolemode/consolewidget.h"
#include "hexmode/tableconsole.h"
#include "serialgui.h"
#include "decorator.h"
#include "converter.h"
#include "settingscontroller.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(FramelessWindow *parent = nullptr);
    ~MainWidget();

    void setPropertiesToMainWidget(void);
    void setPropertiesToConsole(void);
    void setPropertiesToTable(void);
    void setPropertiesToConverter(void);

    void applyColorScheme(int indexOfTheme);
    void applyColorSchemeToMainWidget(Decorator *scheme);
    void applyColorSchemetoConsole(Decorator *scheme);
    void applyColorSchemeToTable(Decorator *scheme);
    void applyColorSchemeToConverter(Decorator *scheme);
    void applyColorSchemeToSettings(Decorator *scheme);

    void setAppFont();

    void setAppTextCodec(int index);

    void fillLanguageList();
    void fillCodecList();
    void fillThemeList();

private:
    Ui::MainWidget     *ui;
    SerialGui          *serial;
    ConsoleWidget      *console;
    TableConsole       *tableConsole;
    Converter          *converter;
    SettingsController *settings;
    QStringList        *codecList;
    QStringList        *languageList;
    QList<Decorator*>  *themeList;

    Decorator          *darkTheme;
    Decorator          *fakeTheme;

    enum quickMenu {
        quickIndexConsole   = 0,
        quickIndexTable     = 1,
        quickIndexConverter = 2,
        quickIndexSettings  = 3,
    };

    QFont *appFont = new QFont("Terminus", 10, QFont::ExtraBold);
};
#endif // MAINWIDGET_H



