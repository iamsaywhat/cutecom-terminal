#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    /** Фиксируем указатели с формы**/
    Ports             = ui->COMports;                  // Указатель на ComboBox c доступными Com портами
    Baudrate          = ui->Baudrate_box;              // Указатель на ComboBox с настройками скорости
    Parity            = ui->Parity_box;                // Указатель на ComboBox с настройками паритета
    Databits          = ui->Data_box;                  // Указатель на ComboBox с настройками бит данных
    Stopbits          = ui->StopBits_box;              // Указатель на ComboBox с настройками стоп-бит
    Flowcontrol       = ui->FlowControl_box;           // Указатель на ComboBox с настройками контроля
    ConnectDisconnect = ui->ConnectDisconnect;         // Указатель на Кнопка подключения/отключения

    /** Оформление окна **/
    setWindowTitle("Setting");
    setFixedSize(203,238);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}
