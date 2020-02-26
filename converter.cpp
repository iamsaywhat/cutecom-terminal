#include "converter.h"
#include <QTextCodec>

#define DEBUG

#ifdef DEBUG
    #include <QDebug>
#endif

QByteArray Converter::_currentCodec = "IBM866";

bool Converter::setCurrentCodec(QByteArray codec){
    bool status = QTextCodec::availableCodecs().contains(codec);
    if(status)
        _currentCodec = codec;
    return status;
}

QByteArray Converter::currentCodec(void){
    return _currentCodec;
}

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

    result->setReadOnly(true);            // Запрещаем редактировать область результата
    convertButton->setText("Convert");    // Именуем кнопки
    swapButton->setText("Swap");
    clearButton->setText("Clear");

    direction = DIRECT;                   // Исходное напрвление преобразования - прямое
    fillComboBoxs(sourceBox, resultBox);  // Заполняем combobox'ы
    updateConversionType();               // Запускаем обновление id текущего преобразования
    resultTypeChanges();                  // Здесь установится нужный валидатор


    connect(resultBox,     QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Converter::resultTypeChanges);
    connect(sourceBox,     QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Converter::resultTypeChanges);
    connect(source,        &QPlainTextEdit::textChanged,                        this, &Converter::validateSource);
    connect(convertButton, &QPushButton::clicked,                               this, &Converter::convert);
    connect(swapButton,    &QPushButton::clicked,                               this, &Converter::swap);
    connect(clearButton,   &QPushButton::clicked,                               this, &Converter::clear);
}

Converter::~Converter(void) {
    delete hexArrayRegEx;
    delete hex8ByteRegEx;
    delete hex4ByteRegEx;
    delete hex2ByteRegEx;
    delete hex1ByteRegEx;
    delete asciiRegEx;
    delete floatingPointRegExp;
    delete unsignedIntegerRegEx;
    delete signedIntegerRegEx;
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
        if(_sourceBox->currentData() == HEX){
            if(_resultBox->currentData() == ASCII)
                convertionId = HexToAscii;
            else if (_resultBox->currentData() == UINT8)
                convertionId = HexToUint8;
            else if (_resultBox->currentData() == INT8)
                convertionId = HexToInt8;
            else if (_resultBox->currentData() == UINT16)
                convertionId = HexToUint16;
            else if (_resultBox->currentData() == INT16)
                convertionId = HexToInt16;
            else if (_resultBox->currentData() == UINT32)
                convertionId = HexToUint32;
            else if (_resultBox->currentData() == INT32)
                convertionId = HexToInt32;
            else if (_resultBox->currentData() == UINT64)
                convertionId = HexToUint64;
            else if (_resultBox->currentData() == INT64)
                convertionId = HexToInt64;
            else if (_resultBox->currentData() == FLOAT)
                convertionId = HexToFloat;
            else if (_resultBox->currentData() == DOUBLE)
                convertionId = HexToDouble;
            else
                convertionId = Unknown;
        }
    }
    // Обратный тип преобразований
    else{
        if(_resultBox->currentData() == HEX){
            // Прямое направление преобзования
            if(_sourceBox->currentData() == ASCII)
                convertionId = AsciiToHex;
            else if (_sourceBox->currentData() == UINT8)
                convertionId = Uint8ToHex;
            else if (_sourceBox->currentData() == INT8)
                convertionId = Int8ToHex;
            else if (_sourceBox->currentData() == UINT16)
                convertionId = Uint16ToHex;
            else if (_sourceBox->currentData() == INT16)
                convertionId = Int16ToHex;
            else if (_sourceBox->currentData() == UINT32)
                convertionId = Uint32ToHex;
            else if (_sourceBox->currentData() == INT32)
                convertionId = Int32ToHex;
            else if (_sourceBox->currentData() == UINT64)
                convertionId = Uint64ToHex;
            else if (_sourceBox->currentData() == INT64)
                convertionId = Int64ToHex;
            else if (_sourceBox->currentData() == FLOAT)
                convertionId = FloatToHex;
            else if (_sourceBox->currentData() == DOUBLE)
                convertionId = DoubleToHex;
            else
                convertionId = Unknown;
        }
    }
}

