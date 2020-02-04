#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "decorator.h"
#include <QLineEdit>

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

    //setStyleSheet(Decorator::getMainWidgetStyleSheet());
    Ports->setStyleSheet(Decorator::getComboBoxStyleSheet());
    Baudrate->setStyleSheet(Decorator::getComboBoxStyleSheet());
//    Parity->setStyleSheet(Decorator::getComboBoxStyleSheet());
//    Databits->setStyleSheet(Decorator::getComboBoxStyleSheet());
//    Stopbits->setStyleSheet(Decorator::getComboBoxStyleSheet());
//    Flowcontrol->setStyleSheet(Decorator::getComboBoxStyleSheet());
    //Baudrate->findChild <QLineEdit*>()->setStyleSheet ("QLineEdit {background: green;}");
    //Baudrate->lineEdit()->setStyleSheet("QLineEdit {background: green;}");
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}
