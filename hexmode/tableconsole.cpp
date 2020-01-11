#include "tableconsole.h"
#include <QHeaderView>
#include <QTime>
#include <QDebug>
#include <QScrollBar>

TableConsole::TableConsole(QObject *parent,
                           QTableView*  tableView,
                           QLineEdit*   lineEdit,
                           QPushButton* sendPushbutton,
                           QPushButton* clearPushbutton)  : QObject(parent)
{
    /* Берем указатели на элементы gui */
    table       = tableView;
    field       = lineEdit;
    sendbutton  = sendPushbutton;
    clearbutton = clearPushbutton;

    /* Настройка элементов gui */
    sendbutton->setText("Send");
    clearbutton->setText("Clear");

    /* Создаём модель и пока только декларируем 4 столбца */
    model = new QStandardItemModel(0,4);

    /* Задаём заголовки столбцов */
    QStringList horizontalHeaders;
    horizontalHeaders << "#"
                      << "Timestamp"
                      << "Direction"
                      << "Message";
    model->setHorizontalHeaderLabels(horizontalHeaders);

    /* Создаём делегата отвечающего отрисовку содержимого таблицы */
    delegate = new TextEditDelegate(this);

    /* Помещаем модель в таблицу */
    table->setModel(model);

    /* Назначаем делегат */
    for(int i = 0; i < 4; i++)
    {
        table->setItemDelegateForColumn(i, delegate);
    }

    /* Действия инициирующие редактирование элемента запрещены */
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    /* Выбирать можно только один элемент */
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    /* За один элемент выделения считается строка целиком */
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    /* Нулевой столбец будет изменён размером под своё содержимое */
    table->resizeColumnToContents(0);
    /* Слобцы будут занимать все место таблицы */
    table->horizontalHeader()->setStretchLastSection(true);
    /* Скрываем вертикальный хидер */
    table->verticalHeader()->hide();
    /* Настройки выравнивания */
    table->setTextElideMode(Qt::ElideNone);
    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    /* Изменение в скроллбаре генерирует сигнал на перерисовку видимой части таблицы */
    connect(table->verticalScrollBar(), &QScrollBar::valueChanged, this, &TableConsole::tableResizeSlot);
    /* Изменение геометрии хидера таблицы генерирует сигнал на перерисовку видимой части таблицы */
    connect(table->horizontalHeader(), &QHeaderView::sectionResized, this, &TableConsole::tableResizeSlot);

    /* Клик по clear-кнопке будет инициировать стирание */
    connect(clearPushbutton, &QPushButton::clicked, this, &TableConsole::clearAll);

    /* Клик по send-кнопке будет инициировать отправку и отображение введенного сообщения */
    connect(sendPushbutton, &QPushButton::clicked, this, &TableConsole::sendMessage);

    QRegExp hexMatcher("[0-9A-Fa-f ]+");
    field->setValidator(new QRegExpValidator(hexMatcher, this));
}

TableConsole::~TableConsole()
{
    delete delegate;
    delete model;
}

void TableConsole::appendData(DirectionType direction, QString* data)
{
    QTime systemTime = QTime::currentTime();

    /* Добавляем строку данных */
    model->setRowCount(model->rowCount()+1);
    //model->insertRow(model->rowCount());

    /* Номер сообщения */
    model->setData(model->index(model->rowCount()-1, indexNumberColumn), model->rowCount()-1);
    /* Метка времени */
    model->setData(model->index(model->rowCount()-1, indexTimestampColumn), systemTime.toString("hh:mm:ss:ms"));
    /* Направление передачи */
    switch (direction)
    {
        case INCOMING:
            model->setData(model->index(model->rowCount()-1, indexDirectionColumn), "Incoming");
            break;
        case OUTGOING:
            model->setData(model->index(model->rowCount()-1, indexDirectionColumn), "Outgoing");
            break;
    }
    /* Сообщение */
    model->setData(model->index(model->rowCount()-1, indexMessageColumn), *data);
    /* Настройки выравниваия */
    model->setData(model->index(model->rowCount()-1,indexNumberColumn),    Qt::AlignTop, Qt::TextAlignmentRole);
    model->setData(model->index(model->rowCount()-1,indexTimestampColumn), Qt::AlignTop, Qt::TextAlignmentRole);
    model->setData(model->index(model->rowCount()-1,indexDirectionColumn), Qt::AlignTop, Qt::TextAlignmentRole);
    model->setData(model->index(model->rowCount()-1,indexMessageColumn),   Qt::AlignTop, Qt::TextAlignmentRole);

    emit dataWasAppend();
}

