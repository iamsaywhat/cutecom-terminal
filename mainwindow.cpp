#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QWidget>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Создаём окно настроек COM */
    settingsWindow = new SettingsDialog(this);
    /* Выполняем связку класса работы с портом окном настроек */
    serial = new SerialForGUI(settingsWindow->Ports,                // ComboBox c доступными Com портами
                              settingsWindow->Baudrate,             // ComboBox с настройками скорости
                              settingsWindow->Parity,               // ComboBox с настройками паритета
                              settingsWindow->Databits,             // ComboBox с настройками бит данных
                              settingsWindow->Stopbits,             // ComboBox с настройками стоп-бит
                              settingsWindow->Flowcontrol,          // ComboBox с настройками контроля
                              settingsWindow->ConnectDisconnect);   // Кнопка подключения/отключения
    /* Выполняем связку класса работы с консолью с формами */
    console = new ConsoleWindow(this,
                                serial,               /* Указатель на SerialSettings экземпляр */
                                ui->consoleField,        /* Указатель на QPlainTextEdit форму*/
                                ui->inputConsoleField,          /* Указатель на QLineEdit форму */
                                ui->sendConsoleButton,       /* Указатель на QPushButton форму*/
                                ui->clearConsoleButton);     /* Указатель на QPushButton форму*/
    /* Создаём и связываем табличную консоль с формой */
    tableConsole = new TableConsole(this,
                                    serial,                  /* Указатель на SerialSettings экземпляр */
                                    ui->tableField,           /* Указатель на QTableView форму */
                                    ui->inputTableField,        /* Указатель на QLineEdit форму */
                                    ui->sendTableButton,     /* Указатель на QPushButton форму*/
                                    ui->clearTableButton);   /* Указатель на QPushButton форму*/


    /* Включаем сетку на таблице */
    ui->tableField->setShowGrid(true);



    setWindowFlags(Qt::FramelessWindowHint);               /* Отключаем оформление окна */
    setAttribute(Qt::WA_TranslucentBackground);            /* Делаем фон главного окна прозрачным */
    ui->interfaceWidget->setAutoFillBackground(true);      /* По-умолчанию фон нашего интефейсного виджета прозрачен, поэтому разрешаем его заполнить автоматически */
    ui->interfaceWidget->layout()->setMargin(0);           /* Задаём размер полей */
    ui->interfaceWidget->layout()->setSpacing(0);          /* Задаём размер пространства между элементами в размещении виджета */
    QGraphicsDropShadowEffect *shadowEffect =
            new QGraphicsDropShadowEffect(this);           /* Создаем графиеский эффект - тень */
    shadowEffect->setBlurRadius(9);                        /* Задаём радиус размытия тени */
    shadowEffect->setOffset(0);                            /* Задаём смещение тени */
    ui->interfaceWidget->setGraphicsEffect(shadowEffect);  /* Применяем эффект тени на окно */

    this->setMouseTracking(true);                          /* Включаем отслеживание курсора без нажатых кнопок */
    ui->centralwidget->setMouseTracking(true);             /* и в родительском окне, и в дочерних виджетах, */
    ui->interfaceWidget->setMouseTracking(true);           /* потому что только так это все заработает */

    /* Подключение кнопок закрыть, свернуть, развернуть окно, так как стандартные скрыты */
    connect(ui->closeButton,    &QToolButton::clicked, this, &QWidget::close);
    connect(ui->minimizeButton, &QToolButton::clicked, this, &QWidget::showMinimized);
    connect(ui->maximazeButton, &QToolButton::clicked, [this](){
        // При нажатии на кнопку максимизации/нормализации окна
        // Делаем проверку на то, в каком состоянии находится окно и переключаем его режим
        if (!this->isMaximized())
        {
            //ui->maximazeButton->setStyleSheet(StyleHelper::getRestoreStyleSheet());

            /* Здесь важный нюанс: при разворачивании окна - необходимо поля centralwidget
             * (на уровень выше interfaceWidget) убрать, чтобы окно полноценно развернулось
             * в полный экран, однако, при этом исчезает тень, которую в полноэкранном режиме
             * и не должно быть видно, но при минимизации окна нужно вернуть */
            ui->centralwidget->layout()->setMargin(0);
            this->showMaximized();
        }
        else
        {
            // Заметьте, каждый раз устанавливаем новый стиль в эту кнопку
            //ui->maximazeButton->setStyleSheet(StyleHelper::getMaximizeStyleSheet());
            /* Здесь при минимизации возвращаем поля в исходный вид,
             * чтобы тень отобразилась */
            ui->centralwidget->layout()->setMargin(9);
            this->showNormal();
        }
    });


    /* Дополнительные функциональные кнопки */
    connect(ui->connectionButton, &QToolButton::clicked, settingsWindow, &SettingsDialog::show);


    // При запуске будем предлагать подключение
    //settingsWindow->show();
}

MainWindow::~MainWindow()
{
    delete console;
    delete serial;
    delete settingsWindow;
    delete ui;
}



