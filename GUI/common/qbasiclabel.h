#ifndef QBASICLABEL_H
#define QBASICLABEL_H

#include <QLabel>
#include <QWidget>

class QBasicLabel : public QLabel
{
    Q_OBJECT
public:
    QBasicLabel(QWidget *parent=nullptr, const QString &text="",  Qt::WindowFlags f=Qt::WindowFlags());
};

#endif // QBASICLABEL_H
