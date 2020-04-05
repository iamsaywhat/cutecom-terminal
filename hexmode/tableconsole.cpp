#include "tableconsole.h"
#include <QHeaderView>
#include <QTime>
#include <QScrollBar>
#include "converter.h"


#include <QDebug>


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
    hexMatcher = new QRegularExpressionValidator(QRegularExpression("[0-9A-Fa-f ]+"), this);
    _input->setValidator(hexMatcher);
    /* Настройки QTableView */
    model = new QStandardItemModel(0,5);  // Создаём модель и пока только декларируем 4 столбца
    // Задаём заголовки столбцов
    horizontalHeaders << "#"
                      << tr("Timestamp")
                      << tr("Direction")
                      << tr("Hex")
                      << tr("Ascii");
    model->setHorizontalHeaderLabels(horizontalHeaders);
    delegate = new TextEditDelegate(this);                 // Создаём делегата отвечающего отрисовку содержимого таблицы
    _table->setModel(model);                               // Помещаем модель в таблицу
    for(int i = 4; i < 5; i++){                            // Назначаем делегат
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
            this, QOverload<>::of(&TableConsole::send));                 // и отображение введенного сообщения
    connect(_input, &QLineEdit::textChanged,                             // Каждый введенный символ запускает автоустановщик
            this, &TableConsole::slotTextDelimiter);                     // разделителей между байтами
    connect(serial, &SerialGui::received,                                // QSerialPort будет уведомлять о принятых данных
            this, &TableConsole::received);                              // и вызывать slot обработки входящих данных

    timer = new QTimer;
    connect(timer, &QTimer::timeout,
            this, &TableConsole::cyclicTimeout);
}
TableConsole::~TableConsole(){
    delete timer;
    delete delegate;
    delete model;
    delete hexMatcher;
}
void TableConsole::appendData(DirectionType direction, QString& hex, QString& ascii){
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
        model->setData(model->index(model->rowCount()-1, indexDirectionColumn),tr("Incoming"));
        break;
    case OUTGOING:
        model->setData(model->index(model->rowCount()-1, indexDirectionColumn), tr("Outgoing"));
        break;
    }
    model->setData(model->index(model->rowCount()-1, indexHexColumn), hex);     // Данные
    model->setData(model->index(model->rowCount()-1, indexAsciiColumn), ascii); // Данные
    /* Настройки выравниваия для новой строки */
    model->setData(model->index(model->rowCount()-1,indexNumberColumn),    Qt::AlignTop, Qt::TextAlignmentRole);
    model->setData(model->index(model->rowCount()-1,indexTimestampColumn), Qt::AlignTop, Qt::TextAlignmentRole);
    model->setData(model->index(model->rowCount()-1,indexDirectionColumn), Qt::AlignTop, Qt::TextAlignmentRole);
    model->setData(model->index(model->rowCount()-1,indexHexColumn),       Qt::AlignTop, Qt::TextAlignmentRole);
    model->setData(model->index(model->rowCount()-1,indexAsciiColumn),     Qt::AlignTop, Qt::TextAlignmentRole);
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
    qDebug() << "firsrVisibleRow = " << firstVisibleRow()
             << "lastVisibleRow = " << lastVisibleRow();
}
void TableConsole::resizeVisibleRows (int firstRow, int lastRow){
    if(firstRow >= 0 && lastRow >= 0){
        for (int i = firstRow; i <= lastRow; i++)
            _table->resizeRowToContents(i);
    }
}
void TableConsole::slotAutoresize(void){
    if(autoresizeIsBlocked())
        return;
    resizeVisibleRows(firstVisibleRow(), lastVisibleRow());
    updateVisibleRows();
    //resizeVisibleRows(firstVisibleRow(), lastVisibleRow());
    qDebug() << "TableConsole::slotAutoresize";
}

void TableConsole::clear(void){
    /* Если мы удалим все содержимое, индексы отображаемой части
     * нужно сделать невалидными */
    _firstVisibleRow = -1;
    _lastVisibleRow = -1;    
    // Запоминаем пользовательный размер колонок
    QList<int> columnsWidth;
    for(int i = 0; i < 5; i++)
        columnsWidth << _table->columnWidth(i);
    // Удаляем данные с модели
    model->clear();
    // Восстанавливаем заголовки
    model->setHorizontalHeaderLabels(horizontalHeaders);
    // Восстанавливаем пользователькую ширину колонок
    for(int i = 0; i < 5; i++)
        _table->setColumnWidth(i, columnsWidth.at(i));
}

