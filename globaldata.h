#ifndef GLOBALDATA_H
#define GLOBALDATA_H
#include "PEInfo.h"
#include "mythread.h"
#include "peloadworker.h"

class GlobalData: public QObject
{
 Q_OBJECT
public:
    GlobalData(QObject *parent);
    ~GlobalData();
    void init();
    CPEInfo &GetPEInfo() {
        return m_peInfo;
    }
    PELoadWorker * GetPELoadWorker() {
        return m_peLoadWorker;
    }
private:

    CPEInfo m_peInfo;
    QThread * m_pThread;
    PELoadWorker *m_peLoadWorker;
    QObject *m_parent;
};

#endif // GLOBALDATA_H
