#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QWidget>
#include <QScrollBar>
#include <QString>

#include "decorator.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    /* Выполняем связку класса работы с портом окном настроек */
    serial = new SerialGui(ui->boxPorts,                   /* ComboBox c доступными Com портами */
                           ui->boxBaudrate,                /* ComboBox с настройками скорости */
                           ui->boxParity,                  /* ComboBox с настройками паритета */
                           ui->boxData,                    /* ComboBox с настройками бит данных */
                           ui->boxStopBits,                /* ComboBox с настройками стоп-бит */
                           ui->boxFlowControl,             /* ComboBox с настройками контроля */
                           ui->buttonConnectDisconnect);   /* Кнопка подключения/отключения */
    /* Выполняем связку класса работы с консолью с формами */
    console = new ConsoleWidget(this,
                                serial,                    /* Указатель на SerialSettings экземпляр */
                                ui->consoleField,          /* Указатель на QPlainTextEdit форму*/
                                ui->inputConsoleField,     /* Указатель на QLineEdit форму */
                                ui->sendConsoleButton,     /* Указатель на QPushButton форму*/
                                ui->clearConsoleButton);   /* Указатель на QPushButton форму*/
    /* Создаём и связываем табличную консоль с формой */
    tableConsole = new TableConsole(this,
                                    serial,                /* Указатель на SerialSettings экземпляр */
                                    ui->tableField,        /* Указатель на QTableView форму */
                                    ui->inputTableField,   /* Указатель на QLineEdit форму */
                                    ui->sendTableButton,   /* Указатель на QPushButton форму*/
                                    ui->clearTableButton); /* Указатель на QPushButton форму*/

    darkTheme = new Decorator(0x1a1c20, 0x2b2d33, 0x33363d, 0xdcddde, 0x3d563d);

    /* Включаем сетку на таблице */
    ui->tableField->setShowGrid(true);

    /* Внешний вид приложения */
    setPropertiesToMainWidget();
    setPropertiesToConsole();
    setPropertiesToTable();
    setPropertiesToConverter();
    setPropertiesToSettingsWidget();
    applyColorScheme(darkTheme);


    /* Подключение кнопок закрыть, свернуть, развернуть окно, так как стандартные скрыты */
    connect(ui->closeButton,    &QToolButton::clicked, parent, &QWidget::close);
    connect(ui->minimizeButton, &QToolButton::clicked, parent, &QWidget::showMinimized);
    connect(ui->maximazeButton, &QToolButton::clicked, [parent](){
        // При нажатии на кнопку максимизации/нормализации окна
        // Делаем проверку на то, в каком состоянии находится окно и переключаем его режим
        if (!parent->isMaximized())
        {
            //ui->maximazeButton->setStyleSheet(StyleHelper::getRestoreStyleSheet());

            /* Здесь важный нюанс: при разворачивании окна - необходимо поля centralwidget
             * (на уровень выше interfaceWidget) убрать, чтобы окно полноценно развернулось
             * в полный экран, однако, при этом исчезает тень, которую в полноэкранном режиме
             * и не должно быть видно, но при минимизации окна нужно вернуть */
            parent->layout()->setMargin(0);
            parent->showMaximized();
        }
        else
        {
            // Заметьте, каждый раз устанавливаем новый стиль в эту кнопку
            //ui->maximazeButton->setStyleSheet(StyleHelper::getMaximizeStyleSheet());
            /* Здесь при минимизации возвращаем поля в исходный вид,
             * чтобы тень отобразилась */
            parent->layout()->setMargin(10);
            parent->showNormal();
        }
    });


    /* Дополнительные функциональные кнопки */
    connect(ui->showConnectionButton,    &QPushButton::clicked, [this](){ui->workspaceWidget->setCurrentIndex(quickIndexSettings);
                                                                         ui->rightStackedPanel->setCurrentIndex(settingsIndexConnection);});
    connect(ui->switchToConsoleButton,   &QPushButton::clicked, [this](){ui->workspaceWidget->setCurrentIndex(quickIndexConsole);});
    connect(ui->switchToTableButton,     &QPushButton::clicked, [this](){ui->workspaceWidget->setCurrentIndex(quickIndexTable);});
    connect(ui->switchToConverterButton, &QPushButton::clicked, [this](){ui->workspaceWidget->setCurrentIndex(quickIndexConverter);});
    connect(ui->showSettingsButton,      &QPushButton::clicked, [this](){ui->workspaceWidget->setCurrentIndex(quickIndexSettings);});

    /* Подключения для меню настроек */
    connect(ui->connectionContentsButton, &QPushButton::clicked, [this](){ui->rightStackedPanel->setCurrentIndex(settingsIndexConnection);});
    connect(ui->generalContentButton,     &QPushButton::clicked, [this](){ui->rightStackedPanel->setCurrentIndex(settingsIndexGeneral);});
    connect(ui->consoleContentButton,     &QPushButton::clicked, [this](){ui->rightStackedPanel->setCurrentIndex(settingsIndexConsole);});
    connect(ui->tableContentButton,       &QPushButton::clicked, [this](){ui->rightStackedPanel->setCurrentIndex(settingsIndexTable);});
    connect(ui->logsContentButton,        &QPushButton::clicked, [this](){ui->rightStackedPanel->setCurrentIndex(settingsIndexLogs);});
    connect(ui->bindsContentButton,       &QPushButton::clicked, [this](){ui->rightStackedPanel->setCurrentIndex(settingsIndexBinds);});
}

