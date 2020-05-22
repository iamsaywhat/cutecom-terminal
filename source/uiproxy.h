#ifndef UIPROXY_H
#define UIPROXY_H

#include <ui_mainwidget.h>
#include <QObject>
#include <QComboBox>
#include <QPushButton>
#include <QTranslator>

#include "terminal.h"
#include "table.h"
#include "serialgui.h"
#include "decorator.h"
#include "converter.h"
#include "logger.h"

class UiProxy : public QObject
{
    Q_OBJECT

public:
    explicit UiProxy(QObject *parent = nullptr,
                     Ui::MainWidget* gui = nullptr);
    ~UiProxy(void);

    void setTheme(int index);
    void setFont();
    void setTextCodec(int index);
    void setLanguage(int index);
    void createLanguageList(QComboBox *comboBox);
    void createCodecList(QComboBox *comboBox);
    void createThemeList(QComboBox *comboBox);

private:
    SerialGui *port = nullptr;
    Terminal *terminal = nullptr;
    Table *table = nullptr;
    Converter *converter = nullptr;
    Logger *logger = nullptr;

    Decorator *darkTheme      = nullptr;
    Decorator *greenTheme     = nullptr;
    Decorator *bitbucketTheme = nullptr;
    Decorator *githubTheme    = nullptr;
    Decorator *purpleTheme    = nullptr;

    QStringList *codecList = nullptr;
    QStringList *languageList = nullptr;
    QStringList *themeNameList = nullptr;
    QList<Decorator*> *themeList = nullptr;

    QTranslator language;
    QFont *appFont = new QFont("Terminus", 10, QFont::ExtraBold);

signals:
    void consoleEchoChanged(bool);
    void consoleCyclicChanged(bool);
    void consoleCyclicIntervalChanged(int);
    void consoleCyclicDataChanged(QString);
    void consoleStartCycle(void);
    void consoleStopCycle(void);
    void consoleCyclicStoped(void);

    void tableEchoChanged(bool);
    void tableCyclicChanged(bool);
    void tableCyclicIntervalChanged(int);
    void tableCyclicDataChanged(QString);
    void tableStartCycle(void);
    void tableStopCycle(void);
    void tableCyclicStoped(void);

    void logEnableChanged(bool);
    void logPathChanged(QString);
    void logColumnSizeChanged(int);
    void logColumnSpacing(int);

    void captureTimeChanges(int);
    void captureBytesChanges(int);
};

#endif // UIPROXY_H