MainWindow::ClickType MainWindow::checkResizableField(QMouseEvent *event)
{
    QPointF position = event->screenPos();  /* Берем координаты курсора на экране */
    qreal x = this->x();                    /* Берём координаты окна приложения */
    qreal y = this->y();                    /* (левого верхнего угла окна) */
    qreal width = this->width();            /* Берем ширину окна */
    qreal height = this->height();          /* Берем высоту окна */

    /* Определяем зоны, в которых может находиться курсор мыши */
    /* По ним будет определён статус клика */
    QRectF rectTop(x + 9, y, width - 18, 7);
    QRectF rectBottom(x + 9, y + height - 7, width - 18, 7);
    QRectF rectLeft(x, y + 9, 7, height - 18);
    QRectF rectRight(x + width - 7, y + 9, 7, height - 18);
    QRectF rectInterface(x + 9, y + 9, width - 18, height - 18);

    /* И в зависимости от области, в которой находится курсор
     * устанавливаем внешний вид курсора и возвращаем его статус */
    if (rectTop.contains(position))
    {
        setCursor(Qt::SizeVerCursor);
        return Top;
    }
    else if (rectBottom.contains(position))
    {
        setCursor(Qt::SizeVerCursor);
        return Bottom;
    }
    else if (rectLeft.contains(position))
    {
        setCursor(Qt::SizeHorCursor);
        return Left;
    }
    else if (rectRight.contains(position))
    {
        setCursor(Qt::SizeHorCursor);
        return Right;
    }
    else if (rectInterface.contains(position))
    {
        setCursor(QCursor());
        return Move;
    }
    else
    {
        setCursor(QCursor());
        return None;
    }
}



void MainWindow::setPreviousPosition(QPoint previousPosition)
{
    if (m_previousPosition == previousPosition)
        return;

    m_previousPosition = previousPosition;
    emit previousPositionChanged(previousPosition);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    /* Нас интересует левый клик мыши */
    if (event->button() == Qt::LeftButton)
    {
        /* Определяем, в какой области произошёл клик */
        m_leftMouseButtonPressed = checkResizableField(event);
        setPreviousPosition(event->pos()); // и устанавливаем позицию клика
    }
    return QWidget::mousePressEvent(event);

}


void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    /* При отпускании левой кнопки мыши сбрасываем состояние клика */
    if (event->button() == Qt::LeftButton)
        m_leftMouseButtonPressed = None;
    return QWidget::mouseReleaseEvent(event);
}


void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    /* При перемещении мыши, проверяем статус нажатия левой кнопки мыши */
    switch (m_leftMouseButtonPressed)
    {
    case Move: {
        // При этом проверяем, не максимизировано ли окно
        if (isMaximized())
        {
            // При перемещении из максимизированного состояния
            // Необходимо вернуть окно в нормальное состояние и установить стили кнопки
            // А также путём нехитрых вычислений пересчитать позицию окна,
            // чтобы оно оказалось под курсором
//            ui->btn_maximize->setStyleSheet(StyleHelper::getMaximizeStyleSheet());
            this->layout()->setMargin(9);
            auto part = event->screenPos().x() / width();
            this->showNormal();
            auto offsetX = width() * part;
            setGeometry(event->screenPos().x() - offsetX, 0, width(), height());
            setPreviousPosition(QPoint(offsetX, event->y()));
        }
        else
        {
            // Если окно не максимизировано, то просто перемещаем его относительно
            // последней запомненной позиции, пока не отпустим кнопку мыши
            auto dx = event->x() - m_previousPosition.x();
            auto dy = event->y() - m_previousPosition.y();
            setGeometry(x() + dx, y() + dy, width(), height());
        }
        break;
    }
    case Top:
    {
        // Для изменения размеров также проверяем на максимизацию
        // поскольку мы же не можем изменить размеры у максимизированного окна
        if (!isMaximized())
        {
            auto dy = event->y() - m_previousPosition.y();
            setGeometry(x(), y() + dy, width(), height() - dy);
        }
        break;
    }
    case Bottom:
    {
        if (!isMaximized())
        {
            auto dy = event->y() - m_previousPosition.y();
            setGeometry(x(), y(), width(), height() + dy);
            setPreviousPosition(event->pos());
        }
        break;
    }
    case Left:
    {
        if (!isMaximized())
        {
            auto dx = event->x() - m_previousPosition.x();
            setGeometry(x() + dx, y(), width() - dx, height());
        }
        break;
    }
    case Right:
    {
        if (!isMaximized())
        {
            auto dx = event->x() - m_previousPosition.x();
            setGeometry(x(), y(), width() + dx, height());
            setPreviousPosition(event->pos());
        }
        break;
    }
    default:
        // Если курсор перемещается по окну без зажатой кнопки,
        // то просто отслеживаем в какой области он находится
        // и изменяем его курсор
        checkResizableField(event);
        break;
    }
    qDebug()<<"From mouseMoveEvent";
    return QWidget::mouseMoveEvent(event);
}

