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
    applyTopPanelStyleSheet();
    applyConsoleStyleSheet();
    applyTableStyleSheet();
    applySettingsStylesheet();


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
    connect(ui->showConnectionButton,    &QPushButton::clicked, [this](){ui->workspaceWidget->setCurrentIndex(indexSettings);});
    connect(ui->switchToConsoleButton,   &QPushButton::clicked, [this](){ui->workspaceWidget->setCurrentIndex(indexConsole);});
    connect(ui->switchToTableButton,     &QPushButton::clicked, [this](){ui->workspaceWidget->setCurrentIndex(indexTable);});
    connect(ui->switchToConverterButton, &QPushButton::clicked, [this](){ui->workspaceWidget->setCurrentIndex(indexConverter);});
    connect(ui->showSettingsButton,      &QPushButton::clicked, [this](){ui->workspaceWidget->setCurrentIndex(indexSettings);});

    /* Подключения для меню настроек */
    connect(ui->connectionContentsButton, &QPushButton::clicked, [this](){ui->rightStackedPanel->setCurrentIndex(0);});
    connect(ui->generalContentButton,     &QPushButton::clicked, [this](){ui->rightStackedPanel->setCurrentIndex(1);});
}

MainWidget::~MainWidget()
{
    delete darkTheme;
    delete console;
    delete serial;
    delete ui;
}

