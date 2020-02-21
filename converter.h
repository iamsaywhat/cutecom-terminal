#ifndef CONVERTER_H
#define CONVERTER_H

#include <QObject>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QRegularExpressionValidator>
#include <QRegularExpression>

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
    // Группировка символов в строке по groupSize штук, разделителем delimiter
    static void setDelimitersInHexString(QString &currentText, int groupSize, char delimiter);
    static void setDelimitersInHexString(QPlainTextEdit *textEdit, int groupSize, char delimiter);
    // Распарсить стоку содержащую HEX коды с разделителем
    static QByteArray parseStringForHex(bool *status, QString &string, char delimiter);
    // Преобразование из HEX
    static QString convertHexToAscii  (QString &source);
    static QString convertHexToUint8  (QString &source);
    static QString convertHexToInt8   (QString &source);
    static QString convertHexToUint16 (QString &source);
    static QString convertHexToInt16  (QString &source);
    static QString convertHexToUint32 (QString &source);
    static QString convertHexToInt32  (QString &source);
    static QString convertHexToUint64 (QString &source);
    static QString convertHexToInt64  (QString &source);
    static QString convertHexToFloat  (QString &source);
    static QString convertHexToDouble (QString &source);
    // Преобразования в HEX
    static QString convertAsciiToHex  (QString &source);
    static QString convertUint8ToHex  (QString &source);
    static QString convertInt8ToHex   (QString &source);
    static QString convertUint16ToHex (QString &source);
    static QString convertInt16ToHex  (QString &source);
    static QString convertUint32ToHex (QString &source);
    static QString convertInt32ToHex  (QString &source);
    static QString convertUint64ToHex (QString &source);
    static QString convertInt64ToHex  (QString &source);
    static QString convertFloatToHex  (QString &source);
    static QString convertDoubleToHex (QString &source);
    // Изменения порядка следования байт на обратный
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
public slots:
    void convert();
    void swap();
    void clear();
private:
    // Указатели на ключеные элементы gui
    QPlainTextEdit  *_source;
    QPlainTextEdit  *_result;
    QPushButton     *_convertButton;
    QPushButton     *_swapButton;
    QPushButton     *_clearButton;
    QComboBox       *_sourceBox;
    QComboBox       *_resultBox;
    // Используемые регулярные выражения и валидатор
    QRegularExpression *hexArrayRegEx        = new QRegularExpression ("[0-9A-Fa-f ]+");
    QRegularExpression *hex8ByteRegEx        = new QRegularExpression ("[0-9A-Fa-f ]{23}");
    QRegularExpression *hex4ByteRegEx        = new QRegularExpression ("[0-9A-Fa-f ]{11}");
    QRegularExpression *hex2ByteRegEx        = new QRegularExpression ("[0-9A-Fa-f ]{5}");
    QRegularExpression *hex1ByteRegEx        = new QRegularExpression ("[0-9A-Fa-f ]{2}");
    QRegularExpression *asciiRegEx           = new QRegularExpression ("[\\x00-\\xFF]+"); // Обычный до 7F, расширенная IBM cp866 до 77
    QRegularExpression *floatingPointRegExp  = new QRegularExpression ("^-?(?:0|[1-9][0-9]*)(?:\\.[0-9]+)?(?:e[+-]?[1-9][0-9]+)?$");
    QRegularExpression *unsignedIntegerRegEx = new QRegularExpression ("^[1-9][0-9]+$");
    QRegularExpression *signedIntegerRegEx   = new QRegularExpression ("^-?[1-9][0-9]+$");
    QRegularExpressionValidator *validator   = new QRegularExpressionValidator;
    // Все используемые форматы преобразований
    enum TypeName {
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
    // Уникальные ID переобразования
    enum ConversionType {
        // Прямые типы преобразований
        HexToAscii  = 0,
        HexToUint8  = 1,
        HexToInt8   = 2,
        HexToUint16 = 3,
        HexToInt16  = 4,
        HexToUint32 = 5,
        HexToInt32  = 6,
        HexToUint64 = 7,
        HexToInt64  = 8,
        HexToFloat  = 9,
        HexToDouble = 10,
        // Обратные типы преобразований
        AsciiToHex  = 11,
        Uint8ToHex  = 12,
        Int8ToHex   = 13,
        Uint16ToHex = 14,
        Int16ToHex  = 15,
        Uint32ToHex = 16,
        Int32ToHex  = 17,
        Uint64ToHex = 18,
        Int64ToHex  = 19,
        FloatToHex  = 20,
        DoubleToHex = 21,
        // Неопределенный тип
        Unknown     = 255
    };
    enum ConversionDirect{
        DIRECT,
        INDIRECT
    };
    // Состояния конвертера
    ConversionDirect direction;     // Текущее направление преобразования
    ConversionType   convertionId;  // Текущий ID преобразования
    // Дополняет строку с HEX кодами нулями до полного формата
    static QString expandToFullType(QString &source, TypeName type);
    // Заполнение combobox'ов доступными конвертируемыми типами
    void fillComboBoxs (QComboBox* comboBox1, QComboBox* comboBox2);
    // Обновление ID текущего преобразования
    void updateConversionType(void);
    // Валидация вводимого текста
    void validateHexInput(void);
    void validateAsciiInput(void);
    void validateFloatingPointInput(void);
    void validateIntegerInput();
private slots:
    // Слот отрабатывающий изменения в combobox'ах
    void resultTypeChanges (void);
    // Слот валидации вводимого текста
    void validateSource (void);
};

#endif // CONVERTER_H