void Converter::convert(void){
    QString sourceData(_source->toPlainText());
    QString resultData;
    switch (convertionId) {
        case HexToAscii:
            resultData = convertHexToAscii(sourceData);
            break;
        case HexToUint8:
            _source->setPlainText(expandToFullType(sourceData, UINT8));
            resultData = convertHexToUint8(sourceData);
            break;
        case HexToInt8:
            _source->setPlainText(expandToFullType(sourceData, INT8));
            resultData = convertHexToInt8(sourceData);
            break;
        case HexToUint16:
            _source->setPlainText(expandToFullType(sourceData, UINT16));
            resultData = convertHexToUint16(sourceData);
            break;
        case HexToInt16:
            _source->setPlainText(expandToFullType(sourceData, INT16));
            resultData = convertHexToInt16(sourceData);
            break;
        case HexToUint32:
            _source->setPlainText(expandToFullType(sourceData, UINT32));
            resultData = convertHexToUint32(sourceData);
            break;
        case HexToInt32:
            _source->setPlainText(expandToFullType(sourceData, INT32));
            resultData = convertHexToInt32(sourceData);
            break;
        case HexToUint64:
            _source->setPlainText(expandToFullType(sourceData, UINT64));
            resultData = convertHexToUint64(sourceData);
            break;
        case HexToInt64:
            _source->setPlainText(expandToFullType(sourceData, INT64));
            resultData = convertHexToInt64(sourceData);
            break;
        case HexToFloat:
            _source->setPlainText(expandToFullType(sourceData, FLOAT));
            resultData = convertHexToFloat(sourceData);
            break;
        case HexToDouble:
            _source->setPlainText(expandToFullType(sourceData, DOUBLE));
            resultData = convertHexToDouble(sourceData);
            break;
        case AsciiToHex:
            resultData = convertAsciiToHex(sourceData);
            break;
        case Uint8ToHex:
            resultData = convertUint8ToHex(sourceData);
            break;
        case Int8ToHex:
            resultData = convertInt8ToHex(sourceData);
            break;
        case Uint16ToHex:
            resultData = convertUint16ToHex(sourceData);
            break;
        case Int16ToHex:
            resultData = convertInt16ToHex(sourceData);
            break;
        case Uint32ToHex:
            resultData = convertUint32ToHex(sourceData);
            break;
        case Int32ToHex:
            resultData = convertInt32ToHex(sourceData);
            break;
        case Uint64ToHex:
            resultData = convertUint64ToHex(sourceData);
            break;
        case Int64ToHex:
            resultData = convertInt64ToHex(sourceData);
            break;
        case FloatToHex:
            resultData = convertFloatToHex(sourceData);
            break;
        case DoubleToHex:
            resultData = convertDoubleToHex(sourceData);
            break;
        default:
            resultData = "";
            break;
    }

    // Результат в правое окно
    _result->setPlainText(resultData);
}

void Converter::swap(){
    int sourceIndex = _sourceBox->currentIndex();
    int resultIndex = _resultBox->currentIndex();
    QString sourceText = _source->toPlainText();
    QString resultText = _result->toPlainText();
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
    _sourceBox->setCurrentIndex(resultIndex);
    _resultBox->setCurrentIndex(sourceIndex);
    _source->setPlainText(resultText);
    _result->setPlainText(sourceText);
    updateConversionType();
}

void Converter::clear(){
    _source->clear();
    _result->clear();
}

