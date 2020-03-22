#include "tableconsole.h"
#include <QHeaderView>
#include <QTime>
#include <QScrollBar>
#include "converter.h"

//#define DEBUG

#ifdef DEBUG
#include <QDebug>
#endif

TableConsole::TableConsole(QObject*           parent,
                           SerialGui*         serial,
                           QTableView*        table,
                           QLineEdit*         input,
                           QPushButton*       sendButton,
                           QPushButton*       clearButton) : QObject(parent)
{
    /* Берем указатели на элементы gui и рабочий serialport */
    _serial      = serial;           // Указатель на внешний экземпляр com-порта
    _table       = table;        // Указатель на QTableView форму GUI
    _input       = input;         // Указатель на QLineEdit форму GUI
    _sendButton  = sendButton;       // Указатель на QPushButton форму GUI
    _clearButton = clearButton;      // Указатель на QPushButton форму GUI
    /* Добавляем текст на кнопки */
    sendButton->setText(tr("Send"));
    clearButton->setText(tr("Clear"));
    // Включаем сетку на таблице
    _table->setShowGrid(false);
    /* Назначаем HEX валидатор на поле ввода */
    QRegularExpression hexMatcher("[0-9A-Fa-f ]+");
    _input->setValidator(new QRegularExpressionValidator(hexMatcher, this));
    /* Настройки QTableView */
    model = new QStandardItemModel(0,4);  // Создаём модель и пока только декларируем 4 столбца
    // Задаём заголовки столбцов
    horizontalHeaders << "#"
                      << "Timestamp"
                      << "Direction"
                      << "Message";
    model->setHorizontalHeaderLabels(horizontalHeaders);
    delegate = new TextEditDelegate(this);                 // Создаём делегата отвечающего отрисовку содержимого таблицы
    _table->setModel(model);                               // Помещаем модель в таблицу
    for(int i = 0; i < 4; i++){                             // Назначаем делегат
        _table->setItemDelegateForColumn(i, delegate);
    }
    _table->setEditTriggers(QAbstractItemView::NoEditTriggers);      // Действия инициирующие редактирование элемента запрещены
    _table->setSelectionMode(QAbstractItemView::SingleSelection);    // Выбирать можно только один элемент
    _table->setSelectionBehavior(QAbstractItemView::SelectRows);     // За один элемент выделения считается строка целиком
    _table->resizeColumnToContents(0);                               // Нулевой столбец будет изменён размером под своё содержимое
    _table->horizontalHeader()->setStretchLastSection(true);         // Слобцы будут занимать все место таблицы
    _table->verticalHeader()->hide();                                // Скрываем вертикальный хидер
    _table->setTextElideMode(Qt::ElideNone);                         // Настройки выравнивания
    _table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    /* Выполняем необходимые функциональные подключения слотов и сигналов */
    connect(_table->verticalScrollBar(), &QScrollBar::valueChanged,      // Изменение в скроллбаре генерирует сигнал на
            this, &TableConsole::slotAutoresize);                        // перерисовку видимой части таблицы
    connect(_table->horizontalHeader(),  &QHeaderView::sectionResized,   // Изменение геометрии хидера таблицы генерирует
            this, &TableConsole::slotAutoresize);                        // сигнал на перерисовку видимой части таблицы
    connect(clearButton, &QPushButton::clicked,                          // Клик по clear-кнопке будет инициировать стирание
            this, &TableConsole::clear);
    connect(sendButton, &QPushButton::clicked,                           // Клик по send-кнопке будет инициировать отправку
            this, &TableConsole::send);                                  // и отображение введенного сообщения
    connect(_input, &QLineEdit::textChanged,                             // Каждый введенный символ запускает автоустановщик
            this, &TableConsole::slotTextDelimiter);                     // разделителей между байтами
    connect(serial, &SerialGui::received,                                // QSerialPort будет уведомлять о принятых данных
            this, &TableConsole::receive);                               // и вызывать slot обработки входящих данных

    setEchoMode(true);
}
TableConsole::~TableConsole(){
    delete delegate;
    delete model;
}
void TableConsole::appendData(DirectionType direction, QString* data){
    QTime systemTime = QTime::currentTime();                                // Получаем системное время
    blockAutoresizeSlot();
    _table->verticalScrollBar()->blockSignals(true);
    /* Добавляем новую строку и заполняем ее содержимое */
    model->setRowCount(model->rowCount()+1);                                // Добавляем пустую строку данных
    model->setData(model->index(model->rowCount()-1, indexNumberColumn),    // Номер сообщения
                   model->rowCount()-1);
    model->setData(model->index(model->rowCount()-1, indexTimestampColumn), // Метка времени
                   systemTime.toString("hh:mm:ss:ms"));
    switch (direction)                                                      // Направление передачи
    {
    case INCOMING:
        model->setData(model->index(model->rowCount()-1, indexDirectionColumn),"Incoming");
        break;
    case OUTGOING:
        model->setData(model->index(model->rowCount()-1, indexDirectionColumn), "Outgoing");
        break;
    }
    model->setData(model->index(model->rowCount()-1, indexMessageColumn), *data); // Данные
    /* Настройки выравниваия для новой строки */
    model->setData(model->index(model->rowCount()-1,indexNumberColumn),    Qt::AlignTop, Qt::TextAlignmentRole);
    model->setData(model->index(model->rowCount()-1,indexTimestampColumn), Qt::AlignTop, Qt::TextAlignmentRole);
    model->setData(model->index(model->rowCount()-1,indexDirectionColumn), Qt::AlignTop, Qt::TextAlignmentRole);
    model->setData(model->index(model->rowCount()-1,indexMessageColumn),   Qt::AlignTop, Qt::TextAlignmentRole);
    _table->scrollToBottom();
    _table->resizeRowToContents(model->rowCount()-1);    
    /* Испускаем сигнал и сообщаем, что только что были добавлены новые данные */
    emit dataWasAppend();
    unblockAutoresizeSlot();
}
void TableConsole::appendData(DirectionType direction, QByteArray* data){
    QTime systemTime = QTime::currentTime();                                // Получаем системное время
    blockAutoresizeSlot();
    /* Добавляем новую строку и заполняем ее содержимое */
    model->setRowCount(model->rowCount()+1);                                // Добавляем пустую строку данных
    model->setData(model->index(model->rowCount()-1, indexNumberColumn),    // Номер сообщения
                   model->rowCount()-1);
    model->setData(model->index(model->rowCount()-1, indexTimestampColumn), // Метка времени
                   systemTime.toString("hh:mm:ss:ms"));
    switch (direction)                                                      // Направление передачи
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
    model->setData(model->index(model->rowCount()-1, indexMessageColumn),   // Данные
                   *data);
    /* Настройки выравниваия для новой строки */
    model->setData(model->index(model->rowCount()-1,indexNumberColumn),    Qt::AlignTop, Qt::TextAlignmentRole);
    model->setData(model->index(model->rowCount()-1,indexTimestampColumn), Qt::AlignTop, Qt::TextAlignmentRole);
    model->setData(model->index(model->rowCount()-1,indexDirectionColumn), Qt::AlignTop, Qt::TextAlignmentRole);
    model->setData(model->index(model->rowCount()-1,indexMessageColumn),   Qt::AlignTop, Qt::TextAlignmentRole);
    _table->scrollToBottom();
    _table->resizeRowToContents(model->rowCount()-1);
    /* Испускаем сигнал и сообщаем, что только что были добавлены новые данные */
    emit dataWasAppend();
    unblockAutoresizeSlot();
}
int TableConsole::firstVisibleRow (void){
    return _firstVisibleRow;
}
int TableConsole::lastVisibleRow (void){
    return _lastVisibleRow;
}
void TableConsole::updateVisibleRows (void){
    int viewportHeight = 0;             // Высота viewport
    int viewportWidth = 0;              // Ширина viewport
    int rowCount = 0;                   // Количество строк
    int rowPositionInViewport = 0;      // Положение во viewport

    /* Берем размеры viewport */
    viewportHeight = _table->viewport()->size().height();
    viewportWidth  = _table->viewport()->size().width();

    /* Берем номер строки в самом верху видимой части */
    _firstVisibleRow = _table->rowAt(0);
    _lastVisibleRow = _table->rowAt(0);

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
        rowPositionInViewport = _table->rowViewportPosition(i);
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
#ifdef DEBUG
    qDebug() << "firsrVisibleRow = " << firstVisibleRow()
             << "lastVisibleRow = " << lastVisibleRow();
#endif
}
void TableConsole::resizeVisibleRows (int firstRow, int lastRow){
    if(firstRow >= 0 && lastRow >= 0){
        for (int i = firstRow; i <= lastRow; i++)
            _table->resizeRowToContents(i);
    }
}
void TableConsole::slotAutoresize(void){
    if(autoresizeIsBlocked())
        return
    resizeVisibleRows(firstVisibleRow(), lastVisibleRow());
    updateVisibleRows();
    //resizeVisibleRows(firstVisibleRow(), lastVisibleRow());
#ifdef DEBUG
    qDebug() << "TableConsole::slotAutoresize";
#endif
}

