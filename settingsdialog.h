#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    /**
     * Размещаю указатели на виджеты формы данного
     * окна в публичной части, чтобы предоставить
     * доступ извне
     **/
    QComboBox*   Ports;                 // Указатель на ComboBox c доступными Com портами
    QComboBox*   Baudrate;              // Указатель на ComboBox с настройками скорости
    QComboBox*   Parity;                // Указатель на ComboBox с настройками паритета
    QComboBox*   Databits;              // Указатель на ComboBox с настройками бит данных
    QComboBox*   Stopbits;              // Указатель на ComboBox с настройками стоп-бит
    QComboBox*   Flowcontrol;           // Указатель на ComboBox с настройками контроля
    QPushButton* ConnectDisconnect;     // Указатель на Кнопку подключения/отключения

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