void Converter::resultTypeChanges (void){
    updateConversionType();
    switch (convertionId) {
        case HexToAscii:
            validator->setRegularExpression(*hexArrayRegEx);
            break;
        case HexToUint8:
        {
            QString temp = _source->toPlainText();
            temp.chop(temp.count()-2);
            _source->setPlainText(temp);
            validator->setRegularExpression(*hex1ByteRegEx);
            break;
        }
        case HexToInt8:
        {
            QString temp = _source->toPlainText();
            temp.chop(temp.count()-2);
            _source->setPlainText(temp);
            validator->setRegularExpression(*hex1ByteRegEx);
            break;
        }
        case HexToUint16:
        {
            QString temp = _source->toPlainText();
            temp.chop(temp.count()-5);
            _source->setPlainText(temp);
            validator->setRegularExpression(*hex2ByteRegEx);
            break;
        }
        case HexToInt16:
        {
            QString temp = _source->toPlainText();
            temp.chop(temp.count()-5);
            _source->setPlainText(temp);
            validator->setRegularExpression(*hex2ByteRegEx);
            break;
        }
        case HexToUint32:
        {
            QString temp = _source->toPlainText();
            temp.chop(temp.count()-11);
            _source->setPlainText(temp);
            validator->setRegularExpression(*hex4ByteRegEx);
            break;
        }
        case HexToInt32:
        {
            QString temp = _source->toPlainText();
            temp.chop(temp.count()-11);
            _source->setPlainText(temp);
            validator->setRegularExpression(*hex4ByteRegEx);
            break;
        }
        case HexToUint64:
        {
            QString temp = _source->toPlainText();
            temp.chop(temp.count()-23);
            _source->setPlainText(temp);
            validator->setRegularExpression(*hex8ByteRegEx);
            break;
        }
        case HexToInt64:
        {
            QString temp = _source->toPlainText();
            temp.chop(temp.count()-23);
            _source->setPlainText(temp);
            validator->setRegularExpression(*hex8ByteRegEx);
            break;
        }
        case HexToFloat:
        {
            QString temp = _source->toPlainText();
            temp.chop(temp.count()-11);
            _source->setPlainText(temp);
            validator->setRegularExpression(*hex4ByteRegEx);
            break;
        }
        case HexToDouble:
        {
            QString temp = _source->toPlainText();
            temp.chop(temp.count()-23);
            _source->setPlainText(temp);
            validator->setRegularExpression(*hex8ByteRegEx);
            break;
        }
        case AsciiToHex:
            validator->setRegularExpression(*asciiRegEx);
            break;
        case Uint8ToHex:
            validator->setRegularExpression(*unsignedIntegerRegEx);
            break;
        case Int8ToHex:
            validator->setRegularExpression(*signedIntegerRegEx);
            break;
        case Uint16ToHex:
            validator->setRegularExpression(*unsignedIntegerRegEx);
            break;
        case Int16ToHex:
            validator->setRegularExpression(*signedIntegerRegEx);
            break;
        case Uint32ToHex:
            validator->setRegularExpression(*unsignedIntegerRegEx);
            break;
        case Int32ToHex:
            validator->setRegularExpression(*signedIntegerRegEx);
            break;
        case Uint64ToHex:
            validator->setRegularExpression(*unsignedIntegerRegEx);
            break;
        case Int64ToHex:
            validator->setRegularExpression(*signedIntegerRegEx);
            break;
        case FloatToHex:
            validator->setRegularExpression(*floatingPointRegExp);
            break;
        case DoubleToHex:
            validator->setRegularExpression(*floatingPointRegExp);
            break;
        default:
            break;
    }
}

void Converter::validateSource(void){
    if(_sourceBox->currentData() == HEX)
        validateHexInput();
    else if(_sourceBox->currentData() == ASCII)
        validateAsciiInput();
    else if((_sourceBox->currentData() == UINT8)  |
            (_sourceBox->currentData() == INT8)   |
            (_sourceBox->currentData() == UINT16) |
            (_sourceBox->currentData() == INT16)  |
            (_sourceBox->currentData() == UINT32) |
            (_sourceBox->currentData() == INT32)  |
            (_sourceBox->currentData() == UINT64) |
            (_sourceBox->currentData() == INT64)  )
        validateIntegerInput();
    else if((_sourceBox->currentData() == DOUBLE) |
            (_sourceBox->currentData() == FLOAT) )
        validateFloatingPointInput();
}

