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

    //static QString convertHexToAscii (QByteArray &source);

    static QByteArray parseStringForHex(QString &string);

    static QString convertHexToAscii (QString &source);
    static QString convertHexToUint8 (QString &source);
    static QString convertHexToInt8 (QString &source);
    static QString convertHexToUint16 (QString &source);
    static QString convertHexToInt16 (QString &source);
    static QString convertHexToUint32 (QString &source);
    static QString convertHexToInt32 (QString &source);
    static QString convertHexToUint64 (QString &source);
    static QString convertHexToInt64 (QString &source);
    static QString convertHexToFloat (QString &source);
    static QString convertHexToDouble (QString &source);


    static void swapEndian(uint8_t &value);
    static void swapEndian(int8_t &value);
    static void swapEndian(uint16_t &value);
    static void swapEndian(int16_t &value);
    static void swapEndian(uint32_t &value);
    static void swapEndian(int32_t &value);
    static void swapEndian(uint64_t &value);
    static void swapEndian(int64_t &value);
    static void swapEndian(float &value);
    static void swapEndian(double &value);

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

    enum ConvertTypes {
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

    QString expandToFullType(QString &source, ConvertTypes type);
signals:

public slots:
    void convert();
    void swap();
    void clear();

private slots:
    void resultTypeChanges (void);
    void validateSource (void);
};

#endif // CONVERTER_H
