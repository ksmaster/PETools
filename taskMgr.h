#ifndef _TASK_MRG__H_
#define _TASK_MRG__H_
#include <QThread>
#include <QQueue>
#include "PEInfo.h"
#include "peloadworker.h"




class TaskMgr : public QObject
{
    Q_OBJECT
public:
    static TaskMgr* instance();
    ~TaskMgr();
    void init();
    
    CPEInfo& GetPEInfo() {
        return m_peInfo;
    }
    void dispatchTask(int nTaskType);
    void startNewTask(const QString&);
private:
    TaskMgr();
    void onWaitAndStartNewTask(const QString& strPEFile);
signals:
    void loadPEBasicTask(const QString& strPEFile);
    void updateGUI(int nTaskType);
private:
    CPEInfo m_peInfo;
    QList<PELoadWorker*> m_workList;
    //QList<QThread*> m_thrdList;
    static TaskMgr* m_taskMgr;
};

#endif // GLOBALDATA_H
