#include "tableconsole.h"
#include <QHeaderView>
#include <QTime>
#include <QDebug>
#include <QScrollBar>

TableConsole::TableConsole(QObject*           parent,
                           SerialGui*         serial,
                           QTableView*        tableView,
                           QLineEdit*         lineEdit,
                           QPushButton*       sendButton,
                           QPushButton*       clearButton)  : QObject(parent)
{
    /* Берем указатели на элементы gui и рабочий serialport */
    TableConsole::serial      = serial;           /* Указатель на внешний экземпляр com-порта */
    TableConsole::table       = tableView;        /* Указатель на QTableView форму GUI */
    TableConsole::field       = lineEdit;         /* Указатель на QLineEdit форму GUI */
    TableConsole::sendButton  = sendButton;       /* Указатель на QPushButton форму GUI */
    TableConsole::clearButton = clearButton;      /* Указатель на QPushButton форму GUI */

    /* Добавляем текст на кнопки */
    sendButton->setText("Send");
    clearButton->setText("Clear");

    /* Назначаем HEX валидатор на поле ввода */
    QRegularExpression hexMatcher("[0-9A-Fa-f ]+");
    field->setValidator(new QRegularExpressionValidator(hexMatcher, this));

    /* Настройки QTableView */
    model = new QStandardItemModel(0,4);                            /* Создаём модель и пока только декларируем 4 столбца */
    QStringList horizontalHeaders;                                  /* Задаём заголовки столбцов */
    horizontalHeaders << "#"
                      << "Timestamp"
                      << "Direction"
                      << "Message";
    model->setHorizontalHeaderLabels(horizontalHeaders);
    delegate = new TextEditDelegate(this);                          /* Создаём делегата отвечающего отрисовку содержимого таблицы */
    table->setModel(model);                                         /* Помещаем модель в таблицу */
    for(int i = 0; i < 4; i++)                                      /* Назначаем делегат */
    {
        table->setItemDelegateForColumn(i, delegate);
    }
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);      /* Действия инициирующие редактирование элемента запрещены */
    table->setSelectionMode(QAbstractItemView::SingleSelection);    /* Выбирать можно только один элемент */
    table->setSelectionBehavior(QAbstractItemView::SelectRows);     /* За один элемент выделения считается строка целиком */
    table->resizeColumnToContents(0);                               /* Нулевой столбец будет изменён размером под своё содержимое */
    table->horizontalHeader()->setStretchLastSection(true);         /* Слобцы будут занимать все место таблицы */
    table->verticalHeader()->hide();                                /* Скрываем вертикальный хидер */
    table->setTextElideMode(Qt::ElideNone);                         /* Настройки выравнивания */
    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    /* Выполняем необходимые функциональные подключения слотов и сигналов */
    connect(table->verticalScrollBar(), &QScrollBar::valueChanged,      /* Изменение в скроллбаре генерирует сигнал на */
            this, &TableConsole::slotAutoresize);                       /* перерисовку видимой части таблицы */
    connect(table->horizontalHeader(),  &QHeaderView::sectionResized,   /* Изменение геометрии хидера таблицы генерирует */
            this, &TableConsole::slotAutoresize);                       /* сигнал на перерисовку видимой части таблицы */
    connect(clearButton, &QPushButton::clicked,                         /* Клик по clear-кнопке будет инициировать стирание */
            this, &TableConsole::clear);
    connect(sendButton, &QPushButton::clicked,                          /* Клик по send-кнопке будет инициировать отправку */
            this, &TableConsole::send);                                 /* и отображение введенного сообщения */
    connect(field, &QLineEdit::textChanged,                             /* Каждый введенный символ запускает автоустановщик */
            this, &TableConsole::slotTextDelimiter);                    /* разделителей между байтами */
    connect(serial, &SerialGui::receivedNewData,                            /* QSerialPort будет уведомлять о принятых данных */
            this, &TableConsole::receive);                              /* и вызывать slot обработки входящих данных */
}


TableConsole::~TableConsole()
{
    delete delegate;
    delete model;
}


