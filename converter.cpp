#include "converter.h"
#include <QtEndian>

#define DEBUG

#ifdef DEBUG
    #include <QDebug>
#endif

Converter::Converter(QObject        *parent,
                     QPlainTextEdit *source,
                     QPlainTextEdit *result,
                     QPushButton    *convertButton,
                     QPushButton    *swapButton,
                     QPushButton    *clearButton,
                     QComboBox      *sourceBox,
                     QComboBox      *resultBox) : QObject(parent){
    _source = source;
    _result = result;
    _convertButton = convertButton;
    _swapButton = swapButton;
    _clearButton = clearButton;
    _sourceBox = sourceBox;
    _resultBox = resultBox;

    result->setReadOnly(true);

    fillComboBoxs(sourceBox, resultBox);

    convertButton->setText("Convert");
    swapButton->setText("Swap");
    clearButton->setText("Clear");

    connect(resultBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Converter::resultTypeChanges);
    connect(sourceBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Converter::resultTypeChanges);
    connect(source, &QPlainTextEdit::textChanged, this,  &Converter::validateSource);
    connect(convertButton, &QPushButton::clicked, this, &Converter::convert);
    connect(swapButton, &QPushButton::clicked, this, &Converter::swap);
    connect(clearButton, &QPushButton::clicked, this, &Converter::clear);


    direction = DIRECT;
    updateConversionType();
    validator->setRegularExpression(*hexArray);

    validator->setRegularExpression(*doublef);
    validator->setRegularExpression(*asciiSyms);

//    QByteArray array;
//    array[0] = 0x40;
//    array[1] = 0x23;
//    array[2] = 0xf4;
//    array[3] = 0x75;
//    array[4] = 0x2c;
//    array[5] = 0x74;
//    array[6] = 0x49;
//    array[7] = 0xa0;
//    QByteArray *ptt = &array;
//    qDebug() << array << array.data();
//    qDebug() << *((unsigned short *)(ptt->data()+1));
//    qDebug() << *(reinterpret_cast<unsigned short *>(ptt->data()));

//    qint64 swaped = qFromBigEndian<qint64>(*(reinterpret_cast<quint64 *>(ptt->data())));
//    qint64 notSwaped = (*(reinterpret_cast<quint64 *>(ptt->data())));
//    qDebug() << "swaped" << swaped;
//    qDebug() << "not swaped" << notSwaped;
//    qDebug() << array.toDouble();


//    qDebug() << "test function:";
//    double doubleValue = 9.977456464;
//    float floatValue = -25.6697;
//    uint64_t uint64Value = 9819329691267507320;// 0x8845445444722478;
//    int64_t int64Value = 0x8845445444722478;
//    uint32_t uint32Value = 0x44722478;
//    int32_t int32Value = 0x44722478;
//    uint16_t uint16Value = 0x2478;
//    int16_t int16Value = 0x2478;
//    uint8_t uint8Value = 0x78;
//    int8_t int8Value = 0x78;

//    swapEndian(doubleValue);
//    swapEndian(floatValue);
//    swapEndian(uint64Value);
//    swapEndian(int64Value);
//    swapEndian(uint32Value);
//    swapEndian(int32Value);
//    swapEndian(uint16Value);
//    swapEndian(int16Value);
//    swapEndian(uint8Value);
//    swapEndian(int8Value);

//    qDebug()<< "double" << hex<< doubleValue;
//    qDebug()<< "float" << hex<< floatValue;
//    qDebug()<< "uint64" << hex<< uint64Value;
//    qDebug()<< "int64" << hex<< int64Value;
//    qDebug()<< "uint32" << hex<< uint32Value;
//    qDebug()<< "int32" << hex<< int32Value;
//    qDebug()<< "uint16" << hex<< uint16Value;
//    qDebug()<< "int16" << hex<< int16Value;
//    qDebug()<< "uint8" << hex<< uint8Value;
//    qDebug()<< "int8" << hex << int8Value;

//    swapEndian(doubleValue);
//    swapEndian(floatValue);
//    swapEndian(uint64Value);
//    swapEndian(int64Value);
//    swapEndian(uint32Value);
//    swapEndian(int32Value);
//    swapEndian(uint16Value);
//    swapEndian(int16Value);
//    swapEndian(uint8Value);
//    swapEndian(int8Value);

//    qDebug()<< "double" << hex<< doubleValue;
//    qDebug()<< "float" << hex<< floatValue;
//    qDebug()<< "uint64" << uint64Value;
//    qDebug()<< "int64" << int64Value;
//    qDebug()<< "uint32" << hex<< uint32Value;
//    qDebug()<< "int32" << hex<< int32Value;
//    qDebug()<< "uint16" << hex<< uint16Value;
//    qDebug()<< "int16" << hex<< int16Value;
//    qDebug()<< "uint8" << hex<< uint8Value;
//    qDebug()<< "int8" << hex << int8Value;
    QString str("Hello world!");
    qDebug() << "like string" << str;
    qDebug() << "like byteArray" << str.toLatin1();
    QByteArray array = str.toLatin1();
    qDebug() << "converted" << convertAsciiToHex(array);
}

