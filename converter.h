#ifndef CONVERTER_H
#define CONVERTER_H

#include <QObject>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QRegularExpressionValidator>
#include <QRegularExpression>

#include <QLineEdit>

class Converter : public QObject
{
    Q_OBJECT
public:
    explicit Converter(QObject        *parent,
                       QPlainTextEdit *source,
                       QPlainTextEdit *result,
                       QPushButton    *convertButton,
                       QPushButton    *swapButton,
                       QPushButton    *clearButton,
                       QComboBox      *sourceBox,
                       QComboBox      *resultBox);
    ~Converter();
    static void setDelimitersInHexString(QPlainTextEdit *textEdit);

private:
    QPlainTextEdit  *_source;
    QPlainTextEdit  *_result;
    QPushButton     *_convertButton;
    QPushButton     *_swapButton;
    QPushButton     *_clearButton;
    QComboBox       *_sourceBox;
    QComboBox       *_resultBox;

    QRegularExpression *hexArray   = new QRegularExpression ("[0-9A-Fa-f ]+");
    QRegularExpression *hex8Byte   = new QRegularExpression ("[0-9A-Fa-f ]{23}");
    QRegularExpression *hex4Byte   = new QRegularExpression ("[0-9A-Fa-f ]{11}");
    QRegularExpression *hex2Byte   = new QRegularExpression ("[0-9A-Fa-f ]{5}");
    QRegularExpression *hex1Byte   = new QRegularExpression ("[0-9A-Fa-f ]{2}");

    QRegularExpressionValidator *validator = new QRegularExpressionValidator;

    enum convertTypes {
        HEX    = 0,
        ASCII  = 1,
        UINT8  = 2,
        INT8   = 3,
        UINT16 = 4,
        INT16  = 5,
        UINT32 = 6,
        INT32  = 7,
        UINT64 = 8,
        INT64  = 9,
        FLOAT  = 10,
        DOUBLE = 11,
    };

signals:

public slots:

private slots:
    void resultTypeChanges (void);
    void validateSource (void);
};

#endif // CONVERTER_H
