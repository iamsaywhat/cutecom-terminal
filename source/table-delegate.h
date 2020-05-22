#ifndef TABLE_DELEGATE_H
#define TABLE_DELEGATE_H

#include <QItemDelegate>

class TableDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    TableDelegate(QObject *parent = nullptr);

    /**
     * @brief createEditor - Виджет редактор
     * @param parent - Указатель на родителя
     * @param option - Параметры элемента
     * @param index  - Индекс редактируемого элемента в модели
     * @return Устанавливаемый виджет
     */
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    /**
     * @brief drawDisplay
     * @param painter
     * @param option
     * @param rect
     * @param text
     */
    void drawDisplay(QPainter *painter,
                     const QStyleOptionViewItem &option,
                     const QRect &rect,
                     const QString &text) const;

    /**
     * @brief sizeHint
     * @param option
     * @param index
     * @return
     */
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;

    /**
     * @brief setEditorData
     * Переопределение функции переноса содержимого из элемента
     * с индексом index в редактор
     * @param editor - Используемый редактор
     * @param index  - Индекс элемента
     */
    void setEditorData(QWidget *editor,
                       const QModelIndex &index) const;

    /**
     * @brief setModelData
     * Переопределение функции переноса содержимого
     * в модель по индексу
     * @param editor - Используемый редактор
     * @param model  - Текущая модель
     * @param index  - Индекс элемента модели
     */
    void setModelData(QWidget * editor,
                      QAbstractItemModel * model,
                      const QModelIndex & index ) const;
};

#endif // TABLE_DELEGATE_H
