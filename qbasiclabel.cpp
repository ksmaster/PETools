#include "qbasiclabel.h"

QBasicLabel::QBasicLabel( QWidget *parent, const QString &text, Qt::WindowFlags f):QLabel(text,parent,f)
{
    this->setStyleSheet("border: 1px solid black;");
    this->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);
}
