#ifndef FRAMELESSWINDOW_H
#define FRAMELESSWINDOW_H

#include <QWidget>
#include <QObject>
#include <QEvent>
#include <QRect>
#include <QPoint>
#include <Qt>
#include <QHoverEvent>
#include <QMouseEvent>
#include <QGridLayout>

class SizeController;


class FramelessWindow : public QWidget
{
    Q_OBJECT
public:
    explicit FramelessWindow(QWidget *parent = nullptr);
    ~FramelessWindow();
    void setCentralWidget(QWidget *widget);
    const int recomendedMargin = 10;
    const int recomendedBorder = 10;

    void setRememberFrameProperties(bool);
    bool rememberFrameProperties(void) const;

private:
    SizeController *sizeControl = nullptr;
    bool _rememberFrameProperties = false;
    QRect normalWindowSize;

signals:

public slots:
    void closeFramelessWindow(void);
    void maximizeFramelessWindow (void);
    void minimizeFramelessWindow (void);
};




class SizeController : public QObject
{
    Q_OBJECT

public:
    enum Edge {    /* Перечисление масштабирующих зон */
        None        = 0x0,   /* Вне всех зон */
        Left        = 0x1,   /* Левая грань */
        Top         = 0x2,   /* Верхняя грань */
        Right       = 0x4,   /* Правая грань */
        Bottom      = 0x8,   /* Нижняя грань */
        TopLeft     = 0x10,  /* Верхний левый угол */
        TopRight    = 0x20,  /* Верхний правый угол */
        BottomLeft  = 0x40,  /* Нижний левый угол */
        BottomRight = 0x80,  /* Нижний правый угол */
    };
    Q_ENUM(Edge)                    /* Регистирируем перечисление в MOC */
    Q_DECLARE_FLAGS(Edges, Edge)    /* Объявляем соотвествующие флаги из перечисления QFlags */

    SizeController(FramelessWindow *target);

    void setBorderWidth(int w) {
        _borderWidth = w;
    }
    int borderWidth() const {
        return _borderWidth;
    }


protected:
    bool eventFilter(QObject *object, QEvent *event) override;
    void mouseHover(QHoverEvent*);
    void mouseLeave(QEvent*);
    void mousePress(QMouseEvent*);
    void mouseRelease(QMouseEvent*);
    void mouseMove(QMouseEvent*);
    void updateCursorShape(const QPoint &);
    void calculateCursorPosition(const QPoint &, const QRect &, Edges &);

private:
    FramelessWindow *_target  = nullptr;          /* Указатель на виджет */
    bool     _cursorchanged;                 /* Был ли изменем внешний вид курсора */
    bool     _leftButtonPressed;             /* Нажата ли левая кнопка мыши */
    Edges    _mousePress = Edge::None;      /* Тип (зона) клика */
    Edges    _mouseMove = Edge::None;       /* Зона расположения свободно перемещаемого курсора */
    int      _borderWidth;                    /* Ширина обрамления для захвата курсора */
    QPoint   _dragPos;                     /* Координата курсора, относительно которой окна будет перемещено */
    bool     _dragStart = false;             /* Флаг режима перемещения окна */
};

Q_DECLARE_OPERATORS_FOR_FLAGS(SizeController::Edges);

#endif // FRAMELESSWINDOW_H
