#include "framelesswindow.h"
#include <QDebug>
#include <QSettings>

FramelessWindow::FramelessWindow(QWidget *parent) : QWidget(parent) {
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(centralWidget());
    this->setLayout(layout);
    this->layout()->setMargin(shadowSize);
    shadowEffect = new QGraphicsDropShadowEffect(this); /* Создаём эффект тени окна */
    shadowEffect->setColor(QColor(0, 0, 0, 200));       /* Задаём цвет тени и ее прозрачность */
    shadowEffect->setBlurRadius(shadowSize);            /* Устанавливаем радиус размытия тени */
    shadowEffect->setOffset(0);                         /* Устанавливаем смещение тени */
    centralWidget()->setGraphicsEffect(shadowEffect);   /* Устанавливаем эффект тени на окно */
    centralWidget()->setAutoFillBackground(true);       /* Автозаполнение центрального виджета */
    control = new FramelessController(this);            /* Создаём контроллер окна */
    control->setBorder(10);                             /* Размер области по краям окна, для маштабирования */
    control->setWindowHeaderSize(10);                   /* Высота заголовка окна */
}
FramelessWindow::~FramelessWindow() {
    delete control;
    delete shadowEffect;
}
void FramelessWindow::showMaximized(void) {
    if (!this->isMaximized()) {
        this->layout()->setMargin(0);      /* При переключение в полноэкранный режим, чтобы это произошло */
        this->QWidget::showMaximized();    /* корректно, необходимо "спрятать прозрачную рамку и тенью */
        emit windowMaximized();            /* а при выходе из полноэкранного режима надо вернуть */
    }
}
void FramelessWindow::showNormal(void) {
    if( this->isMaximized()) {
        this->layout()->setMargin(shadowSize);        /* При выходе из полноэкранного режима, чтобы это произошло */
        this->QWidget::showNormal();                  /* корректно, необходимо вернуть прозрачную рамку и тенью */
        emit windowNormalized();
    }
}
void FramelessWindow::showMinimized(void) {
    this->QWidget::showMinimized();
    emit windowMinimized();
}
void FramelessWindow::changeFullScreenMode(void) {
    if (isMaximized())
        showNormal();
    else
        showMaximized();
}
void FramelessWindow::setRezizeBorder (int border){
    control->setBorder(border);
}
void FramelessWindow::setDragHeaderSise (int size){
    control->setWindowHeaderSize(size);
}






