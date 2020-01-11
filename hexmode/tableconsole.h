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

class TableConsole : public QObject
{
    Q_OBJECT

public:
    explicit TableConsole(QObject*     parent,
                          QTableView*  tableView,
                          QLineEdit*   lineEdit,
                          QPushButton* sendPushbutton,
                          QPushButton* clearPushbutton);
    ~TableConsole();

    /* Типы сообщения */
    typedef enum{
        INCOMING,   /* Исходящее */
        OUTGOING    /* Входящее */
    }DirectionType;

    /* Отобразить новую информациию */
    void appendData(DirectionType direction, QString* data);
    void appendData(DirectionType direction, QByteArray* data);

    /* Узнать номер верхней отображаемой строки */
    int firsrVisibleRow (void);
    /* Узнать номер нижней отображаемой строки */
    int lastVisibleRow (void);

private:
    QTableView*         table;       /* Таблица */
    TextEditDelegate*   delegate;    /* Делегат для особого отображения содержимого */
    QStandardItemModel* model;       /* Модель данных для таблицы */
    QLineEdit*          field;       /* Поле ввода исходящего сообщения */
    QPushButton*        sendbutton;  /* Кнопка отправки нового сообщения */
    QPushButton*        clearbutton; /* Кнопка очистки содержимого */

    /* Здесь храним индексы видимых строк таблицы */
    int _firstVisibleRow = 0; /* Индекс верхней отображаемой строки */
    int _lastVisibleRow = 0;  /* Индекс нижней отображаемой строки */

    /* Индексы столбцов */
    const uint8_t indexNumberColumn    = 0;
    const uint8_t indexTimestampColumn = 1;
    const uint8_t indexDirectionColumn = 2;
    const uint8_t indexMessageColumn   = 3;

    /* Обновить индексы видимой части таблицы */
    void updateVisibleRow ();
    /* Назначить авторесайз части таблицы между firstRow и lastRow строками */
    void autoResizeRowsRange (int firstRow, int lastRow);

signals:
    void dataWasAppend(void);

private slots:
    void tableResizeSlot(void);

public slots:
    void clearAll(void);
    void sendMessage (void);
};

#endif // TABLECONSOLE_H