void Converter::validateHexInput(void){
    static QString previousText("");
    static int cursorPosition;
    QString currentText(_source->toPlainText());
    QTextCursor cursor = _source->textCursor();
    int position = cursor.position();

    if(currentText == previousText)
        return;
    if (validator->validate(currentText, position) == QValidator::Invalid){
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
    setDelimitersInHexString(_source, 2, ' ');
}

void Converter::validateAsciiInput(void){
    static QString previousText("");
    static int cursorPosition;
    QString currentText(_source->toPlainText());

    QTextCursor cursor = _source->textCursor();
    int position = cursor.position();

    if(currentText == previousText)
        return;

    /* Так как валидатор разрешает только символы не более одного байта,
     * а кириллица в юникоде двухбайтовая, нужно провести некоторое
     * соответствие кодировок, а только потом проверить строку валидатором.
     * Строго говоря currentText преобразования из юникода будет содержать
     * кракозябру (QString - это всегда юникод), но зато c соответсвующим hex-кодом*/
    currentText = convertFromUnicode(currentText);

    if (validator->validate(currentText, position) == QValidator::Invalid){
        _source->setPlainText(previousText);
        cursor.setPosition(cursorPosition);
        _source->setTextCursor(cursor);
        qDebug() << "Validator failed";
    }
    else {
        previousText = _source->toPlainText();
        cursorPosition = position;
    }
}

void Converter::validateFloatingPointInput(){
    bool validatorStatus = true;
    bool translateStatus = true;
    static QString previousText("");
    static int cursorPosition;
    QString currentText(_source->toPlainText());
    QTextCursor cursor = _source->textCursor();
    int position = cursor.position();

    if(currentText == previousText)
        return;
    if (validator->validate(currentText, position) == QValidator::Invalid)
        validatorStatus = false;
    if (_sourceBox->currentData() == FLOAT)
        currentText.toFloat(&translateStatus);
    else if (_sourceBox->currentData() == DOUBLE)
        currentText.toDouble(&translateStatus);

    /* translationStatus будет ложен, в том числе, если число не удалось привести
     * к формату, из-за выхода из диапазона допустимых значений, поэтому пределы форматов
     * отдельно не контролируем, но нужно учесть случаи, когда пользователь хочет ввести число
     * в научной нотации - вводит 'e' и при проверке QString::toDouble мы получим ложный
     * translateStatus. То же самое касается пустой строки, '-' в начале числа и '-'/'+' после 'e'. Поэтому эти
     * исключительные случаи выносим в условие оценки валидности. */
    if(!validatorStatus ||
      (!translateStatus && currentText!=""
                        && currentText[currentText.count()-1]!='e'
                        && currentText[currentText.count()-1]!='-'
                        && currentText[currentText.count()-1]!='+')){
        _source->setPlainText(previousText);
        cursor.setPosition(cursorPosition);
        _source->setTextCursor(cursor);
        qDebug() << "Validator failed";
    }
    else{
        previousText = _source->toPlainText();
        cursorPosition = position;
    }
}

void Converter::validateIntegerInput()
{
    bool validatorStatus = true;
    bool translateStatus = true;
    bool rangeStatus = true;
    static QString previousText("");
    static int cursorPosition;
    QString currentText(_source->toPlainText());
    QTextCursor cursor = _source->textCursor();
    int position = cursor.position();

    if(currentText == previousText)
        return;
    if (validator->validate(currentText, position) == QValidator::Invalid)
        validatorStatus = false;
    if(_sourceBox->currentData() == UINT8){
        uint32_t value = currentText.toUInt(&translateStatus, 10);
        if(value > 0xFF)
            rangeStatus = false;
    }
    else if(_sourceBox->currentData() == INT8){
        int32_t value = currentText.toInt(&translateStatus, 10);
        if(value < -128 || value >= 128)
            rangeStatus = false;
    }
    else if(_sourceBox->currentData() == UINT16){
        uint32_t value = currentText.toUInt(&translateStatus, 10);
        if(value > 0xFFFF)
            rangeStatus = false;
    }
    else if(_sourceBox->currentData() == INT16){
        int32_t value = currentText.toInt(&translateStatus, 10);
        if(value < -32768 || value >= 32768)
            rangeStatus = false;
    }
    else if(_sourceBox->currentData() == UINT32)
        currentText.toUInt(&translateStatus, 10);
    else if(_sourceBox->currentData() == INT32)
        currentText.toInt(&translateStatus, 10);
    else if(_sourceBox->currentData() == UINT64)
        currentText.toULongLong(&translateStatus, 10);
    else if(_sourceBox->currentData() == INT64)
        currentText.toLongLong(&translateStatus, 10);

    /* translateStatus ложен, если строка пуста или например вводится только знак минус,
     * который пропускает валидатор, чтобы отличать такие ситуации translateStatus смешиваем
     * по И с проверкой строки на "пусто" и "минус" */
    if(!validatorStatus ||
           !rangeStatus ||
      (!translateStatus && currentText!="-"
                        && currentText!="")){
        _source->setPlainText(previousText);
        cursor.setPosition(cursorPosition);
        _source->setTextCursor(cursor);
        qDebug() << "Validator failed";
    }
    else{
        previousText = _source->toPlainText();
        cursorPosition = position;
    }
}

void Converter::setDelimitersInHexString(QPlainTextEdit *textEdit, int groupSize, char delimiter){
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
        if(currentText[count] != delimiter) {
            resultText.append(currentText[count]);
            pairCounter++;
        }
        if(pairCounter >= groupSize && count < currentText.count()-1){
            resultText.append(delimiter);
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

void Converter::setDelimitersInHexString(QString &currentText, int groupSize, char delimiter){
    QString resultText;
    int pairCounter = 0;
    /* Будем перебирать по байту и группировать символы
     * по два разделяя их пробелом в новой строке и
     * пропуская лишние пробелы. */
    for(int count = 0; count < currentText.count(); count++){
        if(currentText[count] != delimiter) {
            resultText.append(currentText[count]);
            pairCounter++;
        }
        if(pairCounter >= groupSize && count < currentText.count()-1){
            resultText.append(delimiter);
            pairCounter = 0;
        }
    }
    // Возращаем по ссылке результат группировки
    currentText = resultText;
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


QByteArray Converter::parseStringForHex(bool *status, QString &string, char delimiter){
    QByteArray result;
    QStringList clearSource = string.split(delimiter);
    for (int i = 0; i < clearSource.count(); i++)
        result.append(static_cast<char>(clearSource[i].toInt(status, 16)));
    return result;
}

QString Converter::convertHexToAscii(QString &source){
    QByteArray temp = parseStringForHex(nullptr, source, ' ');
    return convertToUnicode(temp);
}

QString Converter::convertHexToUint8(QString &source){
    QByteArray result = parseStringForHex(nullptr, source, ' ');
    uint8_t value = *(reinterpret_cast<uint8_t *>(result.data()));
    swapEndian(value);

    return QString::number(value);
}

QString Converter::convertHexToInt8 (QString &source){
    QByteArray result = parseStringForHex(nullptr, source, ' ');
    int8_t value = *(reinterpret_cast<int8_t *>(result.data()));
    swapEndian(value);

    return QString::number(value);
}

QString Converter::convertHexToUint16 (QString &source){
    QByteArray result = parseStringForHex(nullptr, source, ' ');
    uint16_t value = *(reinterpret_cast<uint16_t *>(result.data()));
    swapEndian(value);

    return QString::number(value);
}

QString Converter::convertHexToInt16 (QString &source){
    QByteArray result = parseStringForHex(nullptr, source, ' ');
    int16_t value = *(reinterpret_cast<int16_t *>(result.data()));
    swapEndian(value);

    return QString::number(value);
}

QString Converter::convertHexToUint32 (QString &source){
    QByteArray result = parseStringForHex(nullptr, source, ' ');
    uint32_t value = *(reinterpret_cast<uint32_t *>(result.data()));
    swapEndian(value);

    return QString::number(value);
}

QString Converter::convertHexToInt32 (QString &source){
    QByteArray result = parseStringForHex(nullptr, source, ' ');
    int32_t value = *(reinterpret_cast<int32_t *>(result.data()));
    swapEndian(value);

    return QString::number(value);
}

QString Converter::convertHexToUint64 (QString &source){
    QByteArray result = parseStringForHex(nullptr, source, ' ');
    uint64_t value = *(reinterpret_cast<uint64_t *>(result.data()));
    swapEndian(value);

    return QString::number(value);
}

QString Converter::convertHexToInt64 (QString &source){
    QByteArray result = parseStringForHex(nullptr, source, ' ');
    int64_t value = *(reinterpret_cast<int64_t *>(result.data()));
    swapEndian(value);

    return QString::number(value);
}

QString Converter::convertHexToFloat (QString &source){
    QByteArray result = parseStringForHex(nullptr, source, ' ');
    float value = *(reinterpret_cast<float *>(result.data()));
    swapEndian(value);

    return QString::number(static_cast<double>(value));
}

QString Converter::convertHexToDouble (QString &source){
    QByteArray result = parseStringForHex(nullptr, source, ' ');
    double value = *(reinterpret_cast<double *>(result.data()));
    swapEndian(value);

    return QString::number(value);
}

QString Converter::convertAsciiToHex (QString &source){
    QString result = "";

    source = convertFromUnicode(source);

    for(int i = 0; i < source.count(); i++){
        result+=QString::number(static_cast<unsigned char>(source.toLatin1()[i]),16).rightJustified(2, '0');
        if(i < source.count()-1)
            result.append(" ");
    }
    return result.toUpper();
}

QString Converter::convertUint8ToHex (QString &source){
    bool status;
    uint32_t value = static_cast<uint32_t>(source.toUInt(&status, 10));
    if(status && value > 0xFF)
        status = false;
    if(status){
        QString result(QString::number(value, 16).rightJustified(2, '0'));
        setDelimitersInHexString(result, 2, ' ');
        expandToFullType(result, UINT8);
        return result.toUpper();
    }
    else
        return "ERROR";
}

QString Converter::convertInt8ToHex(QString &source){
    bool status;
    int32_t value = source.toInt(&status, 10);
    qDebug() << value << hex << value;
    if(status && (value < -128 || value >= 128))
        status = false;
    if(status){
        QString result(QString::number(static_cast<uint8_t>(value), 16).rightJustified(2, '0'));
        setDelimitersInHexString(result, 2, ' ');
        expandToFullType(result, INT8);
        return result.toUpper();
    }
    else
        return "ERROR";
}

QString Converter::convertUint16ToHex(QString &source){
    bool status;
    uint32_t value = static_cast<uint32_t>(source.toUInt(&status, 10));
    if(status && value > 0xFFFF)
        status = false;
    if(status){
        QString result(QString::number(value, 16).rightJustified(4, '0'));
        setDelimitersInHexString(result, 2, ' ');
        expandToFullType(result, UINT16);
        return result.toUpper();
    }
    else
        return "ERROR";
}
QString Converter::convertInt16ToHex(QString &source){
    bool status;
    int32_t value = source.toInt(&status, 10);
    if(status && (value < -32768 || value >= 32768))
        status = false;
    if(status){
        QString result(QString::number(static_cast<uint16_t>(value), 16).rightJustified(4, '0'));
        setDelimitersInHexString(result, 2, ' ');
        expandToFullType(result, INT16);
        return result.toUpper();
    }
    else
        return "ERROR";
}
QString Converter::convertUint32ToHex(QString &source){
    bool status;
    uint32_t value = static_cast<uint32_t>(source.toUInt(&status, 10));
    if(status){
        QString result(QString::number(value, 16).rightJustified(8, '0'));
        setDelimitersInHexString(result, 2, ' ');
        expandToFullType(result, UINT32);
        return result.toUpper();
    }
    else
        return "ERROR";
}
QString Converter::convertInt32ToHex(QString &source){
    bool status;
    uint32_t value = static_cast<uint32_t>(source.toInt(&status, 10));
    if(status){
        QString result(QString::number(value, 16).rightJustified(8, '0'));
        setDelimitersInHexString(result, 2, ' ');
        expandToFullType(result, INT32);
        return result.toUpper();
    }
    else
        return "ERROR";
}
QString Converter::convertUint64ToHex(QString &source){
    bool status;
    uint64_t value = static_cast<uint64_t>(source.toULongLong(&status, 10));
    if(status){
        QString result(QString::number(value, 16).rightJustified(16, '0'));
        setDelimitersInHexString(result, 2, ' ');
        expandToFullType(result, UINT64);
        return result.toUpper();
    }
    else
        return "ERROR";
}
QString Converter::convertInt64ToHex(QString &source){
    bool status;
    uint64_t value = static_cast<uint64_t>(source.toLongLong(&status, 10));
    if(status){
        QString result(QString::number(value, 16).rightJustified(16, '0'));
        setDelimitersInHexString(result, 2, ' ');
        expandToFullType(result, INT64);
        return result.toUpper();
    }
    else
        return "ERROR";
}
QString Converter::convertFloatToHex(QString &source){
    union{
        float    floatValue;
        uint32_t integerValue;
    }convertionUnion;
    bool status;
    convertionUnion.floatValue = source.toFloat(&status);
    if(status){
        QString result(QString::number(convertionUnion.integerValue, 16));
        setDelimitersInHexString(result, 2, ' ');
        expandToFullType(result, FLOAT);
        return result.toUpper();
    }
    else
        return "ERROR";
}
QString Converter::convertDoubleToHex(QString &source){
    union{
        double   doubleValue;
        uint64_t integerValue;
    }convertionUnion;
    bool status;
    convertionUnion.doubleValue = source.toDouble(&status);
    if(status){
        QString result(QString::number(convertionUnion.integerValue, 16));
        setDelimitersInHexString(result, 2, ' ');
        expandToFullType(result, DOUBLE);
        return result.toUpper();
    }
    else
        return "ERROR";
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

QString Converter::convertFromUnicode (QString &text){
    QTextCodec *textCodec = QTextCodec::codecForName(currentCodec());
    return QString::fromLatin1(textCodec->fromUnicode(text));
}
QString Converter::convertToUnicode (QByteArray &text){
    QTextCodec *textCodec = QTextCodec::codecForName(currentCodec());
    return  textCodec->toUnicode(text);
}
