#ifndef TABLE_H
#define TABLE_H

#include <QObject>
#include <QTableView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QToolButton>
#include <QLineEdit>
#include <QString>
#include <QByteArray>

#include "table-delegate.h"
#include "serialgui.h"

class Table : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool echoMode READ echoMode WRITE setEchoMode NOTIFY echoModeChanged)
    Q_PROPERTY(bool cyclicMode READ cyclicMode WRITE setCyclicMode NOTIFY cyclicModeChanged)
    Q_PROPERTY(int cyclicInterval READ cyclicInterval WRITE setCyclicInterval NOTIFY cyclicIntervalChanged)
    Q_PROPERTY(QString bindData READ bindData WRITE setBindData NOTIFY bindDataChanged)

public:
    explicit Table(QObject*           parent,
                   SerialGui*         serial,
                   QTableView*        table,
                   QLineEdit*         input,
                   QPushButton*       sendButton,
                   QPushButton*       clearButton);
    ~Table();
    bool echoMode(void);
    bool cyclicMode(void);
    int cyclicInterval(void);
    QString& bindData(void);
    void retranslate(void);

    /* Типы сообщения */
    enum DirectionType{
        INCOMING,   // Исходящее
        OUTGOING    // Входящее
    };
    /* Отобразить новую информациию */
    void appendData(DirectionType direction, QString& hex, QString& ascii);

    /* Узнать номер верхней отображаемой строки */
    int firstVisibleRow (void);
    /* Узнать номер нижней отображаемой строки */
    int lastVisibleRow (void);

private:
    QRegularExpressionValidator  *hexMatcher;
    TableDelegate*      delegate;     // Делегат для особого отображения содержимого
    QStandardItemModel* model;        // Модель данных для таблицы
    SerialGui*          _serial;      // Com-порт
    QTableView*         _table;       // Таблица
    QLineEdit*          _input;       // Поле ввода исходящего сообщения
    QPushButton*        _sendButton;  // Кнопка отправки нового сообщения
    QPushButton*        _clearButton; // Кнопка очистки содержимого
    bool                skipAutoresize = false;
    QStringList         horizontalHeaders;
    bool                _echo = false;
    bool                _cyclic = false;
    int                 _cyclicInterval = 1000;
    QString             _bindData;
    QTimer              *timer;

    /* Здесь храним индексы видимых строк таблицы */
    int _firstVisibleRow = 0;  // Индекс верхней отображаемой строки
    int _lastVisibleRow = 0;   // Индекс нижней отображаемой строки

    /* Индексы столбцов */
    const uint8_t indexNumberColumn    = 0;
    const uint8_t indexTimestampColumn = 1;
    const uint8_t indexDirectionColumn = 2;
    const uint8_t indexHexColumn       = 3;
    const uint8_t indexAsciiColumn     = 4;

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
    void cyclicModeChanged(bool);
    void cyclicIntervalChanged(int);
    void bindDataChanged(QString&);
    void cyclicIsRunning(void);
    void cyclicStopped(void);

private slots:
    void slotAutoresize(void);
    void slotTextDelimiter(void);
    void cyclicTimeout(void);

public slots:
    void clear(void);
    void send(void);
    void send(QString);
    void sended(QByteArray);
    void received(QByteArray);
    void setEchoMode(bool);
    void setCyclicMode(bool);
    void setCyclicInterval(int);
    void setBindData(QString);
    void startCyclicSending(void);
    void stopCyclicSending(void);
};

#endif // TABLE_H
