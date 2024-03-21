#ifndef _HOVER_WINDOW_H___
#define _HOVER_WINDOW_H___

#include <QObject>
#include <QWidget>
#include <QMouseEvent>


class HoverWindow : public QWidget
{
    Q_OBJECT
public:
    HoverWindow(QWidget *parent=nullptr);
    virtual ~HoverWindow();
private:
    bool m_bHoverIn=false;
protected:
    void enterEvent(QEvent* ev);
    void leaveEvent(QEvent*);
    virtual void onHoverLeave();
    virtual void onHoverEvent(QMouseEvent*);
};

#endif
