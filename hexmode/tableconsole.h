#ifndef TABLECONSOLE_H
#define TABLECONSOLE_H

#include <QObject>
#include <QTableView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <QByteArray>

#include "delegate.h"
#include "serialgui.h"

class TableConsole : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool echoMode READ echoMode WRITE setEchoMode NOTIFY echoModeChanged)

public:
    explicit TableConsole(QObject*           parent,
                          SerialGui*         serial,
                          QTableView*        table,
                          QLineEdit*         input,
                          QPushButton*       sendButton,
                          QPushButton*       clearButton);
    ~TableConsole();
    bool echoMode (void);

    /* Типы сообщения */
    enum DirectionType{
        INCOMING,   // Исходящее
        OUTGOING    // Входящее
    };
    /* Отобразить новую информациию */
    void appendData(DirectionType direction, QString* data);
    void appendData(DirectionType direction, QByteArray* data);

    /* Узнать номер верхней отображаемой строки */
    int firstVisibleRow (void);
    /* Узнать номер нижней отображаемой строки */
    int lastVisibleRow (void);
    /* Преобразование строки с HEX кодами разделенными пробелами
     *  в байтовый массив */
    QByteArray convertAsciiToHex(QString source);
    /* Преобразование байтового массива в строку с HEXкодами разделенными пробелами */
    QString convertHexToAscii(QByteArray source);

private:
    TextEditDelegate*   delegate;     // Делегат для особого отображения содержимого
    QStandardItemModel* model;        // Модель данных для таблицы
    SerialGui*          _serial;      // Com-порт
    QTableView*         _table;       // Таблица
    QLineEdit*          _input;       // Поле ввода исходящего сообщения
    QPushButton*        _sendButton;  // Кнопка отправки нового сообщения
    QPushButton*        _clearButton; // Кнопка очистки содержимого
    bool                _echo;
    bool                skipAutoresize = false;
    QStringList         horizontalHeaders;

    /* Здесь храним индексы видимых строк таблицы */
    int _firstVisibleRow = 0;  // Индекс верхней отображаемой строки
    int _lastVisibleRow = 0;   // Индекс нижней отображаемой строки

    /* Индексы столбцов */
    const uint8_t indexNumberColumn    = 0;
    const uint8_t indexTimestampColumn = 1;
    const uint8_t indexDirectionColumn = 2;
    const uint8_t indexMessageColumn   = 3;

    /* Обновить индексы видимой части таблицы */
    void updateVisibleRows ();
    /* Назначить авторесайз части таблицы между firstRow и lastRow строками */
    void resizeVisibleRows (int firstRow, int lastRow);

    void blockAutoresizeSlot();
    void unblockAutoresizeSlot();
    bool autoresizeIsBlocked();

signals:
    void dataWasAppend(void);
    void echoModeChanged(bool);

private slots:
    void slotAutoresize(void);
    void slotTextDelimiter(void);

public slots:
    void clear(void);
    void send(void);
    void receive(QByteArray);
    void setEchoMode(bool);
};

#endif // TABLECONSOLE_H
