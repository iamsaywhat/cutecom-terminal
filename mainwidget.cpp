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

    QString str("background: %1");
    str = str.arg("#1a1c20;");
    qDebug()<<str;
}

MainWidget::~MainWidget()
{
    delete console;
    delete serial;
    delete ui;
}

void MainWidget::applyTopPanelStyleSheet(void) {
    ui->closeButton->setText("");
    ui->maximazeButton->setText("");
    ui->minimizeButton->setText("");
    ui->appName->setText("");
    this->setStyleSheet(Decorator::getMainWidgetStyleSheet());
    ui->closeButton->setStyleSheet     (Decorator::getCloseButtonStyleSheet());
    ui->minimizeButton->setStyleSheet  (Decorator::getMinimizeButtonStyleSheet());
    ui->maximazeButton->setStyleSheet  (Decorator::getMaximizeButtonStyleSheet());

    ui->showConnectionButton->setStyleSheet   (Decorator::getConnectionButtonStyleSheet());
    ui->showSettingsButton->setStyleSheet     (Decorator::getSettingsButtonStyleSheet());
    ui->switchToConsoleButton->setStyleSheet  (Decorator::getConsoleModeButtonStyleSheet());
    ui->switchToTableButton->setStyleSheet    (Decorator::getTableModeButtonStyleSheet());
    ui->switchToConverterButton->setStyleSheet(Decorator::getConverterModeButtonStyleSheet());

    ui->appName->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    ui->appName->setStyleSheet(Decorator::getAppLabelStyleSheet());

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
    ui->consoleField->setFont(QFont("System", 8, QFont::Normal));
    ui->inputConsoleField->setFont(QFont("System", 8, QFont::Normal));
    ui->consoleField->setStyleSheet(Decorator::getConsoleStyleSheet());
    ui->sendConsoleButton->setStyleSheet(Decorator::getUsualButtonStyleSheet());
    ui->clearConsoleButton->setStyleSheet(Decorator::getUsualButtonStyleSheet());
    ui->inputConsoleField->setStyleSheet(Decorator::getInputFieldStyleSheet());
    ui->consoleField->verticalScrollBar()->setStyleSheet(Decorator::getScrollBarStyleSheet());
}

void MainWidget::applyTableStyleSheet(void) {
    ui->tableField->verticalScrollBar()->setStyleSheet(Decorator::getScrollBarStyleSheet());
    ui->tableField->horizontalScrollBar()->setStyleSheet(Decorator::getScrollBarStyleSheet());
    ui->tableField->setStyleSheet(Decorator::getTableStyleSheet());
    ui->inputTableField->setStyleSheet(Decorator::getInputFieldStyleSheet());
    ui->sendTableButton->setStyleSheet(Decorator::getUsualButtonStyleSheet());
    ui->clearTableButton->setStyleSheet(Decorator::getUsualButtonStyleSheet());
}

void MainWidget::applySettingsStylesheet(void) {

    /* Настройка левой панели */
    ui->connectionContentsButton->setText("Connection");
    ui->generalContentButton->setText("General");
    ui->consoleContentButton->setText("Console");
    ui->tableContentButton->setText("Table");
    ui->logsContentButton->setText("Logs");
    ui->bindsContentButton->setText("Binds");
    ui->infoLabel->setText("Version:????");

    ui->settingsLeftPanel->horizontalScrollBar()->setStyleSheet(Decorator::getScrollBarStyleSheet());
    ui->settingsLeftPanel->verticalScrollBar()->setStyleSheet(Decorator::getScrollBarStyleSheet());
    ui->settingsLeftPanel->setStyleSheet(Decorator::getLeftScrollAreaStyleSheet());
    ui->leftPanelContents->setStyleSheet(Decorator::getLeftScrollAreaStyleSheet());

    //ui->connectionContentsButton->setStyleSheet(Decorator::getSettingsMenuButtons());
    ui->connectionContentsButton->setStyleSheet(Decorator::getTestStyleSheet("#2b2d33", "#33363d", "#2b2d33"));

    ui->generalContentButton->setStyleSheet(Decorator::getSettingsMenuButtons());
    ui->consoleContentButton->setStyleSheet(Decorator::getSettingsMenuButtons());
    ui->tableContentButton->setStyleSheet(Decorator::getSettingsMenuButtons());
    ui->logsContentButton->setStyleSheet(Decorator::getSettingsMenuButtons());
    ui->bindsContentButton->setStyleSheet(Decorator::getSettingsMenuButtons());
    ui->infoLabel->setText("Version:????");


    ui->leftPanelContents->layout()->setMargin(0);
    ui->leftPanelContents->layout()->setSpacing(0);


    ui->pageDelimiterLayout->setSpacing(0); // Убирает границу между левой и правой панелями
    ui->pageDelimiterLayout->setMargin(0); // Убирает границу между левой и правой панелями


    /* Настройка правой панели */
    ui->settingsRightPanel->horizontalScrollBar()->setStyleSheet(Decorator::getScrollBarStyleSheet());
    ui->settingsRightPanel->verticalScrollBar()->setStyleSheet(Decorator::getScrollBarStyleSheet());
    ui->boxPorts->setStyleSheet       (Decorator::getComboBoxStyleSheet());
    ui->boxBaudrate->setStyleSheet    (Decorator::getComboBoxStyleSheet());
    ui->boxParity->setStyleSheet      (Decorator::getComboBoxStyleSheet());
    ui->boxData->setStyleSheet        (Decorator::getComboBoxStyleSheet());
    ui->boxStopBits->setStyleSheet    (Decorator::getComboBoxStyleSheet());
    ui->boxFlowControl->setStyleSheet (Decorator::getComboBoxStyleSheet());
    ui->labelSectionConnection->setText("Connection");

    ui->settingsRightPanel->setStyleSheet(Decorator::getRightScrollAreaStyleSheet());
    ui->rightPanelContents->setStyleSheet(Decorator::getRightScrollAreaStyleSheet());
    ui->rightStackedPanel->setStyleSheet(Decorator::getRightScrollAreaStyleSheet());
    ui->rightPanelContents->layout()->setMargin(0);   // Убирает рамку вокруг StackedWidget

    ui->labelSectionConnection->setStyleSheet(Decorator::getRightScrollAreaStyleSheet());
    ui->labelPorts->setStyleSheet(Decorator::getRightScrollAreaStyleSheet());
    ui->labelBaudrate->setStyleSheet(Decorator::getRightScrollAreaStyleSheet());
    ui->labelData->setStyleSheet(Decorator::getRightScrollAreaStyleSheet());
    ui->labelParity->setStyleSheet(Decorator::getRightScrollAreaStyleSheet());
    ui->labelStopBits->setStyleSheet(Decorator::getRightScrollAreaStyleSheet());
    ui->labelFlowControl->setStyleSheet(Decorator::getRightScrollAreaStyleSheet());


}
