#include "delegate.h"

#include <QTextEdit>
#include <QPainter>
#include <QStyleOptionViewItem>

TextEditDelegate::TextEditDelegate(QObject *parent) : QItemDelegate(parent)
{
}

QWidget *TextEditDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &/* option */,
                                        const QModelIndex &/* index */) const
{
    // Создаём виджет
    QTextEdit *editor = new QTextEdit(parent);
    return editor;
}

void TextEditDelegate::drawDisplay(QPainter *painter,
                                   const QStyleOptionViewItem &option,
                                   const QRect &rect,
                                   const QString &text) const
{
    // Настройки внешнего вида: Если виджет включен применяем палитру состояния активный,
    // иначе состояния неактивный
    QPalette::ColorGroup cg = option.state & QStyle::State_Enabled
                              ? QPalette::Normal : QPalette::Disabled;
    // Если палитра соответствует активному состоянию,
    // а состояние виджета на самом деле: неактивен,
    // то применяем палитру неактивного состояния
    if (cg == QPalette::Normal && !(option.state & QStyle::State_Active))
        cg = QPalette::Inactive;

    // Если выджет был выбран
    if (option.state & QStyle::State_Selected)
    {
        // Применяем к данному прямоугольнику настроенную кисть
        painter->fillRect(rect, option.palette.brush(cg, QPalette::Highlight));
        // Применяем к ручке палитру QPalette::HighlightedText
        painter->setPen(option.palette.color(cg, QPalette::HighlightedText));
    }
    // Если виджет не выбран
    else
    {
        // Применяем к ручке палитру QPalette::Text
        painter->setPen(option.palette.color(cg, QPalette::Text));
    }
    // Если строка пуста - выходим
    if (text.isEmpty())
        return;
    // Если состояние: редактируется
    if (option.state & QStyle::State_Editing)
    {
        // Текущее состояние Qpainter сохраняем в стек
        painter->save();
        // Применяем к ручке палитру QPalette::Text
        painter->setPen(option.palette.color(cg, QPalette::Text));
        // Изменяем размер прямоугольника (?)
        painter->drawRect(rect.adjusted(0, 0, -1, -1));
        // Восстанавливаем из стека состояние QPainter
        painter->restore();
    }
    //
    QTextOption opt(option.displayAlignment);
    // Включаем перенос слов
    opt.setWrapMode(QTextOption::WordWrap);

    // Текущее состояние Qpainter сохраняем в стек
    painter->save();
    // Если состояние: выбран
    if (option.state & QStyle::State_Selected)
    {
        // Применяем к ручке палитру QPalette::HighlightedText
        painter->setPen(option.palette.color(cg, QPalette::HighlightedText));
    }
    // Если не выбран
    else
    {
        // Применяем к ручке палитру QPalette::Text
        painter->setPen(option.palette.color(cg, QPalette::Text));
    }
    // Рисуем текст с возможностью переносить текст
    painter->drawText(rect.adjusted(1, 0, 0, 0), text, opt);
    // Восстанавливаем из стека состояние QPainter
    painter->restore();
}

QSize TextEditDelegate::sizeHint(const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    int width = option.rect.width() - 1;
    int spW = option.fontMetrics.width(QChar(' '));
    QStringList words = index.data().toString().split(QChar(' '));
    int currRow = 0;
    int currCol = 0;
    QString clipString;
    int count = words.size();
    int wcount = 0;
    for (int i = 0; i < count; ++i)
    {
        int currW = option.fontMetrics.width(words[i]);
        if (currCol + currW < width)
        {
            currCol += currW + spW;
            clipString += words[i] + QString(" ");
            ++wcount;
        }
        else
        {
            if (wcount)
            {
                clipString += QString("\n");
                currCol = currW + spW;
                wcount = 1;
                clipString += words[i] + QString(" ");
                ++currRow;
            }
            else
            {
                clipString += words[i];
                clipString += QString("\n");
                currCol = 0;
                wcount = 0;
                ++currRow;
            }
        }
    }
    return option.fontMetrics.size(Qt::TextDontClip, clipString);
}

void TextEditDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QTextEdit* textEdit = qobject_cast<QTextEdit*>(editor);
    textEdit->setPlainText(index.data().toString());
    // Курсор переносим в конец, так удобнее
    QTextCursor cursor = textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    textEdit->setTextCursor(cursor);
}

void TextEditDelegate::setModelData(QWidget * editor,
                                    QAbstractItemModel * model,
                                    const QModelIndex & index ) const
{
    QTextEdit * textEdit = qobject_cast<QTextEdit*>(editor);
    model->setData(index, textEdit->toPlainText(), Qt::EditRole);
}