Converter::~Converter(void) {
    delete hexArray;
    delete hex8Byte;
    delete hex4Byte;
    delete hex2Byte;
    delete hex1Byte;
    delete validator;
}

void Converter::fillComboBoxs (QComboBox* comboBox1, QComboBox* comboBox2){
    comboBox1->clear();
    comboBox2->clear();

    comboBox1->addItem("hex", HEX);

    comboBox2->addItem("ascii",  ASCII);
    comboBox2->addItem("uint8",  UINT8);
    comboBox2->addItem("int8",   INT8);
    comboBox2->addItem("uint16", UINT16);
    comboBox2->addItem("int16",  INT16);
    comboBox2->addItem("uint32", UINT32);
    comboBox2->addItem("int32",  INT32);
    comboBox2->addItem("uint64", UINT64);
    comboBox2->addItem("int64",  INT64);
    comboBox2->addItem("float",  FLOAT);
    comboBox2->addItem("double", DOUBLE);
}

void Converter::updateConversionType(){
    // Прямое направление преобзования
    if(direction == DIRECT)
    {
        if(_sourceBox->currentData() == HEX)
        {
            if(_resultBox->currentData() == ASCII)
            {
                convertionId = hexToAscii;
            }
            else if (_resultBox->currentData() == UINT8)
            {
                convertionId = hexToUint8;
            }
            else if (_resultBox->currentData() == INT8)
            {
                convertionId = hexToInt8;
            }
            else if (_resultBox->currentData() == UINT16)
            {
                convertionId = hexToUint16;
            }
            else if (_resultBox->currentData() == INT16)
            {
                convertionId = hexToInt16;
            }
            else if (_resultBox->currentData() == UINT32)
            {
                convertionId = hexToUint32;
            }
            else if (_resultBox->currentData() == INT32)
            {
                convertionId = hexToInt32;
            }
            else if (_resultBox->currentData() == UINT64)
            {
                convertionId = hexToUint64;
            }
            else if (_resultBox->currentData() == INT64)
            {
                convertionId = hexToInt64;
            }
            else if (_resultBox->currentData() == FLOAT)
            {
                convertionId = hexToFloat;
            }
            else if (_resultBox->currentData() == DOUBLE)
            {
                convertionId = hexToDouble;
            }
            else
                convertionId = Unknown;
        }
    }
    // Обратный тип преобразований
    else
    {
        if(_resultBox->currentData() == HEX)
        {
            // Прямое направление преобзования
            if(_sourceBox->currentData() == ASCII)
            {
                convertionId = AsciiToHex;
            }
            else if (_sourceBox->currentData() == UINT8)
            {
                convertionId = Uint8ToHex;
            }
            else if (_sourceBox->currentData() == INT8)
            {
                convertionId = Int8ToHex;
            }
            else if (_sourceBox->currentData() == UINT16)
            {
                convertionId = Uint16ToHex;
            }
            else if (_sourceBox->currentData() == INT16)
            {
                convertionId = Int16ToHex;
            }
            else if (_sourceBox->currentData() == UINT32)
            {
                convertionId = Uint32ToHex;
            }
            else if (_sourceBox->currentData() == INT32)
            {
                convertionId = Int32ToHex;
            }
            else if (_sourceBox->currentData() == UINT64)
            {
                convertionId = Uint64ToHex;
            }
            else if (_sourceBox->currentData() == INT64)
            {
                convertionId = Int64ToHex;
            }
            else if (_sourceBox->currentData() == FLOAT)
            {
                convertionId = FloatToHex;
            }
            else if (_sourceBox->currentData() == DOUBLE)
            {
                convertionId = DoubleToHex;
            }
            else
                convertionId = Unknown;
        }
    }
}