void TableConsole::clear(void)
{
    /* Если мы удалим все содержимое, индексы отображаемой части
     * нужно сделать невалидными */
    _firstVisibleRow = -1;
    _lastVisibleRow = -1;    
    // Запоминаем пользовательный размер колонок
    QList<int> columnsWidth;
    for(int i = 0; i < 4; i++)
        columnsWidth << _table->columnWidth(i);
    // Удаляем данные с модели
    model->clear();
    // Восстанавливаем заголовки
    model->setHorizontalHeaderLabels(horizontalHeaders);
    // Восстанавливаем пользователькую ширину колонок
    for(int i = 0; i < 4; i++)
        _table->setColumnWidth(i, columnsWidth.at(i));
}

void TableConsole::send(void){
    /* Берём текст с поля ввода */
    QString message(_input->text());
    /* Если в поле ввода пусто, или порт не открыт, ничего не делаем */
    if(message == "" || _serial->getConnectionStatus() == SerialGui::CLOSED)
        return;
    QByteArray buffer = convertAsciiToHex(message);
    _serial->send(buffer);
    _input->clear();
    if (_echo)
        appendData(TableConsole::INCOMING, &message);
#ifdef DEBUG
    qDebug() << "TableConsole::send:" << message;
#endif
}

void TableConsole::receive(QByteArray data){
    QString message = convertHexToAscii(data);     // Принимаем данные и сразу преобразуем в строку
    appendData(TableConsole::OUTGOING, &message);  // Добавляем нлвые данные в таблицу
#ifdef DEBUG
    qDebug() << "TableConsole::receive" << message;
#endif
}

