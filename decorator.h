#ifndef DECORATOR_H
#define DECORATOR_H

#include <QObject>
#include <QString>
#include <QToolButton>
#include <QPushButton>
#include <QLabel>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QTableView>
#include <QScrollBar>
#include <QComboBox>
#include <QCheckBox>

#include "ui_mainwidget.h"


class Decorator : public QObject
{
    Q_OBJECT    
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(IconMode iconMode READ iconMode WRITE setIconMode NOTIFY iconModeChanged)
    Q_PROPERTY(Color baseColor READ baseColor WRITE setBaseColor NOTIFY baseColorChanged)
    Q_PROPERTY(Color secondColor READ secondColor WRITE setSecondColor NOTIFY secondColorChanged)
    Q_PROPERTY(Color thirdColor READ thirdColor WRITE setThirdColor NOTIFY thirdColorChanged)
    Q_PROPERTY(Color closeHoverColor      READ closeHoverColor      WRITE setCloseHoverColor      NOTIFY closeHoverColorChanged)
    Q_PROPERTY(Color closePressedColor    READ closePressedColor    WRITE setClosePressedColor    NOTIFY closePressedColorChanged)
    Q_PROPERTY(Color maximizeHoverColor   READ maximizeHoverColor   WRITE setMaximizeHoverColor   NOTIFY maximizeHoverColorChanged)
    Q_PROPERTY(Color maximizePressedColor READ maximizePressedColor WRITE setMaximizePressedColor NOTIFY maximizePressedColorChanged)
    Q_PROPERTY(Color minimizeHoverColor   READ minimizeHoverColor   WRITE setMinimizeHoverColor   NOTIFY minimizeHoverColorChanged)
    Q_PROPERTY(Color minimizePressedColor READ minimizePressedColor WRITE setMinimizePressedColor NOTIFY minimizePressedColorChanged)

public:
    enum IconMode{
        Light,
        Dark,
    };
    struct Color{
        int background;
        int text;
        int selectionColor;
        int selectedText;
    };
    Decorator(QString name, IconMode iconmode, Color base, Color second, Color third);
    Decorator(const Decorator &decorator);
    Decorator(void);
    ~Decorator(void);
    Decorator &operator=(const Decorator &source);

    void apply(void);

    // Установка стиля для базовых виджетов
    void setBasicColorsToWidget(QWidget *widget, Color color);
    void setComboBoxColors(QComboBox *comboBox, Color activeColor, Color disableColor, Color listColor);
    void setCheckBoxColors(QCheckBox *checkBox, Color color, Color hoverColor, Color pressedColor);


    Ui::MainWidget* TargetGui(void) const  {return gui;}
    Color baseColor(void) const            {return _baseColor;}
    Color secondColor(void) const          {return _secondColor;}
    Color thirdColor(void) const           {return _thirdColor;}
    Color closeHoverColor(void) const      {return _closeHoverColor;}
    Color closePressedColor(void) const    {return _closePressedColor;}
    Color maximizeHoverColor(void) const   {return _maximizeHoverColor;}
    Color maximizePressedColor(void) const {return _maximizePressedColor;}
    Color minimizeHoverColor(void) const   {return _minimizeHoverColor;}
    Color minimizePressedColor(void) const {return _minimizePressedColor;}
    QString name(void) const               {return _name;}
    IconMode iconMode(void) const          {return _iconMode;}

    static void setTargetGui(Ui::MainWidget* target);
    void setBaseColor(Color color);
    void setSecondColor(Color color);
    void setThirdColor(Color color);
    void setCloseHoverColor(Color color);
    void setClosePressedColor(Color color);
    void setMaximizeHoverColor(Color color);
    void setMaximizePressedColor(Color color);
    void setMinimizeHoverColor(Color color);
    void setMinimizePressedColor(Color color);
    void setName(QString text);
    void setIconMode(IconMode mode);


    // Специальные таблицы стилей для главного окна
    static QString getAppLabelStyleSheet(QString iconUrl);
    static QString getWindowButtonStyleSheet(QString iconUrl, Color baseColor, Color hoverColor, Color pressedColor);
    static QString getQuickPanelButtonStyleSheet(QString iconUrl, Color baseColor, Color hoverColor, Color pressedColor);
    // Специальные таблицы стилей для базовых элементов
    static QString getInputFieldStyleSheet (Color color);
    static QString getConsoleStyleSheet(Color color);
    // Общая таблица стилей элементов
    static QString getScrollBarStyleSheet (Color handleColor, Color pageColor);
    static QString getTableStyleSheet(Color tableColor, Color headerColor);
    static QString getSipmleWidgetStyleSheet(Color color);
    static QString getComboBoxStyleSheet(QString &arrowUrl,Color activeColor, Color disableColor, Color listColor);
    static QString getCheckBoxStyleSheet(QString &checkUrl, Color color, Color hoverColor, Color pressedColor);
    static QString getPlainTextStyleSheet(Color color);
    static QString getButtonStyleSheet(Color baseColor, Color hoverColor, Color pressedColor,
                                       int horizontalPadding, int verticalPadding,
                                       int leftUpperRadius,   int leftBottomRadius,
                                       int rightUpperRadius,  int rightBottomRadius);

private:
    QString _name;                    // Имя цветовой схемы
    IconMode _iconMode;               // Цвет иконок
    Color _baseColor;                 // Базовый цвет
    Color _secondColor;               // Дополнительный цвет
    Color _thirdColor;                // Дополнительный цвет 2
    Color _closeHoverColor;           // Цвет кнопки "закрыть" при наведении
    Color _closePressedColor;         // Цвет кнопки "закрыть" при нажатии
    Color _maximizeHoverColor;        // Цвет кнопки "развернуть" при наведении
    Color _maximizePressedColor;      // Цвет кнопки "развернуть" при нажатии
    Color _minimizeHoverColor;        // Цвет кнопки "свернуть" при наведении
    Color _minimizePressedColor;      // Цвет кнопки "свернуть" при нажатии
    QString appIconlUrl;              // Путь к иконке-заголовку
    QString closeIconUrl;             // Путь к иконке "закрыть"
    QString maximizeIconUrl;          // Путь к иконке "развернуть"
    QString minimizeIconUrl;          // Путь к иконке "свернуть"
    QString connectionIconUrl;        // Путь к иконке "подключение"
    QString consoleIconUrl;           // Путь к иконке "консоль"
    QString tableIconUrl;             // Путь к иконке "таблица"
    QString converterIconUrl;         // Путь к иконке "конвертер"
    QString settingsIconUrl;          // Путь к иконке "настройки"
    QString comboBoxArrowUrl;         // Путь к иконке стрелки комбобокса
    QString checkBoxCheckUrl;         // Путь к иконки отметки чекбокса

    static Ui::MainWidget *gui;       // Указатель на целевой виджет gui

    static void validateColor(Color &color);
    static QString colorToString(Color color);

    void applyToMainWidget(void);
    void applyToConsole(void);
    void applyToTable(void);
    void applyToConverter(void);
    void applyToSettings(void);

signals:
    void baseColorChanged(void);
    void secondColorChanged(void);
    void thirdColorChanged(void);
    void textColorChanged(void);
    void selectionColorChanged(void);
    void selectionTextColorChanged(void);
    void closeHoverColorChanged(void);
    void closePressedColorChanged(void);
    void maximizeHoverColorChanged(void);
    void maximizePressedColorChanged(void);
    void minimizeHoverColorChanged(void);
    void minimizePressedColorChanged(void);
    void nameChanged(void);
    void iconModeChanged(void);
};

Q_DECLARE_METATYPE(Decorator)

#endif // DECORATOR_H


