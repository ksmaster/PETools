#ifndef _TASK_MRG__H_
#define _TASK_MRG__H_
#include <QThread>
#include <QQueue>
#include <QVariant>
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
    void startNewTask(const QString&);
   
private:
    TaskMgr();
signals:
    void loadPEBasicTask(const QString& strPEFile);
    void updateGUI(int nTaskType);
private slots:
    void onWaitAndStartNewTask(const QString& strPEFile);
    void dispatchTask(int nTaskType, QVariant param);
private:
    CPEInfo m_peInfo;
    QList<PELoadWorker*> m_workList;
    //QList<QThread*> m_thrdList;
    static TaskMgr* m_taskMgr;
};

#endif

