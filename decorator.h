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


class Decorator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int baseColor            READ baseColor            WRITE setBaseColor            NOTIFY baseColorChanged)
    Q_PROPERTY(int secondColor          READ secondColor          WRITE setSecondColor          NOTIFY secondColorChanged)
    Q_PROPERTY(int thirdColor           READ thirdColor           WRITE setThirdColor           NOTIFY thirdColorChanged)
    Q_PROPERTY(int textColor            READ textColor            WRITE setTextColor            NOTIFY textColorChanged)
    Q_PROPERTY(int selectionColor       READ selectionColor       WRITE setSelectionColor       NOTIFY selectionColorChanged)
    Q_PROPERTY(int selectionTextColor   READ selectionTextColor   WRITE setSelectionTextColor   NOTIFY selectionTextColorChanged)
    Q_PROPERTY(int closeHoverColor      READ closeHoverColor      WRITE setCloseHoverColor      NOTIFY closeHoverColorChanged)
    Q_PROPERTY(int closePressedColor    READ closePressedColor    WRITE setClosePressedColor    NOTIFY closePressedColorChanged)
    Q_PROPERTY(int maximizeHoverColor   READ maximizeHoverColor   WRITE setMaximizeHoverColor   NOTIFY maximizeHoverColorChanged)
    Q_PROPERTY(int maximizePressedColor READ maximizePressedColor WRITE setMaximizePressedColor NOTIFY maximizePressedColorChanged)
    Q_PROPERTY(int minimizeHoverColor   READ minimizeHoverColor   WRITE setMinimizeHoverColor   NOTIFY minimizeHoverColorChanged)
    Q_PROPERTY(int minimizePressedColor READ minimizePressedColor WRITE setMinimizePressedColor NOTIFY minimizePressedColorChanged)

public:
    Decorator(int base, int second, int third, int text, int selection);
    ~Decorator();

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

    void setBasicColorsToWidget(QWidget *widget, int backgroundColor, int textColor);
    void setScrollBarColors(QScrollBar *scrollBar, int handleColor, int pageColor);
    void setSettingsButtonsColors(QPushButton *button, int background, int backgroundHover, int backgroundPressed);
    void setComboBoxColors(QComboBox *comboBox);
    void setStandartButtonColors(QPushButton *button, int backgroundColor, int textColor, int hoverBackgroundColor, int pressedBackgroundColor);

    int baseColor(void)            {return _baseColor;}
    int secondColor(void)          {return _secondColor;}
    int thirdColor(void)           {return _thirdColor;}
    int textColor(void)            {return _textColor;}
    int selectionColor(void)       {return _selectionColor;}
    int selectionTextColor(void)   {return _selectionTextColor;}
    int closeHoverColor(void)      {return _closeHoverColor;}
    int closePressedColor(void)    {return _closePressedColor;}
    int maximizeHoverColor(void)   {return _maximizeHoverColor;}
    int maximizePressedColor(void) {return _maximizePressedColor;}
    int minimizeHoverColor(void)   {return _minimizeHoverColor;}
    int minimizePressedColor(void) {return _minimizePressedColor;}

    void setBaseColor(int value)            {_baseColor = 0xffffff&value;}
    void setSecondColor(int value)          {_secondColor = 0xffffff&value;}
    void setThirdColor(int value)           {_thirdColor = 0xffffff&value;}
    void setTextColor(int value)            {_textColor = 0xffffff&value;}
    void setSelectionColor(int value)       {_selectionColor = 0xffffff&value;}
    void setSelectionTextColor(int value)   {_selectionTextColor = 0xffffff&value;}
    void setCloseHoverColor(int value)      {_closeHoverColor = 0xffffff&value;}
    void setClosePressedColor(int value)    {_closePressedColor = 0xffffff&value;}
    void setMaximizeHoverColor(int value)   {_maximizeHoverColor = 0xffffff&value;}
    void setMaximizePressedColor(int value) {_maximizePressedColor = 0xffffff&value;}
    void setMinimizeHoverColor(int value)   {_minimizeHoverColor = 0xffffff&value;}
    void setMinimizePressedColor(int value) {_minimizePressedColor = 0xffffff&value;}

private:
    int _baseColor;
    int _secondColor;
    int _thirdColor;
    int _textColor;
    int _selectionColor;
    int _selectionTextColor;
    int _closeHoverColor;
    int _closePressedColor;
    int _maximizeHoverColor;
    int _maximizePressedColor;
    int _minimizeHoverColor;
    int _minimizePressedColor;
    QString *appIconlUrl;
    QString *closeIconUrl;
    QString *maximizeIconUrl;
    QString *minimizeIconUrl;
    QString *connectionIconUrl;
    QString *consoleIconUrl;
    QString *tableIconUrl;
    QString *converterIconUrl;
    QString *settingsIconUrl;

    static QString getAppLabelStyleSheet          (QString iconUrl);
    static QString getTableStyleSheet             (int backgroundColor,
                                                   int textColor,
                                                   int selectionBackgroundColor,
                                                   int selectionTextColor,
                                                   int headerBackgroundColor);
    static QString getConsoleStyleSheet           (int backgroundColor,
                                                   int textColor,
                                                   int selectionBackgroundColor,
                                                   int selectionTextColor);
    static QString getQuickPanelButtonStyleSheet  (QString iconUrl,
                                                   int backgroundColor,
                                                   int hoverBackgroundColor,
                                                   int pressedBackgroundColor);
    static QString getMainWidgetStyleSheet        (int backgroundColor,
                                                   int textColor);
    static QString getWindowButtonStyleSheet      (QString iconUrl,
                                                   int backgroundColor,
                                                   int hoverBackgroundColor,
                                                   int pressedBackgroundColor);
    static QString getStandartButtonStyleSheet    (int backgroundColor,
                                                   int textColor,
                                                   int hoverBackgroundColor,
                                                   int pressedBackgroundColor);
    static QString getInputFieldStyleSheet        (int backgroundColor,
                                                   int textColor,
                                                   int selectionBackgroundColor,
                                                   int selectionTextColor);
    static QString getScrollBarStyleSheet         (int handleColor,
                                                   int pageColor);
    static QString getSipmleWidgetStyleSheet      (int backgroundColor,
                                                   int textColor);
    static QString getSettingsMenuButtonStyleSheet(int background,
                                                   int backgroundHover,
                                                   int backgroundPressed);
    static QString getComboBoxStyleSheet          (int background,
                                                   int color,
                                                   int selectionBackgroundColor,
                                                   int selectionColor);
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
};

#endif // DECORATOR_H
