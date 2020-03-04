#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include "ui_mainwidget.h"
#include "decorator.h"

class SettingsController : public QObject
{
    Q_OBJECT

public:
    explicit SettingsController(QObject *parent = nullptr,
                                Ui::MainWidget* gui = nullptr);
    ~SettingsController(void);
    enum settingsMenu {
        settingsIndexConnection = 0,
        settingsIndexGeneral    = 1,
        settingsIndexConsole    = 2,
        settingsIndexTable      = 3,
        settingsIndexLogs       = 4,
        settingsIndexBinds      = 5,
    };
    void setProperties(void);
    void setPropertiesMenu(void);
    void setPropertiesSectionConnection(void);
    void setPropertiesSectionGeneral(void);
    void setPropertiesSectionConsole(void);
    void setPropertiesSectionTable(void);
    void setPropertiesSectionLogs(void);

    void setLanguageList(QStringList* list);
    void setCodecList(QStringList* list);
    void setThemeList(QStringList* list);

private:
    Ui::MainWidget *gui;

signals:
    void currentThemeChanged(int index);
    void currentLanguageChanged(int index);
    void currentTextCodecChanged(int index);
    void consoleEchoChanged();
    void tableEchoChanged();

public slots:
};

#endif // SETTINGSCONTROLLER_H