void Converter::convert(void){
    QString sourceData(_source->toPlainText());
    QString resultData;
    switch (convertionId) {
        case hexToAscii:
        {
            resultData = convertHexToAscii(sourceData);
            break;
        }
        case hexToUint8:
        {
            _source->setPlainText(expandToFullType(sourceData, UINT8));
            resultData = convertHexToUint8(sourceData);
            break;
        }
        case hexToInt8:
        {
            _source->setPlainText(expandToFullType(sourceData, INT8));
            resultData = convertHexToInt8(sourceData);
            break;
        }
        case hexToUint16:
        {
            _source->setPlainText(expandToFullType(sourceData, UINT16));
            resultData = convertHexToUint16(sourceData);
            break;
        }
        case hexToInt16:
        {
            _source->setPlainText(expandToFullType(sourceData, INT16));
            resultData = convertHexToInt16(sourceData);
            break;
        }
        case hexToUint32:
        {
            _source->setPlainText(expandToFullType(sourceData, UINT32));
            resultData = convertHexToUint32(sourceData);
            break;
        }
        case hexToInt32:
        {
            _source->setPlainText(expandToFullType(sourceData, INT32));
            resultData = convertHexToInt32(sourceData);
            break;
        }
        case hexToUint64:
        {
            _source->setPlainText(expandToFullType(sourceData, UINT64));
            resultData = convertHexToUint64(sourceData);
            break;
        }
        case hexToInt64:
        {
            _source->setPlainText(expandToFullType(sourceData, INT64));
            resultData = convertHexToInt64(sourceData);
            break;
        }
        case hexToFloat:
        {
            _source->setPlainText(expandToFullType(sourceData, FLOAT));
            resultData = convertHexToFloat(sourceData);
            break;
        }
        case hexToDouble:
        {
            _source->setPlainText(expandToFullType(sourceData, DOUBLE));
            resultData = convertHexToDouble(sourceData);
            break;
        }
        case AsciiToHex:
        {
            resultData = convertAsciiToHex(sourceData);
            break;
        }
        case Uint8ToHex:
        {
            break;
        }
        case Int8ToHex:
        {
            break;
        }
        case Uint16ToHex:
        {
            break;
        }
        case Int16ToHex:
        {
            break;
        }
        case Uint32ToHex:
        {
            break;
        }
        case Int32ToHex:
        {
            break;
        }
        case Uint64ToHex:
        {
            break;
        }
        case Int64ToHex:
        {
            break;
        }
        case FloatToHex:
        {
            break;
        }
        case DoubleToHex:
        {
            break;
        }
        default:
        {
            resultData = "";
            break;
        }
    }

    // Результат в правое окно
    _result->setPlainText(resultData.toLatin1());
}

void Converter::swap(){
    if(direction == DIRECT)
    {
        direction = INDIRECT;
        fillComboBoxs(_resultBox, _sourceBox);
    }
    else
    {
        direction = DIRECT;
        fillComboBoxs(_sourceBox, _resultBox);
    }
    updateConversionType();
    QString temp = _source->toPlainText();
    _source->setPlainText(_result->toPlainText());
    _result->setPlainText(temp);
}

void Converter::clear(){
    _source->clear();
    _result->clear();
}

