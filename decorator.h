#ifndef DECORATOR_H
#define DECORATOR_H

#include <QString>

class Decorator
{
public:
    Decorator();

    static QString getMainWidgetStyleSheet();
    static QString getCloseButtonStyleSheet();
    static QString getMaximizeButtonStyleSheet();
    static QString getRestoreButtonStyleSheet();
    static QString getMinimizeButtonStyleSheet();
    static QString getConnectionButtonStyleSheet();
    static QString getSettingsButtonStyleSheet();
    static QString getConsoleStyleSheet();
};

#endif // DECORATOR_H