void TableConsole::slotTextDelimiter(void){
    QString source = _input->text();
    Converter::setDelimitersInHexString(source, 2, ' ');
    _input->setText(source);
}

QByteArray TableConsole::convertAsciiToHex(QString source){
    bool status;
    return Converter::parseStringForHex(&status, source, ' ');
}

QString TableConsole::convertHexToAscii(QByteArray source){
    QString result = "";
    for(int i = 0; i < source.count(); i++){
        result+=QString::number(static_cast<unsigned char>(source[i]),16).rightJustified(2, '0');
        if(i < source.count()-1)
            result.append(" ");
    }
    result = result.toUpper();
    return result;
}

bool TableConsole::echoMode(void){
    return _echo;
}
void TableConsole::setEchoMode(bool state){
    _echo = state;
    emit echoModeChanged(state);
}

void TableConsole::blockAutoresizeSlot(void){
    skipAutoresize = true;
    _table->horizontalHeader()->blockSignals(true);
    _table->verticalScrollBar()->blockSignals(true);
}

void TableConsole::unblockAutoresizeSlot(void){
    skipAutoresize = false;
    _table->horizontalHeader()->blockSignals(false);
    _table->verticalScrollBar()->blockSignals(false);
}

bool TableConsole::autoresizeIsBlocked(void){
    return skipAutoresize;
}
