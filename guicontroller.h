#ifndef GUICONTROLLER_H
#define GUICONTROLLER_H

#include "ui_mainwidget.h"
#include "decorator.h"
#include "converter.h"

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
    void retranstate(void);

    void setLanguageList(QStringList* list);
    void setCodecList(QStringList* list);
    void setThemeList(QStringList* list);
    void restore(void);
    void save(void);

private:
    static Ui::MainWidget *gui;
    QRegularExpressionValidator *hexValidator = new QRegularExpressionValidator(
                                                    QRegularExpression("[0-9A-Fa-f ]+"));
    bool eventFilter(QObject *target, QEvent *event);
    void connectQuickMenu(void);
    void connectSettingsMenu(void);
    void connectSectionConnection(void);
    void connectSectionGeneral(void);
    void connectSectionConsole(void);
    void connectSectionTable(void);
    void connectSectionLog(void);

    void setPropertiesMainWidget(void);
    void setPropertiesMenu(void);
    void setPropertiesSectionConnection(void);
    void setPropertiesSectionGeneral(void);
    void setPropertiesSectionConsole(void);
    void setPropertiesSectionTable(void);
    void setPropertiesSectionLogs(void);
    void setPropertiesSectionBinds(void);

    void setTextSettingsMenu(void);
    void setTextSectionConnection(void);
    void setTextSectionGeneral(void);
    void setTextSectionConsole(void);
    void setTextSectionTable(void);
    void setTextSectionLogs(void);
    void setTextSectionBinds(void);

signals:
    void restoredSize(QRect);

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

    void logEnableChanged(bool);
    void logPathChanged(QString);
    void logColumnSizeChanged(int);
    void logColumnSpacing(int);

private slots:
    void consoleHotkeys(void);                // Обработка нажатий горячих клавиш "консоли"
    void tableHotkeys(void);                  // Обработка нажатий горячих клавиш "таблицы"
    void hexDelimiters(const QString&);       // Авторасстановка разделителей hex-кодов
    void selectLogPath(void);                 // Обработка кнопки выбора папки для логов
    void generalSettingsChanged(void);

public slots:
    void consoleCyclicStoped(void);
    void tableCyclicStoped(void);
};

#endif // GUICONTROLLER_H
