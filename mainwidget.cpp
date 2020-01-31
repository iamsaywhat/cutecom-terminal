#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QWidget>

#include "decorator.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    /* Создаём окно настроек COM */
    settingsWindow = new SettingsDialog(this);
    /* Выполняем связку класса работы с портом окном настроек */
    serial = new SerialGui(settingsWindow->Ports,                // ComboBox c доступными Com портами
                              settingsWindow->Baudrate,             // ComboBox с настройками скорости
                              settingsWindow->Parity,               // ComboBox с настройками паритета
                              settingsWindow->Databits,             // ComboBox с настройками бит данных
                              settingsWindow->Stopbits,             // ComboBox с настройками стоп-бит
                              settingsWindow->Flowcontrol,          // ComboBox с настройками контроля
                              settingsWindow->ConnectDisconnect);   // Кнопка подключения/отключения
    /* Выполняем связку класса работы с консолью с формами */
    console = new ConsoleWidget(this,
                                serial,               /* Указатель на SerialSettings экземпляр */
                                ui->consoleField,        /* Указатель на QPlainTextEdit форму*/
                                ui->inputConsoleField,          /* Указатель на QLineEdit форму */
                                ui->sendConsoleButton,       /* Указатель на QPushButton форму*/
                                ui->clearConsoleButton);     /* Указатель на QPushButton форму*/
    /* Создаём и связываем табличную консоль с формой */
    tableConsole = new TableConsole(this,
                                    serial,                  /* Указатель на SerialSettings экземпляр */
                                    ui->tableField,           /* Указатель на QTableView форму */
                                    ui->inputTableField,        /* Указатель на QLineEdit форму */
                                    ui->sendTableButton,     /* Указатель на QPushButton форму*/
                                    ui->clearTableButton);   /* Указатель на QPushButton форму*/


    /* Включаем сетку на таблице */
    ui->tableField->setShowGrid(true);

    /* Внешний вид приложения */
    this->setStyleSheet(Decorator::getMainWidgetStyleSheet());
    ui->closeButton->setStyleSheet(Decorator::getCloseButtonStyleSheet());
    ui->minimizeButton->setStyleSheet(Decorator::getMinimizeButtonStyleSheet());
    ui->maximazeButton->setStyleSheet(Decorator::getMaximizeButtonStyleSheet());
    ui->connectionButton->setStyleSheet(Decorator::getConnectionButtonStyleSheet());
    ui->consoleField->setStyleSheet(Decorator::getConsoleStyleSheet());
    //ui->consoleField->setStyleSheet( "background-color: #FFAA00;" );
    //ui->consolePage->setStyleSheet( "background-color: #FFAA00;" );

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
    connect(ui->connectionButton, &QToolButton::clicked, settingsWindow, &SettingsDialog::show);


    // При запуске будем предлагать подключение
    //settingsWindow->show();
}

MainWidget::~MainWidget()
{
    delete console;
    delete serial;
    delete settingsWindow;
    delete ui;
}