MainWidget::~MainWidget()
{
    delete darkTheme;
    delete console;
    delete serial;
    delete ui;
}


void MainWidget::setPropertiesToMainWidget(void){
    ui->closeButton->setText("");
    ui->maximazeButton->setText("");
    ui->minimizeButton->setText("");
    ui->appName->setText("");
    ui->appName->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    ui->showConnectionButton->setMinimumSize(50,50);
    ui->showConnectionButton->setMinimumSize(50,50);
    ui->showSettingsButton->setMinimumSize(50,50);
    ui->switchToConsoleButton->setMinimumSize(50,50);
    ui->switchToTableButton->setMinimumSize(50,50);
    ui->switchToConverterButton->setMinimumSize(50,50);
    ui->showConnectionButton->setText("");
    ui->showConnectionButton->setText("");
    ui->showSettingsButton->setText("");
    ui->switchToConsoleButton->setText("");
    ui->switchToTableButton->setText("");
    ui->switchToConverterButton->setText("");
}

void MainWidget::setPropertiesToConsole(void){

}

void MainWidget::setPropertiesToTable(void){

}

void MainWidget::setPropertiesToConverter(void){

}

void MainWidget::setPropertiesToSettingsWidget(void){
    /* Настройка левой панели */
    ui->connectionContentsButton->setText("Connection");
    ui->generalContentButton->setText("General");
    ui->consoleContentButton->setText("Console");
    ui->tableContentButton->setText("Table");
    ui->logsContentButton->setText("Logs");
    ui->bindsContentButton->setText("Binds");
    ui->infoLabel->setText("Version: 0.0.0");
    ui->settingsPage->layout()->setMargin(0);
    ui->pageDelimiterLayout->setMargin(0);
    ui->leftPanelContents->layout()->setMargin(0);
    ui->rightPanelContents->layout()->setMargin(0);
    ui->rightStackedPanel->layout()->setMargin(0);
    ui->pageDelimiterLayout->setSpacing(0);
    /* Настройка правой панели */
    ui->labelSectionConnection->setText("Connection");
}

void MainWidget::applyColorSchemeToMainWidget(Decorator *scheme){
    scheme->setBasicColorsToWidget  (this, scheme->baseColor(), scheme->textColor());
    scheme->applyToAppLabel         (ui->appName);
    scheme->applyToCloseButton      (ui->closeButton);
    scheme->applyToMaximizeButton   (ui->maximazeButton);
    scheme->applyToMinimizeButton   (ui->minimizeButton);
    scheme->applyToConnectionButton (ui->showConnectionButton);
    scheme->applyToConsoleButton    (ui->switchToConsoleButton);
    scheme->applyToTableButton      (ui->switchToTableButton);
    scheme->applyToConverterButton  (ui->switchToConverterButton);
    scheme->applyToSettingsButton   (ui->showSettingsButton);
}
void MainWidget::applyColorSchemetoConsole(Decorator *scheme){
    scheme->applyToConsoleWidget    (ui->consoleField, ui->inputConsoleField, ui->sendConsoleButton, ui->clearConsoleButton);
}

void MainWidget::applyColorSchemeToTable(Decorator *scheme){
    scheme->applyToTableWidget      (ui->tableField, ui->inputTableField, ui->sendTableButton, ui->clearTableButton);
}

void MainWidget::applyColorSchemeToConverter(Decorator *scheme){

}

