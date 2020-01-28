#include "framelesswindow.h"
#include <QGraphicsDropShadowEffect>



FramelessWindow::FramelessWindow(QWidget *parent) : QWidget(parent) {
    QGridLayout *layout = new QGridLayout(this);
    this->setLayout(layout);
    sizeControl = new SizeController(this);
}


void FramelessWindow::setCentralWidget(QWidget *widget){
    this->layout()->addWidget(widget);
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(10); // Устанавливаем радиус размытия
    shadowEffect->setOffset(0);     // Устанавливаем смещение тени
    widget->setGraphicsEffect(shadowEffect);   // Устанавливаем эффект тени на окно
    widget->layout()->setMargin(0);            // Устанавливаем размер полей
    widget->layout()->setSpacing(0);
    widget->setAutoFillBackground(true);
}


FramelessWindow::~FramelessWindow() {
    delete sizeControl;
}

void FramelessWindow::closeFramelessWindow(void) {
    this->close();
}
void FramelessWindow::maximazeFramelessWindow (void) {
    // При нажатии на кнопку максимизации/нормализации окна
    // Делаем проверку на то, в каком состоянии находится окно и переключаем его режим
    if (!this->isMaximized())
    {
        //ui->maximazeButton->setStyleSheet(StyleHelper::getRestoreStyleSheet());

        /* Здесь важный нюанс: при разворачивании окна - необходимо поля centralwidget
         * (на уровень выше interfaceWidget) убрать, чтобы окно полноценно развернулось
         * в полный экран, однако, при этом исчезает тень, которую в полноэкранном режиме
         * и не должно быть видно, но при минимизации окна нужно вернуть */
        this->layout()->setMargin(0);
        this->showMaximized();
    }
    else
    {
        // Заметьте, каждый раз устанавливаем новый стиль в эту кнопку
        //ui->maximazeButton->setStyleSheet(StyleHelper::getMaximizeStyleSheet());
        /* Здесь при минимизации возвращаем поля в исходный вид,
         * чтобы тень отобразилась */
        this->layout()->setMargin(9);
        this->showNormal();
    }
}
void FramelessWindow::minimazeFramelessWindow (void) {
    this->showMinimized();
}










SizeController::SizeController(QWidget *target) :
    _target(target),
    _cursorchanged(false),
    _leftButtonPressed(false),
    _borderWidth(10),
    _dragPos(QPoint())
{
    _target->setMouseTracking(true);                        /* Разрешаем отслеживания положения курсоса при отпущеных кнопках мыши */
    _target->setWindowFlags(Qt::FramelessWindowHint);       /* Отключаем стандартное обрамление */
    _target->setAttribute(Qt::WA_Hover);                    /* Разрешаем генерацию событий по наведению мыши */
    _target->installEventFilter(this);                      /* Применяем фильтр событий данному виджету */
    _target->setAutoFillBackground(true);
    _target->setAttribute(Qt::WA_TranslucentBackground);
    _target->layout()->setMargin(10);

//    QHBoxLayout *layout = new QHBoxLayout(_substrate);
//    _substrate->setLayout(layout);
//    _substrate->layout()->addWidget(mainWidget);

//    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
//    shadowEffect->setBlurRadius(10); // Устанавливаем радиус размытия
//    shadowEffect->setOffset(0);     // Устанавливаем смещение тени
//    _target->setGraphicsEffect(shadowEffect);   // Устанавливаем эффект тени на окно
//    _target->layout()->setMargin(0);            // Устанавливаем размер полей
//    _target->layout()->setSpacing(0);
//    _target->setAutoFillBackground(true);
}

/* Фильтр событий */
bool SizeController::eventFilter(QObject* object, QEvent* event)
{
    /* Выбираем только интересующие нас события */
    if (event->type() == QEvent::MouseMove ||           /* Движение мыши */
        event->type() == QEvent::HoverMove ||           /* Наведение мыши */
        event->type() == QEvent::Leave ||               /* Курсор за пределами виджета */
        event->type() == QEvent::MouseButtonPress ||    /* Нажатие кнопки мыши */
        event->type() == QEvent::MouseButtonRelease) {  /* Отпускани кнопки мыши */

        switch (event->type()) {
        case QEvent::MouseMove:
            mouseMove(static_cast<QMouseEvent*>(event));
            break;
        case QEvent::HoverMove:
            mouseHover(static_cast<QHoverEvent*>(event));
            break;
        case QEvent::Leave:
            mouseLeave(event);
            break;
        case QEvent::MouseButtonPress:
            mousePress(static_cast<QMouseEvent*>(event));
            break;
        case QEvent::MouseButtonRelease:
            mouseRelease(static_cast<QMouseEvent*>(event));
            break;
        default:
            break;
        }
        return true;
    }
    else {
        return _target->eventFilter(object, event);
    }
}

