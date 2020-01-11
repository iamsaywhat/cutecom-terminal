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

    /*  */
    connect(field, &QLineEdit::textChanged, this, &TableConsole::textCheck);

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


void TableConsole::sendMessage (void)
{
    /* Берём текст с поля ввода */
    QString message(field->text());
    QByteArray buffer;
    buffer = convertAsciiToHex(message);
}

void TableConsole::textCheck(void)
{
    QString result;
    /* Берём текст с поля ввода */
    QString message(field->text());
    /* Будем перебирать по байту и группировать символы
     * по два разделяя их пробелом в новой строке и
     * пропуская лишние пробелы. */
    for(int count = 0; count < message.count(); count++)
    {
        if(count%3 != 2 && message[count] != ' ')
            result.append(message[count]);
        else if (count%3 == 2)
        {
            if(message[count] == ' ')
                result.append(message[count]);
            else
                result.append(" " + message[count]);
        }
    }
    /* В поле просто выбрасываем уже сгруппированную строку */
    field->setText(result);
}

QByteArray TableConsole::convertAsciiToHex(QString source)
{
    QString clearSource;
    QByteArray result;

    /* Удалим все разделители и дополним */
    for(int i = 0; i < source.count(); i++)
    {
        if(source[i] != ' ')
            clearSource.append(source[i]);
    }
    if(clearSource.count()%2 != 0)
        clearSource.insert(clearSource.count()-1, '0');

    /* Будем брать по два байта из очищенной строки и преобразовывать */
    for(int i = 0, j = 0; i < clearSource.count(); i+=2, j++)
    {
        QString temp(clearSource[i]);
        temp.append(clearSource[i+1]);
        result.insert(j, static_cast<char>(temp.toInt(nullptr, 16)));
    }

    return result;
}
