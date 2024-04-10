#include <QThread>
#include <QDebug>
#include "peloadworker.h"
#include "Util.h"


PELoadWorker::PELoadWorker(PE_TASK_TYPE taskType, CPEInfo &peInfo, QObject *parent) : QObject(parent), m_taskType(taskType),
m_peInfoRef(peInfo)
{
    connect(this, SIGNAL(executeTask(int, QVariant)), this, SLOT(doPETask(int, QVariant)));
    connect(this, &PELoadWorker::SignalStartNewPETask, this, &PELoadWorker::onSignalStartNewPETask);
}

void PELoadWorker::doPETask(int nType, QVariant param)
{
    CBinaryStateGuard guard(m_bIsRunning, true);
    if (m_taskType != nType)
    {
        return;
    }
    switch (nType)
    {
    case PE_TASK_TYPE_LOAD_BASIC:
    {
        loadPEBasic();
        if (m_peInfoRef.loaded())
        {
            emit relayTask(PE_TASK_TYPE_LOAD_EAT, QVariant::fromValue(0));
            emit relayTask(PE_TASK_TYPE_LOAD_IDT, QVariant::fromValue(0));
        }
        break;
    }
    case PE_TASK_TYPE_LOAD_EAT:
    {
        loadExportTable();
        break;
    }
    
    case PE_TASK_TYPE_LOAD_IDT:
    {
        loadImportTable();
        break;
    }
    case PE_TASK_TYPE_LOAD_CONTROLLER:
    {
        break;
    }
    default:
        return;
    }
    emit updateGUI(m_taskType);
}

void PELoadWorker::onSignalStartNewPETask(const QString& fileName)
{
    emit waitAndStartNewTask(fileName);
}


void PELoadWorker::loadPEBasic()
{
    qDebug() << "PELoadWorker::loadPEBasic: thread: " << QThread::currentThread();
#if defined(WIN32)
     m_peInfoRef.LoadPE(reinterpret_cast<LPCTSTR>(m_strPEFileName.utf16()));
#elif __linux__
     qDebug() << "before calling:  fileName length: " << m_strPEFileName.length();
     m_peInfoRef.LoadPE(fileName.toUtf8().constData());
#endif
    
   
}

void PELoadWorker::loadExportTable()
{
    qDebug() << "PELoadWorker::loadExportTable: thread: " << QThread::currentThread();
    if (!m_peInfoRef.loaded())
    {
        return;
    }
    m_peInfoRef.loadEATDataDirectory();
}

void PELoadWorker::loadImportTable()
{
    qDebug() << "PELoadWorker::loadImportTable: thread: " << QThread::currentThread();
    if (!m_peInfoRef.loaded())
    {
        return;
    }
    m_peInfoRef.loadImportDataDirectory();
}


