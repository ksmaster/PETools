#ifndef _BASE_WINDOW_H___
#define _BASE_WINDOW_H___

#include <QObject>
#include <QPaintEvent>
#include <QWidget>

class BaseWindow;


struct ItemPair
{
    QString key;
    QString value;
};


extern QList<BaseWindow*> getAllTransWindows();

class BaseWindow : public QWidget
{
    Q_OBJECT
public:
    BaseWindow(QWidget *parent=nullptr);
    ~BaseWindow();
    virtual void retranslateUi();
    void enableMove(bool bEnable) { m_bEnableMove = bEnable; }
private:
    bool m_bEnableMove = false;
    bool m_isMouseInTitle = false;
    QPoint m_ptWindow;
protected:
    void changeEvent(QEvent* ev);
    void paintEvent(QPaintEvent* ev);
    void mousePressEvent(QMouseEvent* ev);
    void mouseMoveEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);
};

#endif
