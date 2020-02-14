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

    sourceBox->addItem("HEX", HEX);

    resultBox->addItem("ASCII",  ASCII);
    resultBox->addItem("uint8",  UINT8);
    resultBox->addItem("int8",   INT8);
    resultBox->addItem("uint16", UINT16);
    resultBox->addItem("int16",  INT16);
    resultBox->addItem("uint32", UINT32);
    resultBox->addItem("int32",  INT32);
    resultBox->addItem("uint64", UINT64);
    resultBox->addItem("int64",  INT64);
    resultBox->addItem("float",  FLOAT);
    resultBox->addItem("double", DOUBLE);

    convertButton->setText("Convert");
    swapButton->setText("Swap");
    clearButton->setText("Clear");

    connect(resultBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Converter::resultTypeChanges);
    connect(source, &QPlainTextEdit::textChanged, this,  &Converter::validateSource);
    connect(convertButton, &QPushButton::clicked, this, &Converter::convert);
    connect(swapButton, &QPushButton::clicked, this, &Converter::swap);
    connect(clearButton, &QPushButton::clicked, this, &Converter::clear);

    validator->setRegularExpression(*hexArray);

    QByteArray array;
    array[0] = 0x40;
    array[1] = 0x23;
    array[2] = 0xf4;
    array[3] = 0x75;
    array[4] = 0x2c;
    array[5] = 0x74;
    array[6] = 0x49;
    array[7] = 0xa0;
    QByteArray *ptt = &array;
    qDebug() << array << array.data();
    qDebug() << qFromBigEndian<quint64>(ptt->data());
    qDebug() << *((unsigned short *)(ptt->data()+1));
    qDebug() << *(reinterpret_cast<unsigned short *>(ptt->data()));

}


Converter::~Converter(void) {
    delete hexArray;
    delete hex8Byte;
    delete hex4Byte;
    delete hex2Byte;
    delete hex1Byte;
    delete validator;
}


void Converter::resultTypeChanges (void){
#ifdef DEBUG
    qDebug() << "Call resultTypeChanges";
#endif
    if(_resultBox->currentData() == HEX){

    }
    else if (_resultBox->currentData() == ASCII){
        validator->setRegularExpression(*hexArray);
    }
    else if (_resultBox->currentData() == UINT8 ||
             _resultBox->currentData() == INT8){
        validator->setRegularExpression(*hex1Byte);
    }
    else if (_resultBox->currentData() == UINT16 ||
             _resultBox->currentData() == INT16){
        validator->setRegularExpression(*hex2Byte);
    }
    else if (_resultBox->currentData() == UINT32 ||
             _resultBox->currentData() == INT32 ||
             _resultBox->currentData() == FLOAT){
        validator->setRegularExpression(*hex4Byte);
    }
    else if (_resultBox->currentData() == UINT64 ||
             _resultBox->currentData() == INT64 ||
             _resultBox->currentData() == DOUBLE){
        validator->setRegularExpression(*hex8Byte);
    }
    else {

    }

}

void Converter::validateSource(void){
#ifdef DEBUG
    qDebug() << "Call validateSource";
#endif
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
    }
    else {
        previousText = _source->toPlainText();
        cursorPosition = position;
    }
    // Выполним группировку побайтово
    setDelimitersInHexString(_source);
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

//QString Converter::convertHexToAscii (QByteArray &source){
//    QString result = "";
//    for(int i = 0; i < source.count(); i++)
//    {
//        result+=QString::number(static_cast<unsigned char>(source[i]),16).rightJustified(2, '0');
//        if(i < source.count()-1)
//            result.append(" ");
//    }
//    result = result.toUpper();
//    return result;
//}


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

    qDebug()<< QString::number(*(reinterpret_cast<uint8_t *>(result.data())));
    return QString::number(*(reinterpret_cast<uint8_t *>(result.data())));
}

QString Converter::convertHexToInt8 (QString &source){
    QByteArray result = parseStringForHex(source);

    qDebug()<< QString::number(*(reinterpret_cast<int8_t *>(result.data())));
    return QString::number(*(reinterpret_cast<int8_t *>(result.data())));
}

