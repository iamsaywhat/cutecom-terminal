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
    if(_resultBox->currentIndex() == HEX){

    }
    else if (_resultBox->currentIndex() == ASCII){
        validator->setRegularExpression(*hexArray);
    }
    else if (_resultBox->currentIndex() == UINT8 ||
             _resultBox->currentIndex() == INT8){
        validator->setRegularExpression(*hex1Byte);
    }
    else if (_resultBox->currentIndex() == UINT16 ||
             _resultBox->currentIndex() == INT16){
        validator->setRegularExpression(*hex2Byte);
    }
    else if (_resultBox->currentIndex() == UINT32 ||
             _resultBox->currentIndex() == INT32 ||
             _resultBox->currentIndex() == FLOAT){
        validator->setRegularExpression(*hex4Byte);
    }
    else if (_resultBox->currentIndex() == UINT64 ||
             _resultBox->currentIndex() == INT64 ||
             _resultBox->currentIndex() == DOUBLE){
        validator->setRegularExpression(*hex8Byte);
    }
    else {

    }

}

void Converter::validateSource(void){
#ifdef DEBUG
    qDebug() << "Call validateSource";
#endif

//   if(validator->validate(_source->toPlainText(), _source->textCursor().position()) == QValidator::Invalid) {

//   }

}
