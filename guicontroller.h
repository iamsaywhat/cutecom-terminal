#ifndef GUICONTROLLER_H
#define GUICONTROLLER_H

#include "ui_mainwidget.h"
#include "decorator.h"

class GuiController : public QObject
{
    Q_OBJECT

public:
    explicit GuiController(QObject *parent = nullptr,
                           Ui::MainWidget* gui = nullptr);
    ~GuiController(void);
    enum settingsMenu {
        settingsIndexConnection = 0,
        settingsIndexGeneral    = 1,
        settingsIndexConsole    = 2,
        settingsIndexTable      = 3,
        settingsIndexLogs       = 4,
        settingsIndexBinds      = 5,
    };
    enum quickMenu {
        quickIndexConsole   = 0,
        quickIndexTable     = 1,
        quickIndexConverter = 2,
        quickIndexSettings  = 3,
    };
    void setProperties(void);
    void setPropertiesMainWidget(void);
    void setPropertiesMenu(void);
    void setPropertiesSectionConnection(void);
    void setPropertiesSectionGeneral(void);
    void setPropertiesSectionConsole(void);
    void setPropertiesSectionTable(void);
    void setPropertiesSectionLogs(void);
    void retranstate(void);

    void setLanguageList(QStringList* list);
    void setCodecList(QStringList* list);
    void setThemeList(QStringList* list);

private:
    static Ui::MainWidget *gui;
    bool eventFilter(QObject *target, QEvent *event);

signals:
    void currentThemeChanged(int index);
    void currentLanguageChanged(int index);
    void currentTextCodecChanged(int index);
    void captureTimeChanges(int value);
    void captureBytesChanges(int value);

    void consoleEchoChanged(bool state);
    void consoleCyclicChanged(bool state);
    void consoleCyclicIntervalChanged(int interval);
    void consoleCyclicDataChanged(QString);
    void consoleStartCycle(void);
    void consoleStopCycle(void);

    void tableEchoChanged(bool state);
    void tableCyclicChanged(bool state);
    void tableCyclicIntervalChanged(int interval);
    void tableCyclicDataChanged(QString);
    void tableStartCycle(void);
    void tableStopCycle(void);

private slots:
    void consoleHotkeys(void);
    void tableHotkeys(void);

public slots:
};

#endif // GUICONTROLLER_H
