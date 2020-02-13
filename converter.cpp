#include "converter.h"

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

    validator->setRegularExpression(*hexArray);
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