void MainWidget::applyColorSchemeToSettings(Decorator *scheme){
    scheme->setScrollBarColors      (ui->settingsLeftPanel->horizontalScrollBar(), scheme->secondColor(), scheme->baseColor());
    scheme->setScrollBarColors      (ui->settingsLeftPanel->verticalScrollBar(), scheme->secondColor(), scheme->baseColor());
    scheme->setBasicColorsToWidget  (ui->leftPanelContents, scheme->baseColor(), scheme->textColor());
    scheme->setBasicColorsToWidget  (ui->settingsPage, scheme->baseColor(), scheme->textColor());
    scheme->setSettingsButtonsColors(ui->connectionContentsButton, scheme->baseColor(), scheme->thirdColor(), scheme->secondColor());
    scheme->setSettingsButtonsColors(ui->generalContentButton, scheme->baseColor(), scheme->thirdColor(), scheme->secondColor());
    scheme->setSettingsButtonsColors(ui->consoleContentButton, scheme->baseColor(), scheme->thirdColor(), scheme->secondColor());
    scheme->setSettingsButtonsColors(ui->tableContentButton, scheme->baseColor(), scheme->thirdColor(), scheme->secondColor());
    scheme->setSettingsButtonsColors(ui->logsContentButton, scheme->baseColor(), scheme->thirdColor(), scheme->secondColor());
    scheme->setSettingsButtonsColors(ui->bindsContentButton, scheme->baseColor(), scheme->thirdColor(), scheme->secondColor());
    scheme->setScrollBarColors      (ui->settingsRightPanel->horizontalScrollBar(), scheme->secondColor(), scheme->baseColor());
    scheme->setScrollBarColors      (ui->settingsRightPanel->verticalScrollBar(), scheme->secondColor(), scheme->baseColor());
    scheme->setBasicColorsToWidget  (ui->rightPanelContents, scheme->secondColor(), scheme->textColor());
    scheme->setBasicColorsToWidget  (ui->rightStackedPanel, scheme->secondColor(), scheme->textColor());
    scheme->setBasicColorsToWidget  (ui->labelSectionConnection, scheme->secondColor(), scheme->textColor());
    scheme->setBasicColorsToWidget  (ui->labelPorts, scheme->secondColor(), scheme->textColor());
    scheme->setBasicColorsToWidget  (ui->labelBaudrate, scheme->secondColor(), scheme->textColor());
    scheme->setBasicColorsToWidget  (ui->labelData, scheme->secondColor(), scheme->textColor());
    scheme->setBasicColorsToWidget  (ui->labelParity, scheme->secondColor(), scheme->textColor());
    scheme->setBasicColorsToWidget  (ui->labelStopBits, scheme->secondColor(), scheme->textColor());
    scheme->setBasicColorsToWidget  (ui->labelFlowControl, scheme->secondColor(), scheme->textColor());

    scheme->setComboBoxColors(ui->boxPorts);
    scheme->setComboBoxColors(ui->boxData);
    scheme->setComboBoxColors(ui->boxParity);
    scheme->setComboBoxColors(ui->boxBaudrate);
    scheme->setComboBoxColors(ui->boxStopBits);
    scheme->setComboBoxColors(ui->boxFlowControl);

}

void MainWidget::applyColorScheme(Decorator *scheme){
    /* Основная рамка */
    applyColorSchemeToMainWidget(scheme);
    /* Виджет консоли */
    applyColorSchemetoConsole(scheme);
    /* Виджет таблицы */
    applyColorSchemeToTable(scheme);
    /* Виджет настроек */
    applyColorSchemeToSettings(scheme);
}



//void MainWidget::applySettingsStylesheet(void) {
//    ///////////////////////////////////////////////////////////////////////////////////
//        //    ui->settingsPage;
//        //        ui->pageDelimiterLayout;
//        //            ui->settingsLeftPanel;
//        //                ui->leftPanelContents;
//        //            ui->settingsRightPanel;
//        //                ui->rightPanelContents;
//        //                  ui->rightStackedPanel;

//        ui->settingsPage->layout()->setMargin(5);
//            ui->pageDelimiterLayout->setMargin(5);
//    //            ui->settingsLeftPanel->layout()->setMargin(5);
//                    ui->leftPanelContents->layout()->setMargin(5);
//    //            ui->settingsRightPanel->layout()->setMargin(5);
//                    ui->rightPanelContents->layout()->setMargin(5);
//                     ui->rightStackedPanel->layout()->setMargin(5);

//        ui->settingsPage->setStyleSheet(Decorator::getSipmleWidgetStyleSheet(0xFFFFFF, 0x100000));
//            ui->pageDelimiterLayout->setMargin(5);
//                    ui->leftPanelContents->setStyleSheet(Decorator::getSipmleWidgetStyleSheet(0xFF0000, 0x100000));
//                ui->settingsRightPanel->setStyleSheet(Decorator::getSipmleWidgetStyleSheet(0xFFFFFF, 0x100000));
//                    ui->rightPanelContents->setStyleSheet(Decorator::getSipmleWidgetStyleSheet(0xFF0000, 0x100000));
//                        ui->rightStackedPanel->setStyleSheet(Decorator::getSipmleWidgetStyleSheet(0xFF00FF, 0x100000));