void TableConsole::appendData(DirectionType direction, QByteArray* data)
{
    QTime systemTime = QTime::currentTime();

    /* Добавляем строку данных */
    model->setRowCount(model->rowCount()+1);
    //model->insertRow(model->rowCount());

    /* Номер сообщения */
    model->setData(model->index(model->rowCount()-1, indexNumberColumn), model->rowCount()-1);
    /* Метка времени */
    model->setData(model->index(model->rowCount()-1, indexTimestampColumn), systemTime.toString("hh:mm:ss:ms"));
    /* Направление передачи */
    switch (direction)
    {
        case INCOMING:
            model->setData(model->index(model->rowCount()-1, indexDirectionColumn), "Incoming");
            break;
        case OUTGOING:
            model->setData(model->index(model->rowCount()-1, indexDirectionColumn), "Outgoing");
            break;
    }
    /* Сообщение */
    model->setData(model->index(model->rowCount()-1, indexMessageColumn), *data);
    /* Настройки выравниваия */
    model->setData(model->index(model->rowCount()-1,indexNumberColumn),    Qt::AlignTop, Qt::TextAlignmentRole);
    model->setData(model->index(model->rowCount()-1,indexTimestampColumn), Qt::AlignTop, Qt::TextAlignmentRole);
    model->setData(model->index(model->rowCount()-1,indexDirectionColumn), Qt::AlignTop, Qt::TextAlignmentRole);
    model->setData(model->index(model->rowCount()-1,indexMessageColumn),   Qt::AlignTop, Qt::TextAlignmentRole);

    emit dataWasAppend();
}

int TableConsole::firsrVisibleRow (void)
{
    return _firstVisibleRow;
}

int TableConsole::lastVisibleRow (void)
{
    return _lastVisibleRow;
}

void TableConsole::updateVisibleRow (void)
{
    int viewportHeight = 0;             /* Высота viewport */
    int viewportWidth = 0;              /* Ширина viewport */
    int rowCount = 0;                   /* Количество строк */
    int rowPositionInViewport = 0;      /* Положение во viewport */

    /* Берем размеры viewport */
    viewportHeight = table->viewport()->size().height();
    viewportWidth  = table->viewport()->size().width();

    /* Берем номер строки в самом верху видимой части */
    _firstVisibleRow = table->rowAt(0);
    _lastVisibleRow = table->rowAt(0);

    /* Берем общее число строк */
    rowCount = model->rowCount()-1;
    /* Начинаем искать последнюю отображаемую строку
     * От первой видимой будем последовательно вниз
     * (в сторону увеличения номера) перебирать
     * строки, получать ее положение относительно
     * Viewport, и искать первую выходящую отображаемую
     * часть строку */
    for(int i = _firstVisibleRow+1 ; i < rowCount; i++)
    {
        /* Берем положение строки относительно Viewport */
        rowPositionInViewport = table->rowViewportPosition(i);
        /* Признаком являются следующие факты:
         * 1) Если положение отрительно - значит строка находится
         * выше отображаемой части;
         * 2) Если положение больше, чем высота Viewport, значит
         * строка находится ниже viewport */
        if (rowPositionInViewport > viewportHeight || rowPositionInViewport < 0)
        {
            /* Если выпали за viewport, значит предыдущая
             * строка последняя отображаемая */
            _lastVisibleRow = i-1;
            break;
        }
        /* Граничное условие: если i здесь равен rowCount - 1
         * значит мы беребрали все строки, но так и не нашли
         * последнюю отображаемую строку, а значит снизу мы
         * видим всю таблицу
         * */
        else if (i == rowCount - 1)
            _lastVisibleRow = rowCount;
    }
    qDebug() << "firsrVisibleRow" << firsrVisibleRow() << "lastVisibleRow" << lastVisibleRow();
}

