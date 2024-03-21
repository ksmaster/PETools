#include "hexlineedit.h"
#include <QRegExpValidator>
#include <QRegExp>

QRegExp regx("[0-9a-fA-F]+$");
HexLineEdit::HexLineEdit(QWidget *parent): QLineEdit(parent)
{

    QValidator *validator = new QRegExpValidator(regx, this);
    setValidator(validator);
    connect(this, &QLineEdit::textEdited, this, &HexLineEdit::onTextEdit);
}

void HexLineEdit::onTextEdit(const QString &str) {
   this->setText(str.toUpper());
}



