#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QWidget>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Создаём окно настроек COM */
    settingsWindow = new SettingsDialog(this);
    /* Выполняем связку класса работы с портом окном настроек */
    serial = new SerialForGUI(settingsWindow->Ports,                // ComboBox c доступными Com портами
                              settingsWindow->Baudrate,             // ComboBox с настройками скорости
                              settingsWindow->Parity,               // ComboBox с настройками паритета
                              settingsWindow->Databits,             // ComboBox с настройками бит данных
                              settingsWindow->Stopbits,             // ComboBox с настройками стоп-бит
                              settingsWindow->Flowcontrol,          // ComboBox с настройками контроля
                              settingsWindow->ConnectDisconnect);   // Кнопка подключения/отключения
    /* Выполняем связку класса работы с консолью с формами */
    console = new ConsoleWindow(this,
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



    setWindowFlags(Qt::FramelessWindowHint);               /* Отключаем оформление окна */
    setAttribute(Qt::WA_TranslucentBackground);            /* Делаем фон главного окна прозрачным */
    ui->interfaceWidget->setAutoFillBackground(true);      /* По-умолчанию фон нашего интефейсного виджета прозрачен, поэтому разрешаем его заполнить автоматически */
    ui->interfaceWidget->layout()->setMargin(0);           /* Задаём размер полей */
    ui->interfaceWidget->layout()->setSpacing(0);          /* Задаём размер пространства между элементами в размещении виджета */
    QGraphicsDropShadowEffect *shadowEffect =
            new QGraphicsDropShadowEffect(this);           /* Создаем графиеский эффект - тень */
    shadowEffect->setBlurRadius(9);                        /* Задаём радиус размытия тени */
    shadowEffect->setOffset(0);                            /* Задаём смещение тени */
    ui->interfaceWidget->setGraphicsEffect(shadowEffect);  /* Применяем эффект тени на окно */

    /* Подключение кнопок закрыть, свернуть, развернуть окно, так как стандартные скрыты */
    connect(ui->closeButton,    &QToolButton::clicked, this, &QWidget::close);
    connect(ui->minimizeButton, &QToolButton::clicked, this, &QWidget::showMinimized);
    connect(ui->maximazeButton, &QToolButton::clicked, [this](){
        // При нажатии на кнопку максимизации/нормализации окна
        // Делаем проверку на то, в каком состоянии находится окно и переключаем его режим
        if (!this->isMaximized())
        {
            //ui->maximazeButton->setStyleSheet(StyleHelper::getRestoreStyleSheet());

            /* Здесь важный нюанс: при разворачивании окна - необходимо поля centralwidget
             * (на уровень выше interfaceWidget) убрать, чтобы окно полноценно развернулось
             * в полный экран, однако, при этом исчезает тень, которую в полноэкранном режиме
             * и не должно быть видно, но при минимизации окна нужно вернуть */
            ui->centralwidget->layout()->setMargin(0);
            this->showMaximized();
        }
        else
        {
            // Заметьте, каждый раз устанавливаем новый стиль в эту кнопку
            //ui->maximazeButton->setStyleSheet(StyleHelper::getMaximizeStyleSheet());
            /* Здесь при минимизации возвращаем поля в исходный вид,
             * чтобы тень отобразилась */
            ui->centralwidget->layout()->setMargin(9);
            this->showNormal();
        }
    });


    /* Дополнительные функциональные кнопки */
    connect(ui->connectionButton, &QToolButton::clicked, settingsWindow, &SettingsDialog::show);


    // При запуске будем предлагать подключение
    //settingsWindow->show();
}

MainWindow::~MainWindow()
{
    delete console;
    delete serial;
    delete settingsWindow;
    delete ui;
}