void TableConsole::autoResizeRowsRange (int firstRow, int lastRow)
{
    static int _firstRow = -1;    /* Предыдущее положение видимой части: начало */
    static int _lastRow = -1;      /* Предыдущее положение видимой части: конец */
    int i;

    /* Если строк нет, то ничего делать и не надо */
    if(model->rowCount() == 0)
        return;

    /* Выполняем только, если если изменения в отображаемой части  */
    if(firstRow!=_firstRow || lastRow!=_lastRow)
    {
        if(_firstRow>=0 && _lastRow>=0)
        {
            /* Деактивируем авторесайз в предыдущем положении */
            for (i = _firstRow; i <= _lastRow; i++)
            {
                table->verticalHeader()->setSectionResizeMode(i,QHeaderView::Fixed);
            }
        }
        if(firstRow>=0 && lastRow>=0)
        {
            /* Активируем авторесайз текущей отображаемой части */
            for (i = firstRow; i <= lastRow; i++)
            {
                table->verticalHeader()->setSectionResizeMode(i,QHeaderView::ResizeToContents);
            }
        }
        /* Запоминаем положение отображаемой части */
        _firstRow = firstRow;
        _lastRow = lastRow;
    }
}

void TableConsole::tableResizeSlot(void)
{
    /* Сначала узнаем видимую часть */
    updateVisibleRow();
    /* Видимой части разрешаем авторесайз */
    autoResizeRowsRange(_firstVisibleRow, _lastVisibleRow);
}

void TableConsole::clearAll(void)
{
    /* Если мы удалим все содержимое, индексы отображаемой части
     * нужно сделать невалидными */
    _firstVisibleRow = -1;
    _lastVisibleRow = -1;
    /* Выполнив с невалидными индексами, мы заберем свойство
     * autoresize у ячеек отображаемых в данный момент и заодно
     * сбросим внутри static-переменные, иначе после удаления
     * функция попробует отключить свойство у уже несуществующих
     * строк и мы поймаем вылет */
    autoResizeRowsRange(_firstVisibleRow, _lastVisibleRow);
    /* Теперь можем удалить все существующие строки */
    model->removeRows(0, model->rowCount());
}

QByteArray convertAscii(QString source);
void TableConsole::sendMessage (void)
{
//    /* Берём текст с поля ввода */
//    QString message(field->text());

//    /* Если в поле ввода пусто, или порт не открыт, ничего не делаем */
//    if(message == "" || !Serial->getStatus())
//        return;

//    Serial->write(message.toLocal8Bit());
//    Serial->waitForBytesWritten(100);
//    field->clear();

//    appendData(TableConsole::INCOMING, &message);

//    QString test = "000102030405060708090A0BFF HH";
//    QByteArray test2 = QByteArray::fromHex(test.toLatin1());
//    qDebug() << test.toLatin1() << test2;

//    Serial->write(test2);



    /* Берём текст с поля ввода */
    QString message(field->text());
    qDebug()<< "message len = " << message.count();
    QString temp;
    for(uint i = 0; i < sizeof (message)-1; i+=2)
    {
        temp = message[i];
        temp.append(message[i+1]);
        //temp = temp.to
        //qDebug() << temp.toInt();
    }
    QByteArray s;
    s = convertAscii(message);
    qDebug() << "s=" << s;

    int num = message.toInt(nullptr,16);
//    qDebug() << "num=" << num;
//    qDebug() << "message" << message;
//    qDebug() << message[0].toLatin1();
}



QByteArray convertAscii(QString source)
{
    QString temp;
    QByteArray result;

    int j = 0;

    /* Будем перебирать байт за байтом исходной строки */
    for(int i = 0; i < source.count(); i++)
    {
        /* Если встретили пробел, то пропускаем текущий байт */
        if(source[i] == ' ')
            continue;
        /* Берем */
        temp = source[i];
        temp.append(source[i+1]);
        i++;
        result.insert(j, static_cast<char>(temp.toInt(nullptr, 16)));
        j++;
    }

    return result;
}
