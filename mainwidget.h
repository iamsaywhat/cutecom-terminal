#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QTranslator>
#include "framelesswindow.h"
#include "uiproxy.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public FramelessWindow
{
    Q_OBJECT

public:
    MainWidget(FramelessWindow *parent = nullptr);
    ~MainWidget();

    enum settingsMenu {                // Индексы вкладок меню настроек
        settingsIndexConnection = 0,
        settingsIndexGeneral    = 1,
        settingsIndexConsole    = 2,
        settingsIndexTable      = 3,
        settingsIndexLogs       = 4,
        settingsIndexBinds      = 5,
    };
    enum quickMenu {                   // Индексы основного меню
        quickIndexConsole   = 0,
        quickIndexTable     = 1,
        quickIndexConverter = 2,
        quickIndexSettings  = 3,
    };

    void saveSettings(void);                              // Сохранение настроек в conf-файл
    void restoreSettings(void);                           // Восстановление настроек из conf-файла

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

    void setProperties (void);
    void retranstate(void);

private:
    Ui::MainWidget *gui = nullptr;
    UiProxy *uiProxy;
    QRegularExpressionValidator *hexValidator = new QRegularExpressionValidator(
                                                   QRegularExpression("[0-9A-Fa-f ]+"));

private slots:
    void consoleCyclicStoped(void);
    void tableCyclicStoped(void);
    void consoleHotkeys(void);                // Обработка нажатий горячих клавиш "консоли"
    void tableHotkeys(void);                  // Обработка нажатий горячих клавиш "таблицы"
    void hexDelimiters(const QString&);       // Авторасстановка разделителей hex-кодов
    void generalSettingsChanged(void);        //
    void selectLogPath(void);                 // Обработка кнопки выбора папки для логов
};
#endif // MAINWIDGET_H