void SizeController::mouseHover(QHoverEvent *event) {
    updateCursorShape(_target->mapToGlobal(event->pos())); /* Передаём глобальную позицию курсора на экране */
}

void SizeController::mouseLeave(QEvent *event) {
    if (!_leftButtonPressed) {      /* Если кнопка не нажата, а курсор покинул виджет */
        _target->unsetCursor();     /* необходимо гарантированно сбросить внешний вид курсора */
    }
}

void SizeController::mousePress(QMouseEvent* event) {
    if (event->button() & Qt::LeftButton) {                  /* Проверяем была ли нажата именно левая кнопка мыши */
        _leftButtonPressed = true;                           /* Устанавливаем флаг, что она была нажата */
        calculateCursorPosition(event->globalPos(),          /* Определяем зону, в которой произошел клик мыши */
                                _target->frameGeometry(),
                                _mousePress);
        /* Если прямоугольник взятый по размеру виджета, с удалёнными полями, содержит позицию клика курсора */
        if (_target->rect().marginsRemoved(QMargins(borderWidth(), borderWidth(), borderWidth(), borderWidth())).contains(event->pos())) {
            _dragStart = true;           /* Выставляем флаг начала перемещения */
            _dragPos = event->pos();     /* Фиксируем координату перемещения */
        }
    }
}

void SizeController::mouseRelease(QMouseEvent *event) {
    if (event->button() & Qt::LeftButton) {   /* Проверяем была ли отпущена именно левая кнопка мыши */
        _leftButtonPressed = false;           /* Сбрасываем флаг: кнопка нажата */
        _dragStart = false;                   /* Сбрасываем флаг: перемещение началось */
    }
}

void SizeController::mouseMove(QMouseEvent *event) {
    if (_leftButtonPressed) {                        /* Курсор двигается и левая кнопка мыши нажата */
        if (_dragStart) {                            /* И был выставлен флаг режима перемещения окна */
            _target->move(_target->frameGeometry().topLeft() + (event->pos() - _dragPos));
        }

        if (!_mousePress.testFlag(Edge::None)) {                /* Клик левой кнопки мыши был в одной из масштабирующих зон */
            int left = _target->frameGeometry().left();         /* Сначала берём исходные размеры окна */
            int top = _target->frameGeometry().top();           /* left, right - x координаты левой и правой сторон окна */
            int right = _target->frameGeometry().right();       /* top и bottom - y координаты верхней и нижней стороны окна */
            int bottom = _target->frameGeometry().bottom();
            switch (_mousePress) {                              /* В зависимости от области в которой был выполнен клик мыши */
            case Edge::Top:                                     /* Будем изменять координаты сторон */
                top = event->globalPos().y();
                break;
            case Edge::Bottom:
                bottom = event->globalPos().y();
                break;
            case Edge::Left:
                left = event->globalPos().x();
                break;
            case Edge::Right:
                right = event->globalPos().x();
                break;
            case Edge::TopLeft:
                top = event->globalPos().y();
                left = event->globalPos().x();
                break;
            case Edge::TopRight:
                right = event->globalPos().x();
                top = event->globalPos().y();
                break;
            case Edge::BottomLeft:
                bottom = event->globalPos().y();
                left = event->globalPos().x();
                break;
            case Edge::BottomRight:
                bottom = event->globalPos().y();
                right = event->globalPos().x();
                break;
            }
            QRect newRect(QPoint(left, top), QPoint(right, bottom));  /* Создаём новый квадрат по полученным координатам сторон */
            if (newRect.width() < _target->minimumWidth()) {          /* Здесь запрещаем изменение геометрии окна менее минимального размера */
                left = _target->frameGeometry().x();
            }
            else if (newRect.height() < _target->minimumHeight()) {
                top = _target->frameGeometry().y();
            }
            _target->setGeometry(QRect(QPoint(left, top), QPoint(right, bottom)));      /* Устанавливаем размер окна по вычисленным координатам сторон */
        }
    }
    else {                                          /* Если движение мыши без нажатой левой кнопки */
        updateCursorShape(event->globalPos());      /* То следим только за внешним видом курсора */
    }
}

