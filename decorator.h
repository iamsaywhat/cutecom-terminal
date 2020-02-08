#ifndef DECORATOR_H
#define DECORATOR_H

#include <QString>

class Decorator
{
public:
    Decorator();

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
    static QString getMainWidgetStyleSheet        (int backgroundColor, int textColor);
    static QString getWindowButtonStyleSheet      (QString iconUrl,
                                                   int backgroundColor,
                                                   int hoverBackgroundColor,
                                                   int pressedBackgroundColor);
    static QString getUsualButtonStyleSheet       (int backgroundColor,
                                                   int textColor,
                                                   int hoverBackgroundColor,
                                                   int pressedBackgroundColor);
    static QString getInputFieldStyleSheet        (int backgroundColor,
                                                   int textColor,
                                                   int selectionBackgroundColor,
                                                   int selectionTextColor);
    static QString getScrollBarStyleSheet         (int handleColor, int pageColor);
    static QString getSipmleWidgetStyleSheet      (int backgroundColor,
                                                   int textColor);
    static QString getSettingsMenuButtonStyleSheet(int background, int backgroundHover, int backgroundPressed);
    static QString getComboBoxStyleSheet          (int background,
                                                   int color,
                                                   int selectionBackgroundColor,
                                                   int selectionColor);
};

#endif // DECORATOR_H
