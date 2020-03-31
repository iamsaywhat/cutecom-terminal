#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QTime>
#include <QDate>
#include <QDebug>
#include "serialgui.h"

class Logger : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)

public:
    explicit Logger(QObject *parent = nullptr,
                    SerialGui *port = nullptr);
    ~Logger(void);
    bool enabled(void);
    QString path(void) const;

private:
    QString _path;
    bool _enabled = true;
    QFile *file = nullptr;
    SerialGui *_port = nullptr;

signals:
    void enabledChanged(bool);
    void pathChanged(QString&);

public slots:
    void setEnabled(bool);
    void setPath(const QString &path);
    void incoming(QByteArray);
    void outgoing(QByteArray);
    void openFile(void);
    void closeFile(void);

};

#endif // LOGGER_H
