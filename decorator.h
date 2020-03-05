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

    void applyToCloseButton(QToolButton *button);
    void applyToMaximizeButton(QToolButton *button);
    void applyToMinimizeButton(QToolButton *button);
    void applyToConnectionButton(QPushButton *button);
    void applyToConsoleButton(QPushButton *button);
    void applyToTableButton(QPushButton *button);
    void applyToConverterButton(QPushButton *button);
    void applyToSettingsButton(QPushButton *button);
    void applyToAppLabel(QLabel *label);
    void applyToConsoleWidget(QPlainTextEdit *display, QLineEdit *input, QPushButton *sendButton, QPushButton *clearButton);
    void applyToTableWidget(QTableView *taleView, QLineEdit *lineEdit, QPushButton *sendButton, QPushButton *clearButton);
    void applyToConverterWidget(QPlainTextEdit*, QPlainTextEdit*, QComboBox*, QComboBox*, QPushButton*, QPushButton*, QPushButton*);

    void setBasicColorsToWidget(QWidget *widget, Color color);
    void setScrollBarColors(QScrollBar *scrollBar, Color handleColor, Color pageColor);
    void setSettingsButtonsColors(QPushButton *button, Color background, Color backgroundHover, Color backgroundPressed);
    void setComboBoxColors(QComboBox *comboBox);
    void setStandartButtonColors(QPushButton *button, Color baseColor, Color hoverColor, Color pressedColor);
    void setCheckBoxColors(QCheckBox *checkBox, Color color, Color hoverColor, Color pressedColor);


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

    static void validateColor(Color &color);
    static QString colorToString(Color color);

    static QString getAppLabelStyleSheet(QString iconUrl);
    static QString getMainWidgetStyleSheet(Color color);
    static QString getWindowButtonStyleSheet(QString iconUrl, Color baseColor, Color hoverColor, Color pressedColor);
    static QString getStandartButtonStyleSheet (Color baseColor, Color hoverColor, Color pressedColor);
    static QString getInputFieldStyleSheet (Color color);
    static QString getScrollBarStyleSheet (Color handleColor, Color pageColor);
    static QString getConsoleStyleSheet(Color color);
    static QString getQuickPanelButtonStyleSheet(QString iconUrl, Color baseColor, Color hoverColor, Color pressedColor);
    static QString getTableStyleSheet(Color tableColor, Color headerColor);
    static QString getSipmleWidgetStyleSheet(Color color);
    static QString getSettingsMenuButtonStyleSheet(Color baseColor,Color hoverColor, Color pressedColor);
    static QString getComboBoxStyleSheet(QString &arrowUrl,Color activeColor, Color disableColor, Color listColor);
    static QString getCheckBoxStyleSheet(QString &checkUrl, Color color, Color hoverColor, Color pressedColor);

private:
    QString _name;
    IconMode _iconMode;
    Color _baseColor;
    Color _secondColor;
    Color _thirdColor;
    Color _closeHoverColor;
    Color _closePressedColor;
    Color _maximizeHoverColor;
    Color _maximizePressedColor;
    Color _minimizeHoverColor;
    Color _minimizePressedColor;
    QString appIconlUrl;
    QString closeIconUrl;
    QString maximizeIconUrl;
    QString minimizeIconUrl;
    QString connectionIconUrl;
    QString consoleIconUrl;
    QString tableIconUrl;
    QString converterIconUrl;
    QString settingsIconUrl;
    QString comboBoxArrowUrl;
    QString checkBoxCheckUrl;

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