void Converter::resultTypeChanges (void){
    updateConversionType();
    switch (convertionId) {
        case hexToAscii:
        {
            validator->setRegularExpression(*hexArray);
            break;
        }
        case hexToUint8:
        {
            QString temp = _source->toPlainText();
            temp.chop(temp.count()-2);
            _source->setPlainText(temp);
            validator->setRegularExpression(*hex1Byte);
            break;
        }
        case hexToInt8:
        {
            QString temp = _source->toPlainText();
            temp.chop(temp.count()-2);
            _source->setPlainText(temp);
            validator->setRegularExpression(*hex1Byte);
            break;
        }
        case hexToUint16:
        {
            QString temp = _source->toPlainText();
            temp.chop(temp.count()-5);
            _source->setPlainText(temp);
            validator->setRegularExpression(*hex2Byte);
            break;
        }
        case hexToInt16:
        {
            QString temp = _source->toPlainText();
            temp.chop(temp.count()-5);
            _source->setPlainText(temp);
            validator->setRegularExpression(*hex2Byte);
            break;
        }
        case hexToUint32:
        {
            QString temp = _source->toPlainText();
            temp.chop(temp.count()-11);
            _source->setPlainText(temp);
            validator->setRegularExpression(*hex4Byte);
            break;
        }
        case hexToInt32:
        {
            QString temp = _source->toPlainText();
            temp.chop(temp.count()-11);
            _source->setPlainText(temp);
            validator->setRegularExpression(*hex4Byte);
            break;
        }
        case hexToUint64:
        {
            QString temp = _source->toPlainText();
            temp.chop(temp.count()-23);
            _source->setPlainText(temp);
            validator->setRegularExpression(*hex8Byte);
            break;
        }
        case hexToInt64:
        {
            QString temp = _source->toPlainText();
            temp.chop(temp.count()-23);
            _source->setPlainText(temp);
            validator->setRegularExpression(*hex8Byte);
            break;
        }
        case hexToFloat:
        {
            QString temp = _source->toPlainText();
            temp.chop(temp.count()-11);
            _source->setPlainText(temp);
            validator->setRegularExpression(*hex4Byte);
            break;
        }
        case hexToDouble:
        {
            QString temp = _source->toPlainText();
            temp.chop(temp.count()-23);
            _source->setPlainText(temp);
            validator->setRegularExpression(*hex8Byte);
            break;
        }
        case AsciiToHex:
        {

            break;
        }
        case Uint8ToHex:
        {
            break;
        }
        case Int8ToHex:
        {
            break;
        }
        case Uint16ToHex:
        {
            break;
        }
        case Int16ToHex:
        {
            break;
        }
        case Uint32ToHex:
        {
            break;
        }
        case Int32ToHex:
        {
            break;
        }
        case Uint64ToHex:
        {
            break;
        }
        case Int64ToHex:
        {
            break;
        }
        case FloatToHex:
        {
            break;
        }
        case DoubleToHex:
        {
            break;
        }
        default:
        {
            break;
        }
    }
}

void Converter::validateSource(void){
    static QString previousText("");
    static int cursorPosition;

    QString currentText(_source->toPlainText());
    QTextCursor cursor = _source->textCursor();
    int position = cursor.position();

    if(_source->toPlainText() == previousText)
        return;
    if (validator->validate(currentText, position) == QValidator::Invalid) {
        _source->setPlainText(previousText);
        cursor.setPosition(cursorPosition);
        _source->setTextCursor(cursor);
        qDebug() << "Validator failed";
    }
    else {
        previousText = _source->toPlainText();
        cursorPosition = position;
    }
    // Выполним группировку побайтово
    //setDelimitersInHexString(_source);
}


void Converter::setDelimitersInHexString(QPlainTextEdit *textEdit){
    QString resultText;
    QString currentText(textEdit->toPlainText());
    int pairCounter = 0;
    QTextCursor cursor = textEdit->textCursor();
    bool cursorAtEnd = cursor.atEnd();
    int cursorPosition = cursor.position();
    // Будем перебирать по байту и группировать символы
    // по два разделяя их пробелом в новой строке и
    // пропуская лишние пробелы.
    for(int count = 0; count < currentText.count(); count++){
        if(currentText[count] != ' ') {
            resultText.append(currentText[count]);
            pairCounter++;
        }
        if(pairCounter >= 2 && count < currentText.count()-1){
            resultText.append(' ');
            pairCounter = 0;
        }
    }
    // В поле просто выбрасываем уже сгруппированную строку
    textEdit->setPlainText(resultText);
    // Необходимо выбрал расположение курсора, согласно его расположению до группировки:
    // Если до группировки он был в конце, также переводим его в конец
    if(cursorAtEnd)
        cursor.movePosition(QTextCursor::End);
    // Если внутри, то нужно поставить курсор в тоже положение
    else
        cursor.setPosition(cursorPosition);
    // Устанавливаем положение курсора на исходном виджете
    textEdit->setTextCursor(cursor);
}