QString Converter::convertHexToUint16 (QString &source){
    QByteArray result = parseStringForHex(source);

    qDebug()<< QString::number(*(reinterpret_cast<uint16_t *>(result.data())));
    return QString::number(*(reinterpret_cast<uint16_t *>(result.data())));
}

QString Converter::convertHexToInt16 (QString &source){
    QByteArray result = parseStringForHex(source);

    qDebug()<< QString::number(*(reinterpret_cast<int16_t *>(result.data())));
    return QString::number(*(reinterpret_cast<int16_t *>(result.data())));
}

QString Converter::convertHexToUint32 (QString &source){
    QByteArray result = parseStringForHex(source);
    qDebug()<< QString::number(*(reinterpret_cast<uint32_t *>(result.data())));

    return QString::number(*(reinterpret_cast<uint32_t *>(result.data())));
}

QString Converter::convertHexToInt32 (QString &source){
    QByteArray result = parseStringForHex(source);

    qDebug()<< QString::number(*(reinterpret_cast<int32_t *>(result.data())));
    return QString::number(*(reinterpret_cast<int32_t *>(result.data())));
}

QString Converter::convertHexToUint64 (QString &source){
    QByteArray result = parseStringForHex(source);

    qDebug()<< QString::number(*(reinterpret_cast<uint64_t *>(result.data())));
    return QString::number(*(reinterpret_cast<uint64_t *>(result.data())));
}

QString Converter::convertHexToInt64 (QString &source){
    QByteArray result = parseStringForHex(source);

    qDebug()<< QString::number(*(reinterpret_cast<int64_t *>(result.data())));
    return QString::number(*(reinterpret_cast<int64_t *>(result.data())));
}

QString Converter::convertHexToFloat (QString &source){
    QByteArray result = parseStringForHex(source);

    qDebug()<< QString::number(*(reinterpret_cast<float *>(result.data())));
    return QString::number(*(reinterpret_cast<float *>(result.data())));
}

QString Converter::convertHexToDouble (QString &source){
    QByteArray result = parseStringForHex(source);
    // result.s
    qDebug()<< QString::number(*(reinterpret_cast<double *>(result.data())));
    return QString::number(*(reinterpret_cast<double *>(result.data())));
}






void Converter::convert(void){
    QString sourceData(_source->toPlainText());
    QString resultData;

    if(_resultBox->currentData() == ASCII)
        resultData = convertHexToAscii(sourceData);
    else if (_resultBox->currentData() == UINT8)
        resultData = convertHexToUint8(sourceData);
    else if (_resultBox->currentData() == INT8)
        resultData = convertHexToInt8(sourceData);
    else if (_resultBox->currentData() == UINT16)
        resultData = convertHexToUint16(sourceData);
    else if (_resultBox->currentData() == INT16)
        resultData = convertHexToInt16(sourceData);
    else if (_resultBox->currentData() == UINT32)
        resultData = convertHexToUint32(sourceData);
    else if (_resultBox->currentData() == INT32)
        resultData = convertHexToInt32(sourceData);
    else if (_resultBox->currentData() == UINT64)
        resultData = convertHexToUint64(sourceData);
    else if (_resultBox->currentData() == INT64)
        resultData = convertHexToInt64(sourceData);
    else if (_resultBox->currentData() == FLOAT)
        resultData = convertHexToFloat(sourceData);
    else if (_resultBox->currentData() == DOUBLE)
        resultData = convertHexToDouble(sourceData);
    else
        resultData = "";

    // Результат в правое окно
    _result->setPlainText(resultData.toLatin1());
}
void Converter::swap(){
    QString str(_source->toPlainText());
    convertHexToUint8(str);
    convertHexToUint8 (str);
    convertHexToInt8 (str);
    convertHexToUint16 (str);
    convertHexToInt16 (str);
    convertHexToUint32 (str);
    convertHexToInt32 (str);
    convertHexToUint64 (str);
    convertHexToInt64 (str);
    convertHexToFloat (str);
    convertHexToDouble (str);
}

void Converter::clear(){
    _source->clear();
    _result->clear();
}
