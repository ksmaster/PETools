#ifndef PELOADWORKER_H
#define PELOADWORKER_H
#include <QVariant>
#include <QObject>
#include "PEInfo.h"

enum PE_TASK_TYPE {
    PE_TASK_TYPE_LOAD_CONTROLLER = 0,
    PE_TASK_TYPE_LOAD_BASIC,
    PE_TASK_TYPE_LOAD_EAT,
    PE_TASK_TYPE_LOAD_IDT,
    PE_TASK_TYPE_CNT
};

inline const QString getPETaskName(int type)
{
    switch (type)
    {
    case PE_TASK_TYPE_LOAD_CONTROLLER:
        return "controller";
    case PE_TASK_TYPE_LOAD_BASIC:
        return "load basic";
    case PE_TASK_TYPE_LOAD_EAT:
        return "load eat";
    case PE_TASK_TYPE_LOAD_IDT:
        return "load idt";
    default:
        return QString("invalid type(%1)").arg(int(type));
    }
    
}

class PELoadWorker : public QObject
{
    Q_OBJECT
public:
    explicit PELoadWorker(PE_TASK_TYPE, CPEInfo &peInfo, QObject *parent = nullptr);
    bool isRunning() { return m_bIsRunning; }
    void initFileName(const QString& strFileName) {  m_strPEFileName = strFileName;    }
    void initIATFirstTrunk(DWORD dwFirstTrunk) { m_dwFirstTrunk = dwFirstTrunk; }
private:
    void loadPEBasic();
    void loadExportTable();
    void loadImportTable();
private:
    QString m_strPEFileName;
    DWORD m_dwFirstTrunk=0;
    CPEInfo &m_peInfoRef;
    PE_TASK_TYPE m_taskType;
    bool m_bIsRunning=false;
signals:
     void SignalStartNewPETask(const QString& fileName);
     void waitAndStartNewTask(const QString& fileName);
     void relayTask(int nType, QVariant param);
     void executeTask(int nType, QVariant param);
     void updateGUI(int nTaskType);
public slots:
    void doPETask(int nType, QVariant param);
    void onSignalStartNewPETask(const QString& fileName);
    
};

#endif // PELOADWORKER_H
