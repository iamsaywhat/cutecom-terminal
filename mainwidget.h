#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QTranslator>
#include "framelesswindow.h"
#include <QSerialPort>
#include "consolemode/consolewidget.h"
#include "hexmode/tableconsole.h"
#include "serialgui.h"
#include "decorator.h"
#include "converter.h"
#include "guicontroller.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(FramelessWindow *parent = nullptr);
    ~MainWidget();

    void applyColorScheme(int indexOfTheme);
    void setAppFont();
    void setAppTextCodec(int index);
    void setAppLanguage(int index);
    void fillLanguageList();
    void fillCodecList();
    void fillThemeList();

private:
    Ui::MainWidget     *ui           = nullptr;
    SerialGui          *serial       = nullptr;
    ConsoleWidget      *console      = nullptr;
    TableConsole       *tableConsole = nullptr;
    Converter          *converter    = nullptr;
    GuiController      *settings     = nullptr;
    QStringList        *codecList    = nullptr;
    QStringList        *languageList = nullptr;
    QList<Decorator*>  *themeList    = nullptr;

    Decorator          *darkTheme      = nullptr;
    Decorator          *greenTheme     = nullptr;
    Decorator          *bitbucketTheme = nullptr;
    Decorator          *githubTheme    = nullptr;
    Decorator          *purpleTheme    = nullptr;

    QTranslator        language;

    QFont *appFont = new QFont("Terminus", 10, QFont::ExtraBold);
};
#endif // MAINWIDGET_H



