#include "mainwindow.h"
#include "ui_mainwindow.h"




#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Создаём окно настроек COM
    settingsWindow = new SettingsDialog(this);

    // Выполняем связку класса работы с портом окном настроек
    serialsettings = new SerialSettings(settingsWindow->Ports,                // ComboBox c доступными Com портами
                                        settingsWindow->Baudrate,             // ComboBox с настройками скорости
                                        settingsWindow->Parity,               // ComboBox с настройками паритета
                                        settingsWindow->Databits,             // ComboBox с настройками бит данных
                                        settingsWindow->Stopbits,             // ComboBox с настройками стоп-бит
                                        settingsWindow->Flowcontrol,          // ComboBox с настройками контроля
                                        settingsWindow->ConnectDisconnect);   // Кнопка подключения/отключения

    // Выполняем связку класса работы с консолью с формами
    console = new ConsoleWindow(serialsettings, ui->workspace, ui->message, ui->SendButton, ui->ClearButton);

    // При запуске будем предлагать подключение
    settingsWindow->show();

    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->setRowCount(3);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setWordWrap(true);
    //ui->tableWidget->item(1,3)->setTextAlignment(Qt::TextWordWrap);
//    connect( this->ui->tableWidget, SIGNAL(cellChanged(int,int)),
//                this->ui->tableWidget, SLOT(resizeRowsToContents()) );
//       connect( this->ui->tableWidget->horizontalHeader(), SIGNAL(sectionResized(int,int,int)),
//                this->ui->tableWidget, SLOT(resizeRowsToContents()) );

}

MainWindow::~MainWindow()
{
    delete console;
    delete serialsettings;
    delete settingsWindow;
    delete ui;
}

void MainWindow::on_PortSettings_triggered()
{
    settingsWindow->show();
}
