#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include "PEInfo.h"

class MyThread : public QThread
{
    Q_OBJECT
public:
    MyThread(QObject* parent = nullptr);
   private:
    QString m_strFileName;
//signals:
//    void result();
};

#endif // MYTHREAD_H
