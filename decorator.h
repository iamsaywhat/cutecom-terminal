#ifndef DECORATOR_H
#define DECORATOR_H

#include <QString>

class Decorator
{
public:
    Decorator();

    static QString getMainWidgetStyleSheet(void);
    static QString getCloseButtonStyleSheet(void);
    static QString getMaximizeButtonStyleSheet(void);
    static QString getRestoreButtonStyleSheet(void);
    static QString getMinimizeButtonStyleSheet(void);
    static QString getConnectionButtonStyleSheet(void);
    static QString getSettingsButtonStyleSheet(void);
    static QString getConsoleStyleSheet(void);
    static QString getUsualButtonStyleSheet(void);
    static QString getInputFieldStyleSheet (void);
    static QString getScrollBarStyleSheet (void);
    static QString getComboBoxStyleSheet(void);


    static QString get_ConnectionButtonStyleSheet(void);
    static QString get_SettingsButtonStyleSheet(void);
    static QString getConsoleModeButtonStyleSheet(void);
    static QString getTableModeButtonStyleSheet(void);
    static QString getConverterModeButtonStyleSheet(void);
};

#endif // DECORATOR_H