QString Converter::expandToFullType(QString &source, TypeName type)
{
    QString fullType;
    if(type == UINT8 || type == INT8)
        fullType = "00";
    else if (type == UINT16 || type == INT16)
        fullType = "00 00";
    else if (type == UINT32 || type == INT32 || type == FLOAT)
        fullType = "00 00 00 00";
    else if(type == UINT64 || type == INT64 || type == DOUBLE)
        fullType = "00 00 00 00 00 00 00 00";
    else
        return "";
    // Обрезаем конец форматной строки по кол-ву символов источника
    fullType.chop(source.count());
    // И добавляем в нее источник
    fullType.append(source);
    source = fullType;

    return fullType;
}


QByteArray Converter::parseStringForHex(QString &string){
    QString clearSource;
    QByteArray result;
    // Удалим все разделители
    for(int i = 0; i < string.count(); i++)
    {
        if(string[i] != ' ')
            clearSource.append(string[i]);
    }
    // И дополним нулем последний байт, если необходимо
    if(clearSource.count()%2 != 0)
        clearSource.insert(clearSource.count()-1, '0');

    // Будем брать по два байта из очищенной строки и преобразовывать
    for(int i = 0, j = 0; i < clearSource.count(); i+=2, j++)
    {
        QString temp(clearSource[i]);
        temp.append(clearSource[i+1]);
        result.insert(j, static_cast<char>(temp.toInt(nullptr, 16)));
    }
    return result;
}


QString Converter::convertHexToAscii(QString &source){
    return parseStringForHex(source);
}

QString Converter::convertHexToUint8(QString &source){
    QByteArray result = parseStringForHex(source);
    uint8_t value = *(reinterpret_cast<uint8_t *>(result.data()));
    swapEndian(value);

    return QString::number(value);
}

QString Converter::convertHexToInt8 (QString &source){
    QByteArray result = parseStringForHex(source);
    int8_t value = *(reinterpret_cast<int8_t *>(result.data()));
    swapEndian(value);

    return QString::number(value);
}

QString Converter::convertHexToUint16 (QString &source){
    QByteArray result = parseStringForHex(source);
    uint16_t value = *(reinterpret_cast<uint16_t *>(result.data()));
    swapEndian(value);

    return QString::number(value);
}

QString Converter::convertHexToInt16 (QString &source){
    QByteArray result = parseStringForHex(source);
    int16_t value = *(reinterpret_cast<int16_t *>(result.data()));
    swapEndian(value);

    return QString::number(value);
}

QString Converter::convertHexToUint32 (QString &source){
    QByteArray result = parseStringForHex(source);
    uint32_t value = *(reinterpret_cast<uint32_t *>(result.data()));
    swapEndian(value);

    return QString::number(value);
}

QString Converter::convertHexToInt32 (QString &source){
    QByteArray result = parseStringForHex(source);
    int32_t value = *(reinterpret_cast<int32_t *>(result.data()));
    swapEndian(value);

    return QString::number(value);
}

QString Converter::convertHexToUint64 (QString &source){
    QByteArray result = parseStringForHex(source);
    uint64_t value = *(reinterpret_cast<uint64_t *>(result.data()));
    swapEndian(value);

    return QString::number(value);
}

QString Converter::convertHexToInt64 (QString &source){
    QByteArray result = parseStringForHex(source);
    int64_t value = *(reinterpret_cast<int64_t *>(result.data()));
    swapEndian(value);

    return QString::number(value);
}

QString Converter::convertHexToFloat (QString &source){
    QByteArray result = parseStringForHex(source);
    float value = *(reinterpret_cast<float *>(result.data()));
    swapEndian(value);

    return QString::number(value);
}

QString Converter::convertHexToDouble (QString &source){
    QByteArray result = parseStringForHex(source);
    double value = *(reinterpret_cast<double *>(result.data()));
    swapEndian(value);

    return QString::number(value);
}

QString Converter::convertAsciiToHex (QString source){
    QString result = "";
    for(int i = 0; i < source.count(); i++)
    {
        result+=QString::number(static_cast<unsigned char>(source.toLatin1()[i]),16).rightJustified(2, '0');
        if(i < source.count()-1)
            result.append(" ");
    }
    result = result.toUpper();
    return result;
}

