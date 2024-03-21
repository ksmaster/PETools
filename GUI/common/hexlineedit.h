#ifndef HEXLINEEDIT_H
#define HEXLINEEDIT_H

#include <QObject>
#include <QtWidgets/QLineEdit>

class HexLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    HexLineEdit(QWidget *parent=nullptr);

    void onTextEdit(const QString &str);
};

#endif // HEXLINEEDIT_H