void TableConsole::send(void){
    QString text(_input->text());                                             // Берём текст с поля ввода
    if(text != "" && _serial->getConnectionStatus() != SerialGui::CLOSED) {   // Если в поле ввода не пусто,
        QByteArray data = Converter::hexStringToByteArray(text, ' ');         // и порт открыт, то конвертируем
        _serial->send(data);                                                  // в байтовый массив и отправляем
        _input->clear();                                                      // в порт, после очищаем поле ввода
    }
}
void TableConsole::send(QString text){
    if(text != "" && _serial->getConnectionStatus() != SerialGui::CLOSED) {   // Если в поле ввода не пусто,
        QByteArray data = Converter::hexStringToByteArray(text, ' ');         // и порт открыт, то конвертируем
        _serial->send(data);                                                  // в байтовый массив и отправляем в порт
    }
}
void TableConsole::sended(QByteArray data){
    QString hexString = Converter::byteArrayToHexString(data, ' ');       // Посланные данные нужно сконверировать в текст
    Converter::removeNonPrintedSymbols(data, '.');
    QString asciiString = Converter::byteArrayToAsciiString(data);
    appendData(TableConsole::INCOMING, hexString, asciiString);          // А после добавить в таблицу
}
void TableConsole::received(QByteArray data){
    QString hexString = Converter::byteArrayToHexString(data, ' ');       // Посланные данные нужно сконверировать в текст
    Converter::removeNonPrintedSymbols(data, '.');
    QString asciiString = Converter::byteArrayToAsciiString(data);
    appendData(TableConsole::OUTGOING, hexString, asciiString);           // А после добавить в таблицу
}

void TableConsole::slotTextDelimiter(void){
    QLineEdit *input = static_cast<QLineEdit*>(QObject::sender());   // Определим отправителя
    QString source = input->text();                                  // Берем текст с поля
    Converter::setDelimitersInHexString(source, 2, ' ');             // Группируем символы
    input->setText(source);                                          // Возвращаем в поле ввода
}
bool TableConsole::echoMode(void){
    return _echo;
}
bool TableConsole::cyclicMode(void){
    return _cyclic;
}
int TableConsole::cyclicInterval(void){
    return _cyclicInterval;
}
QString& TableConsole::bindData(void){
    return _bindData;
}
void TableConsole::setEchoMode(bool state){   
    if(state)                                                                 // Режим эхо, не просто маскирует посылаемые
        connect(_serial, &SerialGui::send, this, &TableConsole::sended);      // данные, а фактически подписывает/отписывает
    else                                                                      // нас на исходящие данные порта
        disconnect(_serial, &SerialGui::send, this, &TableConsole::sended);   //
    _echo = state;                                                            // Фиксируем состояние
    emit echoModeChanged(state);                                              // Уведомляем о изменении
}
void TableConsole::setCyclicMode(bool mode){
    _cyclic = mode;
    emit cyclicModeChanged(mode);
}
void TableConsole::setCyclicInterval(int interval){
    if(interval < 0)
        return;
    _cyclicInterval = interval;
    emit cyclicIntervalChanged(interval);
}
void TableConsole::setBindData(QString text){
    _bindData = text;
    emit bindDataChanged(text);
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
void TableConsole::retranslate(void){
    _sendButton->setText(tr("Send"));
    _clearButton->setText(tr("Clear"));
    horizontalHeaders.clear();
    horizontalHeaders << "#" << tr("Timestamp") << tr("Direction") << tr("Hex") << tr("Ascii");
    model->setHorizontalHeaderLabels(horizontalHeaders);
}
void TableConsole::cyclicTimeout(void){
   send(_bindData);
}
void TableConsole::startCyclicSending(void){
    send(_bindData);
    if(cyclicMode()) {
        timer->start(cyclicInterval());
        emit cyclicIsRunning();
    }
    else
        emit cyclicStopped();
}
void TableConsole::stopCyclicSending(void){
    timer->stop();
    emit cyclicStopped();
}