QString Converter::convertUint8ToHex (QString source){

}
QString Converter::convertInt8ToHex(QString source){

}
QString Converter::convertUnt16ToHex(QString source){

}
QString Converter::convertInt16ToHex(QString source){

}
QString Converter::convertUnt32ToHex(QString source){

}
QString Converter::convertInt32ToHex(QString source){

}
QString Converter::convertUnt64ToHex(QString source){

}
QString Converter::convertInt64ToHex(QString source){

}
QString Converter::convertFloatToHex(QString source){

}
QString Converter::convertDoubleToHex(QString source){

}

void Converter::swapEndian(uint8_t &value){
    uint8_t result = value;
    value = result;
}
void Converter::swapEndian(int8_t &value){
    int8_t result = value;
    value = result;
}
void Converter::swapEndian(uint16_t &value){
    uint16_t result = value;
    value = static_cast<uint16_t>((result & 0x00FF) << 8 | (result & 0xFF00) >> 8);
}
void Converter::swapEndian(int16_t &value){
    int16_t result = value;
    value = static_cast<int16_t>((result & 0x00FF) << 8 | (result & 0xFF00) >> 8);
}
void Converter::swapEndian(uint32_t &value){
    uint32_t result = value;
    result = (result & 0x00FF00FF) << 8 | (result & 0xFF00FF00) >> 8;
    result = (result & 0x0000FFFF)<< 16 | (result & 0xFFFF0000) >> 16;
    value = result;
}
void Converter::swapEndian(int32_t &value){
    int32_t result = value;
    result = (result & int32_t(0x00FF00FF)) << 8  | (result & int32_t(0xFF00FF00)) >> 8;
    result = (result & int32_t(0x0000FFFF)) << 16 | (result & int32_t(0xFFFF0000)) >> 16;
    value = result;
}
void Converter::swapEndian(uint64_t &value){
    uint64_t result = value;
    result = (result & 0x00FF00FF00FF00FF) << 8 | (result & 0xFF00FF00FF00FF00) >> 8;
    result = (result & 0x0000FFFF0000FFFF)<< 16 | (result & 0xFFFF0000FFFF0000) >> 16;
    result = (result & 0x00000000FFFFFFFF)<< 32 | (result & 0xFFFFFFFF00000000) >> 32;
    value = result;
}
void Converter::swapEndian(int64_t &value){
    uint64_t result = static_cast<uint64_t>(value);
    result = (result & 0x00FF00FF00FF00FF) << 8 | (result & 0xFF00FF00FF00FF00) >> 8;
    result = (result & 0x0000FFFF0000FFFF)<< 16 | (result & 0xFFFF0000FFFF0000) >> 16;
    result = (result & 0x00000000FFFFFFFF)<< 32 | (result & 0xFFFFFFFF00000000) >> 32;
    value = static_cast<int64_t>(result);
}
void Converter::swapEndian(float &value){
    union{
        float    floatValue;
        uint32_t integerValue;
    }convertionUnion;

    convertionUnion.floatValue = value;
    convertionUnion.integerValue = (convertionUnion.integerValue & 0x00FF00FF) << 8 |
                                   (convertionUnion.integerValue & 0xFF00FF00) >> 8;
    convertionUnion.integerValue = (convertionUnion.integerValue & 0x0000FFFF)<< 16 |
                                   (convertionUnion.integerValue & 0xFFFF0000) >> 16;
    value = convertionUnion.floatValue;
}

void Converter::swapEndian(double &value){
    union{
        double   doubleValue;
        uint64_t integerValue;
    }convertionUnion;
    convertionUnion.doubleValue = value;
    convertionUnion.integerValue = (convertionUnion.integerValue & 0x00FF00FF00FF00FF) << 8 |
                                   (convertionUnion.integerValue & 0xFF00FF00FF00FF00) >> 8;
    convertionUnion.integerValue = (convertionUnion.integerValue & 0x0000FFFF0000FFFF)<< 16 |
                                   (convertionUnion.integerValue & 0xFFFF0000FFFF0000) >> 16;
    convertionUnion.integerValue = (convertionUnion.integerValue & 0x00000000FFFFFFFF)<< 32 |
                                   (convertionUnion.integerValue & 0xFFFFFFFF00000000) >> 32;
    value = convertionUnion.doubleValue;
}