void TableConsole::appendData(DirectionType direction, QString* data)
{
    QTime systemTime = QTime::currentTime();                                /* Получаем системное время */

    /* Добавляем новую строку и заполняем ее содержимое */
    model->setRowCount(model->rowCount()+1);                                /* Добавляем пустую строку данных */
    model->setData(model->index(model->rowCount()-1, indexNumberColumn),    /* Номер сообщения */
                   model->rowCount()-1);
    model->setData(model->index(model->rowCount()-1, indexTimestampColumn), /* Метка времени */
                   systemTime.toString("hh:mm:ss:ms"));
    switch (direction)                                                      /* Направление передачи */
    {
        case INCOMING:
            model->setData(model->index(model->rowCount()-1, indexDirectionColumn),
                           "Incoming");
            break;
        case OUTGOING:
            model->setData(model->index(model->rowCount()-1, indexDirectionColumn),
                           "Outgoing");
            break;
    }
    model->setData(model->index(model->rowCount()-1, indexMessageColumn),   /* Сообщение */
                   *data);
    /* Настройки выравниваия для новой строки */
    model->setData(model->index(model->rowCount()-1,indexNumberColumn),    Qt::AlignTop, Qt::TextAlignmentRole);
    model->setData(model->index(model->rowCount()-1,indexTimestampColumn), Qt::AlignTop, Qt::TextAlignmentRole);
    model->setData(model->index(model->rowCount()-1,indexDirectionColumn), Qt::AlignTop, Qt::TextAlignmentRole);
    model->setData(model->index(model->rowCount()-1,indexMessageColumn),   Qt::AlignTop, Qt::TextAlignmentRole);


    slotAutoresize();
    table->scrollToBottom();
    /* Испускаем сигнал и сообщаем, что только что были добавлены новые данные */
    emit dataWasAppend();
}


void TableConsole::appendData(DirectionType direction, QByteArray* data)
{
    QTime systemTime = QTime::currentTime();                                /* Получаем системное время */

    /* Добавляем новую строку и заполняем ее содержимое */
    model->setRowCount(model->rowCount()+1);                                /* Добавляем пустую строку данных */
    model->setData(model->index(model->rowCount()-1, indexNumberColumn),    /* Номер сообщения */
                   model->rowCount()-1);
    model->setData(model->index(model->rowCount()-1, indexTimestampColumn), /* Метка времени */
                   systemTime.toString("hh:mm:ss:ms"));
    switch (direction)                                                      /* Направление передачи */
    {
        case INCOMING:
            model->setData(model->index(model->rowCount()-1, indexDirectionColumn),
                           "Incoming");
            break;
        case OUTGOING:
            model->setData(model->index(model->rowCount()-1, indexDirectionColumn),
                           "Outgoing");
            break;
    }
    model->setData(model->index(model->rowCount()-1, indexMessageColumn),   /* Сообщение */
                   *data);
    /* Настройки выравниваия для новой строки */
    model->setData(model->index(model->rowCount()-1,indexNumberColumn),    Qt::AlignTop, Qt::TextAlignmentRole);
    model->setData(model->index(model->rowCount()-1,indexTimestampColumn), Qt::AlignTop, Qt::TextAlignmentRole);
    model->setData(model->index(model->rowCount()-1,indexDirectionColumn), Qt::AlignTop, Qt::TextAlignmentRole);
    model->setData(model->index(model->rowCount()-1,indexMessageColumn),   Qt::AlignTop, Qt::TextAlignmentRole);

    slotAutoresize();
    table->scrollToBottom();
    /* Испускаем сигнал и сообщаем, что только что были добавлены новые данные */
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



void TableConsole::updateVisibleRows (void)
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
    rowCount = model->rowCount();
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
            _lastVisibleRow = i;
    }
    qDebug() << "firsrVisibleRow" << firsrVisibleRow() << "lastVisibleRow" << lastVisibleRow();
}



void TableConsole::enableAutoresizeVisibleRows (int firstRow, int lastRow)
{
    static int _firstRow = -1;     /* Предыдущее положение видимой части: начало */
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

void TableConsole::slotAutoresize(void)
{
    /* Сначала узнаем видимую часть */
    updateVisibleRows();
    /* Видимой части разрешаем авторесайз */
    enableAutoresizeVisibleRows(_firstVisibleRow, _lastVisibleRow);
}

void TableConsole::clear(void)
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
    enableAutoresizeVisibleRows(_firstVisibleRow, _lastVisibleRow);
    /* Теперь можем удалить все существующие строки */
    model->removeRows(0, model->rowCount());
}


void TableConsole::send(void)
{
    /* Берём текст с поля ввода */
    QString message(field->text());
    /* Если в поле ввода пусто, или порт не открыт, ничего не делаем */
    if(message == "" || serial->getConnectionStatus() == CLOSED)
        return;

    QByteArray buffer = convertAsciiToHex(message);
    qDebug() << "Send: " << message;
    serial->write(buffer);
    serial->waitForBytesWritten(100);
    field->clear();
    appendData(TableConsole::INCOMING, &message);
}

void TableConsole::receive(void)
{
    /* Принимаем данные и сразу преобразуем в строку */
    QString message = convertHexToAscii(serial->getData());
    /* Добавляем нлвые данные в таблицу */
    appendData(TableConsole::OUTGOING, &message);
    qDebug() << message;
}

void TableConsole::slotTextDelimiter(void)
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
    QString    clearSource;
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


QString TableConsole::convertHexToAscii(QByteArray source)
{
    QString result = "";
    for(int i = 0; i < source.count(); i++)
    {
        result+=QString::number(static_cast<unsigned char>(source[i]),16).rightJustified(2, '0');
        if(i < source.count()-1)
            result.append(" ");
    }
    result = result.toUpper();
    return result;
}