FramelessController::FramelessController(FramelessWindow *target) : _target(target) {
    _cursorchanged = false;
    _leftButtonPressed = false;
    _dragPos = QPoint();
    _target->setMouseTracking(true);                        /* Разрешаем отслеживания положения курсоса при отпущеных кнопках мыши */
    _target->setWindowFlags(Qt::FramelessWindowHint);       /* Отключаем стандартное обрамление */
    _target->setAttribute(Qt::WA_Hover);                    /* Разрешаем генерацию событий по наведению мыши */
    _target->installEventFilter(this);                      /* Применяем фильтр событий данному виджету */
    _target->setAutoFillBackground(true);                   /* Включаем автозаполнение виджета */
    _target->setAttribute(Qt::WA_TranslucentBackground);    /* Делам фон прозрачным */
    setBorder(10);
}
void FramelessController::setBorder(int size) {                   /* Установка размера границы по краям окна, в которой курсор */
    _border = size;                                          /* будет захватываться для масштабирования */
}
int FramelessController::border() const {
    return _border;
}
void FramelessController::setWindowHeaderSize(int size) {         /* Установка размера header окна, который используется для */
    _windowHeadersize = size;                                /* перетаскивания окна. Фактически означает ширину зоны вниз от */
}                                                            /* верхнего края окна, в которой будет происходить захват */
int FramelessController::windowHeaderSize(void) {                 /* курсора для операции перетаскивания и перехода в полноэкранный */
    return _windowHeadersize;                                /* режим по двойному щелчку */
}
bool FramelessController::eventFilter(QObject* object, QEvent* event) {
    /* Выбираем только интересующие нас события */
    if (event->type() == QEvent::MouseMove ||            /* Движение мыши */
        event->type() == QEvent::HoverMove ||            /* Наведение мыши */
        event->type() == QEvent::Leave ||                /* Курсор за пределами виджета */
        event->type() == QEvent::MouseButtonPress ||     /* Нажатие кнопки мыши */
        event->type() == QEvent::MouseButtonRelease ||   /* Отпускани кнопки мыши */
        event->type() == QEvent::MouseButtonDblClick) {  /* Двойной щелчок мыши */

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
        case QEvent::MouseButtonDblClick:
            mouseDoubleClick(static_cast<QMouseEvent*>(event));
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

void FramelessController::mouseHover(QHoverEvent *event) {           /* Здесь следим только за внешним видом курсора */
    updateCursorShape(_target->mapToGlobal(event->pos()));      /* Передаём глобальную позицию курсора на экране */
}

void FramelessController::mouseLeave(QEvent *event) {
    Q_UNUSED(event)
    if (!_leftButtonPressed) {                                  /* Если кнопка не нажата, а курсор покинул виджет */
        _target->unsetCursor();                                 /* необходимо гарантированно сбросить внешний вид курсора */
    }
}

void FramelessController::mousePress(QMouseEvent* event) {
    if (event->button() & Qt::LeftButton) {                  /* Проверяем была ли нажата именно левая кнопка мыши */
        _leftButtonPressed = true;                           /* Устанавливаем флаг, что она была нажата */
        calculateCursorPosition(event->globalPos(),          /* Определяем зону, в которой произошел клик мыши */
                                _target->frameGeometry(),
                                _mousePress);
         QRect actionRect = _target->rect();                                   /* Активную (видимую) часть окна. Если окно не полноэкранном режиме, */
        if(!_target->isMaximized() && !_target->isFullScreen()){               /* то фактический размер окна включает и невидимые рамки, которые здесь */
            actionRect = actionRect.marginsRemoved(QMargins(border(), border(), border(), border()));  /* нельзя учитывать, поэтому их вычитаем из общего размера окна */
        }                                                                                              /* А далее ограничиваем эту зону размером header окна */
        if(windowHeaderSize() < actionRect.height())
            actionRect.setHeight(windowHeaderSize());                                                  /* то есть обрезаем ее по высоте до размера windowHeaderSize() */
        if (actionRect .contains(event->pos())) {   /* Если эта зона содержит позицию клика курсора */
            _dragStart = true;                      /* Выставляем флаг начала перемещения */
            _dragPos = event->pos();                /* Фиксируем координату перемещения */
        }
    }
}
void FramelessController::mouseDoubleClick(QMouseEvent* event){
    if (event->button() & Qt::LeftButton) {                  /* Проверяем была ли нажата именно левая кнопка мыши */
        _leftButtonPressed = true;                           /* Устанавливаем флаг, что она была нажата */
        calculateCursorPosition(event->globalPos(),          /* Определяем зону, в которой произошел клик мыши */
                                _target->frameGeometry(),
                                _mousePress);
        QRect actionRect = _target->rect();                                   /* Активную (видимую) часть окна. Если окно не полноэкранном режиме, */
        if(!_target->isMaximized() && !_target->isFullScreen()){               /* то фактический размер окна включает и невидимые рамки, которые здесь */
            actionRect = actionRect.marginsRemoved(QMargins(border(), border(), border(), border()));  /* нельзя учитывать, поэтому их вычитаем из общего размера окна */
        }                                                                                              /* А далее ограничиваем эту зону размером header окна */
        if(windowHeaderSize() < actionRect.height())
            actionRect.setHeight(windowHeaderSize());                                                  /* то есть обрезаем ее по высоте до размера windowHeaderSize() */
        if (actionRect .contains(event->pos())) {   /* Если эта зона содержит позицию двойного клика курсора */
            _target->changeFullScreenMode();        /* То меняем режим на полноэкранный или наоборот */
        }
    }
}

void FramelessController::mouseRelease(QMouseEvent *event) {
    if (event->button() & Qt::LeftButton) {   /* Проверяем была ли отпущена именно левая кнопка мыши */
        _leftButtonPressed = false;           /* Сбрасываем флаг: кнопка нажата */
        _dragStart = false;                   /* Сбрасываем флаг: перемещение началось */
    }
}

void FramelessController::mouseMove(QMouseEvent *event) {
    if (_leftButtonPressed) {                        /* Курсор двигается и левая кнопка мыши нажата */
        if (_dragStart) {                            /* И был выставлен флаг режима перемещения окна */
            if (_target->isFullScreen() || _target->isMaximized()) {
                 auto part = event->screenPos().x() / _target->width();    /* Это вариант, когда положение курсора */
                 _target->showNormal();                                    /* на нормализованном виджете определяется */
                 int offsetX = static_cast<int>(_target->width() * part);  /* по пропорции */
//                 _target->showNormal();                                  /* Это вариант, когда курсор всегда */
//                 int offsetX = _target->width()/2;                       /* будет по центру x оси виджета */
                 _target->move(static_cast<int>(event->screenPos().x()) - offsetX, 0);
                 _dragPos = QPoint(offsetX, event->y());
            }
            _target->move(_target->frameGeometry().topLeft() + (event->pos() - _dragPos));
        }
        if (!_mousePress.testFlag(Edge::None) && !_dragStart) {                /* Клик левой кнопки мыши был в одной из масштабирующих зон */
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
void FramelessController::updateCursorShape(const QPoint &position) {
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
void FramelessController::calculateCursorPosition(const QPoint &position, const QRect &framerect, Edges &edge) {

    bool onLeft =
            position.x() >= framerect.x() - border() &&                       /* position.x на левой границе виджета +- borderWidth */
            position.x() <= framerect.x() + border() &&
            position.y() <= framerect.y() + framerect.height() - border() &&  /* position.y по всех высоте виджета минус borderWidth*/
            position.y() >= framerect.y() + border();                         /* с обоих концов */

    bool onRight =
            position.x() >= framerect.x() + framerect.width() - border() &&   /* position.x на правой границе виджета +- borderWidth */
            position.x() <= framerect.x() + framerect.width() + border() &&
            position.y() <= framerect.y() + framerect.height()- border() &&   /* position.y по всех высоте виджета минус borderWidth*/
            position.y() >= framerect.y() + border();                         /* с обоих концов */

    bool onBottom =
            position.x() >= framerect.x() + border() &&
            position.x() <= framerect.x() + framerect.width() - border()  &&
            position.y() >= framerect.y() + framerect.height() - border() &&
            position.y() <= framerect.y() + framerect.height() + border();

    bool onTop =
            position.x() >= framerect.x() + border() &&
            position.x() <= framerect.x() + framerect.width() - border() &&
            position.y() >= framerect.y() - border() &&
            position.y() <= framerect.y() + border();

    bool  onBottomLeft =
            position.x() <= framerect.x() + border() &&
            position.x() >= framerect.x() - border() &&
            position.y() <= framerect.y() + framerect.height() + border() &&
            position.y() >= framerect.y() + framerect.height() - border();

    bool onBottomRight =
            position.x() >= framerect.x() + framerect.width() - border() &&
            position.x() <= framerect.x() + framerect.width() + border() &&
            position.y() >= framerect.y() + framerect.height() - border() &&
            position.y() <= framerect.y() + framerect.height() + border();

    bool onTopRight =
            position.x() >= framerect.x() + framerect.width() - border() &&
            position.x() <= framerect.x() + framerect.width() + border() &&
            position.y() >= framerect.y() - border() &&
            position.y() <= framerect.y() + border();

    bool onTopLeft =
            position.x() >= framerect.x() - border() &&
            position.x() <= framerect.x() + border() &&
            position.y() >= framerect.y() - border() &&
            position.y() <= framerect.y() + border();

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