void MainWidget::applyTopPanelStyleSheet(void) {
    ui->closeButton->setText("");
    ui->maximazeButton->setText("");
    ui->minimizeButton->setText("");
    ui->appName->setText("");

    //this->setStyleSheet(Decorator::getMainWidgetStyleSheet(0x1a1c20, 0xdcddde));
    darkTheme->applyBasicColorsToWidget(this, darkTheme->baseColor(), darkTheme->textColor());


    darkTheme->applyToCloseButton(ui->closeButton);
    darkTheme->applyToMaximizeButton(ui->maximazeButton);
    darkTheme->applyToMinimizeButton(ui->minimizeButton);
    darkTheme->applyToConnectionButton(ui->showConnectionButton);
    darkTheme->applyToConsoleButton(ui->switchToConsoleButton);
    darkTheme->applyToTableButton(ui->switchToTableButton);
    darkTheme->applyToConverterButton(ui->switchToConverterButton);
    darkTheme->applyToSettingsButton(ui->showSettingsButton);


    ui->appName->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    darkTheme->applyToAppLabel(ui->appName);

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

void MainWidget::applyConsoleStyleSheet(void) {
    darkTheme->applyToConsoleWidget(ui->consoleField, ui->inputConsoleField, ui->sendConsoleButton, ui->clearConsoleButton);
}

void MainWidget::applyTableStyleSheet(void) {
    darkTheme->applyToTableWidget(ui->tableField, ui->inputTableField, ui->sendTableButton, ui->clearTableButton);
}

void MainWidget::applySettingsStylesheet(void) {
    ///////////////////////////////////////////////////////////////////////////////////
        //    ui->settingsPage;
        //        ui->pageDelimiterLayout;
        //            ui->settingsLeftPanel;
        //                ui->leftPanelContents;
        //            ui->settingsRightPanel;
        //                ui->rightPanelContents;
        //                  ui->rightStackedPanel;

        ui->settingsPage->layout()->setMargin(5);
            ui->pageDelimiterLayout->setMargin(5);
    //            ui->settingsLeftPanel->layout()->setMargin(5);
                    ui->leftPanelContents->layout()->setMargin(5);
    //            ui->settingsRightPanel->layout()->setMargin(5);
                    ui->rightPanelContents->layout()->setMargin(5);
                     ui->rightStackedPanel->layout()->setMargin(5);

        ui->settingsPage->setStyleSheet(Decorator::getSipmleWidgetStyleSheet(0xFFFFFF, 0x100000));
            ui->pageDelimiterLayout->setMargin(5);
                    ui->leftPanelContents->setStyleSheet(Decorator::getSipmleWidgetStyleSheet(0xFF0000, 0x100000));
                ui->settingsRightPanel->setStyleSheet(Decorator::getSipmleWidgetStyleSheet(0xFFFFFF, 0x100000));
                    ui->rightPanelContents->setStyleSheet(Decorator::getSipmleWidgetStyleSheet(0xFF0000, 0x100000));
                        ui->rightStackedPanel->setStyleSheet(Decorator::getSipmleWidgetStyleSheet(0xFF00FF, 0x100000));

    ////////////////////////////////////////////////////////////


    /* Настройка левой панели **************************************************************/
    ui->connectionContentsButton->setText("Connection");
    ui->generalContentButton->setText("General");
    ui->consoleContentButton->setText("Console");
    ui->tableContentButton->setText("Table");
    ui->logsContentButton->setText("Logs");
    ui->bindsContentButton->setText("Binds");
    ui->infoLabel->setText("Version: 0.0.0");
    ui->settingsLeftPanel->horizontalScrollBar()->setStyleSheet(Decorator::getScrollBarStyleSheet(0x2b2d33,0x1a1c20));
    ui->settingsLeftPanel->verticalScrollBar()->setStyleSheet(Decorator::getScrollBarStyleSheet(0x2b2d33,0x1a1c20));
    ui->leftPanelContents->setStyleSheet(Decorator::getSipmleWidgetStyleSheet(0x1a1c20, 0xdcddde));
    ui->connectionContentsButton->setStyleSheet(Decorator::getSettingsMenuButtonStyleSheet(0x2b2d33, 0x33363d, 0x2b2d33));
    ui->generalContentButton->setStyleSheet(Decorator::getSettingsMenuButtonStyleSheet(0x2b2d33, 0x33363d, 0x2b2d33));
    ui->consoleContentButton->setStyleSheet(Decorator::getSettingsMenuButtonStyleSheet(0x2b2d33, 0x33363d, 0x2b2d33));
    ui->tableContentButton->setStyleSheet(Decorator::getSettingsMenuButtonStyleSheet(0x2b2d33, 0x33363d, 0x2b2d33));
    ui->logsContentButton->setStyleSheet(Decorator::getSettingsMenuButtonStyleSheet(0x2b2d33, 0x33363d, 0x2b2d33));
    ui->bindsContentButton->setStyleSheet(Decorator::getSettingsMenuButtonStyleSheet(0x2b2d33, 0x33363d, 0x2b2d33));


    /* Настройка правой панели **************************************************************/
    ui->labelSectionConnection->setText("Connection");
    ui->settingsRightPanel->horizontalScrollBar()->setStyleSheet(Decorator::getScrollBarStyleSheet(0x2b2d33,0x1a1c20));
    ui->settingsRightPanel->verticalScrollBar()->setStyleSheet(Decorator::getScrollBarStyleSheet(0x2b2d33,0x1a1c20));
    ui->rightPanelContents->setStyleSheet(Decorator::getSipmleWidgetStyleSheet(0x2b2d33, 0xdcddde));
    ui->rightStackedPanel->setStyleSheet(Decorator::getSipmleWidgetStyleSheet(0x2b2d33, 0xdcddde));

    /* Вкладка Connection */
    ui->labelSectionConnection->setStyleSheet(Decorator::getScrollBarStyleSheet(0x2b2d33,0x1a1c20));
    ui->labelPorts->setStyleSheet            (Decorator::getScrollBarStyleSheet(0x2b2d33,0x1a1c20));
    ui->labelBaudrate->setStyleSheet         (Decorator::getScrollBarStyleSheet(0x2b2d33,0x1a1c20));
    ui->labelData->setStyleSheet             (Decorator::getScrollBarStyleSheet(0x2b2d33,0x1a1c20));
    ui->labelParity->setStyleSheet           (Decorator::getScrollBarStyleSheet(0x2b2d33,0x1a1c20));
    ui->labelStopBits->setStyleSheet         (Decorator::getScrollBarStyleSheet(0x2b2d33,0x1a1c20));
    ui->labelFlowControl->setStyleSheet      (Decorator::getScrollBarStyleSheet(0x2b2d33,0x1a1c20));


    //    ui->boxPorts->setStyleSheet       (Decorator::getComboBoxStyleSheet());
    //    ui->boxBaudrate->setStyleSheet    (Decorator::getComboBoxStyleSheet());
    //    ui->boxParity->setStyleSheet      (Decorator::getComboBoxStyleSheet());
    //    ui->boxData->setStyleSheet        (Decorator::getComboBoxStyleSheet());
    //    ui->boxStopBits->setStyleSheet    (Decorator::getComboBoxStyleSheet());
    //    ui->boxFlowControl->setStyleSheet (Decorator::getComboBoxStyleSheet());


}