//    ////////////////////////////////////////////////////////////


//    /* Настройка левой панели **************************************************************/
//    ui->connectionContentsButton->setText("Connection");
//    ui->generalContentButton->setText("General");
//    ui->consoleContentButton->setText("Console");
//    ui->tableContentButton->setText("Table");
//    ui->logsContentButton->setText("Logs");
//    ui->bindsContentButton->setText("Binds");
//    ui->infoLabel->setText("Version: 0.0.0");

//    ui->settingsPage->layout()->setMargin(0);
//    ui->pageDelimiterLayout->setMargin(0);
//    ui->leftPanelContents->layout()->setMargin(0);
//    ui->rightPanelContents->layout()->setMargin(0);
//    ui->rightStackedPanel->layout()->setMargin(0);
//    ui->pageDelimiterLayout->setSpacing(0);

//    darkTheme->setScrollBarColors(ui->settingsLeftPanel->horizontalScrollBar(), darkTheme->secondColor(), darkTheme->baseColor());
//    darkTheme->setScrollBarColors(ui->settingsLeftPanel->verticalScrollBar(), darkTheme->secondColor(), darkTheme->baseColor());
//    darkTheme->setBasicColorsToWidget(ui->leftPanelContents, darkTheme->baseColor(), darkTheme->textColor());
//    darkTheme->setBasicColorsToWidget(ui->settingsPage, darkTheme->baseColor(), darkTheme->textColor());

//    darkTheme->setSettingsButtonsColors(ui->connectionContentsButton, darkTheme->baseColor(), darkTheme->thirdColor(), darkTheme->secondColor());
//    darkTheme->setSettingsButtonsColors(ui->generalContentButton, darkTheme->baseColor(), darkTheme->thirdColor(), darkTheme->secondColor());
//    darkTheme->setSettingsButtonsColors(ui->consoleContentButton, darkTheme->baseColor(), darkTheme->thirdColor(), darkTheme->secondColor());
//    darkTheme->setSettingsButtonsColors(ui->tableContentButton, darkTheme->baseColor(), darkTheme->thirdColor(), darkTheme->secondColor());
//    darkTheme->setSettingsButtonsColors(ui->logsContentButton, darkTheme->baseColor(), darkTheme->thirdColor(), darkTheme->secondColor());
//    darkTheme->setSettingsButtonsColors(ui->bindsContentButton, darkTheme->baseColor(), darkTheme->thirdColor(), darkTheme->secondColor());

//    /* Настройка правой панели **************************************************************/
//    ui->labelSectionConnection->setText("Connection");
//    darkTheme->setScrollBarColors(ui->settingsRightPanel->horizontalScrollBar(), darkTheme->secondColor(), darkTheme->baseColor());
//    darkTheme->setScrollBarColors(ui->settingsRightPanel->verticalScrollBar(), darkTheme->secondColor(), darkTheme->baseColor());
//    darkTheme->setBasicColorsToWidget(ui->rightPanelContents, darkTheme->secondColor(), darkTheme->textColor());
//    darkTheme->setBasicColorsToWidget(ui->rightStackedPanel, darkTheme->secondColor(), darkTheme->textColor());
//    /* Вкладка Connection */
//    darkTheme->setBasicColorsToWidget(ui->labelSectionConnection, darkTheme->secondColor(), darkTheme->textColor());
//    darkTheme->setBasicColorsToWidget(ui->labelPorts, darkTheme->secondColor(), darkTheme->textColor());
//    darkTheme->setBasicColorsToWidget(ui->labelBaudrate, darkTheme->secondColor(), darkTheme->textColor());
//    darkTheme->setBasicColorsToWidget(ui->labelData, darkTheme->secondColor(), darkTheme->textColor());
//    darkTheme->setBasicColorsToWidget(ui->labelParity, darkTheme->secondColor(), darkTheme->textColor());
//    darkTheme->setBasicColorsToWidget(ui->labelStopBits, darkTheme->secondColor(), darkTheme->textColor());
//    darkTheme->setBasicColorsToWidget(ui->labelFlowControl, darkTheme->secondColor(), darkTheme->textColor());



//    //    ui->boxPorts->setStyleSheet       (Decorator::getComboBoxStyleSheet());
//    //    ui->boxBaudrate->setStyleSheet    (Decorator::getComboBoxStyleSheet());
//    //    ui->boxParity->setStyleSheet      (Decorator::getComboBoxStyleSheet());
//    //    ui->boxData->setStyleSheet        (Decorator::getComboBoxStyleSheet());
//    //    ui->boxStopBits->setStyleSheet    (Decorator::getComboBoxStyleSheet());
//    //    ui->boxFlowControl->setStyleSheet (Decorator::getComboBoxStyleSheet());


//}