/* Обновление вида курсора */
void SizeController::updateCursorShape(const QPoint &position) {
    if (_target->isFullScreen() || _target->isMaximized()) {   /* В полноэкранном режиме вид курсора на */
        if (_cursorchanged) {                                  /* масштабирующие стрелочки не меняем */
            _target->unsetCursor();                            /* Гаранированно сбрасываем внешний вид */
        }
        return;
    }
    if (!_leftButtonPressed) {                                                        /* В зависимости от зоны положения курсора */
        calculateCursorPosition(position, _target->frameGeometry(), _mouseMove);      /* будем менять соответствующи образом вид на */
        _cursorchanged = true;                                                        /* масштабирующие стрелочки */
        if (_mouseMove.testFlag(Edge::Top) || _mouseMove.testFlag(Edge::Bottom))
            _target->setCursor(Qt::SizeVerCursor);

        else if (_mouseMove.testFlag(Edge::Left) || _mouseMove.testFlag(Edge::Right))
            _target->setCursor(Qt::SizeHorCursor);

        else if (_mouseMove.testFlag(Edge::TopLeft) || _mouseMove.testFlag(Edge::BottomRight))
            _target->setCursor(Qt::SizeFDiagCursor);

        else if (_mouseMove.testFlag(Edge::TopRight) || _mouseMove.testFlag(Edge::BottomLeft))
            _target->setCursor(Qt::SizeBDiagCursor);

        else if (_cursorchanged) {      /* Курсор не попадает ни в одну из зон */
            _target->unsetCursor();     /* поэтому мы его гарантированно приводим */
            _cursorchanged = false;     /* к стандартному виду */
        }
    }
}

/* Определение зоны нахождения курсора */
void SizeController::calculateCursorPosition(const QPoint &position, const QRect &framerect, Edges &edge) {

    bool onLeft =
            position.x() >= framerect.x() - _borderWidth &&                       /* position.x на левой границе виджета +- borderWidth */
            position.x() <= framerect.x() + _borderWidth &&
            position.y() <= framerect.y() + framerect.height() - _borderWidth &&  /* position.y по всех высоте виджета минус borderWidth*/
            position.y() >= framerect.y() + _borderWidth;                         /* с обоих концов */

    bool onRight =
            position.x() >= framerect.x() + framerect.width() - _borderWidth &&   /* position.x на правой границе виджета +- borderWidth */
            position.x() <= framerect.x() + framerect.width() + _borderWidth &&
            position.y() <= framerect.y() + framerect.height()- _borderWidth &&   /* position.y по всех высоте виджета минус borderWidth*/
            position.y() >= framerect.y() + _borderWidth;                         /* с обоих концов */

    bool onBottom =
            position.x() >= framerect.x() + _borderWidth &&
            position.x() <= framerect.x() + framerect.width() - _borderWidth  &&
            position.y() >= framerect.y() + framerect.height() - _borderWidth &&
            position.y() <= framerect.y() + framerect.height() + _borderWidth;

    bool onTop =
            position.x() >= framerect.x() + _borderWidth &&
            position.x() <= framerect.x() + framerect.width() - _borderWidth &&
            position.y() >= framerect.y() - _borderWidth &&
            position.y() <= framerect.y() + _borderWidth;

    bool  onBottomLeft =
            position.x() <= framerect.x() + _borderWidth &&
            position.x() >= framerect.x() - _borderWidth &&
            position.y() <= framerect.y() + framerect.height() + _borderWidth &&
            position.y() >= framerect.y() + framerect.height() - _borderWidth;

    bool onBottomRight =
            position.x() >= framerect.x() + framerect.width() - _borderWidth &&
            position.x() <= framerect.x() + framerect.width() + _borderWidth &&
            position.y() >= framerect.y() + framerect.height() - _borderWidth &&
            position.y() <= framerect.y() + framerect.height() + _borderWidth;

    bool onTopRight =
            position.x() >= framerect.x() + framerect.width() - _borderWidth &&
            position.x() <= framerect.x() + framerect.width() + _borderWidth &&
            position.y() >= framerect.y() - _borderWidth &&
            position.y() <= framerect.y() + _borderWidth;

    bool onTopLeft =
            position.x() >= framerect.x() - _borderWidth &&
            position.x() <= framerect.x() + _borderWidth &&
            position.y() >= framerect.y() - _borderWidth &&
            position.y() <= framerect.y() + _borderWidth;

    /* Выставляем флаг согласно определенной зоне */
    if (onLeft)
        edge = Left;
    else if (onRight)
        edge = Right;
    else if (onBottom)
        edge = Bottom;
    else if (onTop)
        edge = Top;
    else if (onBottomLeft)
        edge = BottomLeft;
    else if (onBottomRight)
        edge = BottomRight;
    else if (onTopRight)
        edge = TopRight;
    else if (onTopLeft)
        edge = TopLeft;
    else
        edge = None;
}
