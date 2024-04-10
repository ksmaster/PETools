#include "TaskMgr.h"
#include <QThread>

#include "peloadworker.h"
#include <QDebug>


TaskMgr* TaskMgr::m_taskMgr = nullptr;

TaskMgr::TaskMgr()
{

}

TaskMgr* TaskMgr::instance()
{
    if (!m_taskMgr)
    {
        m_taskMgr = new TaskMgr();
    }
    return m_taskMgr;
}


TaskMgr::~TaskMgr() {
#if 0
    for (auto thrd : m_thrdList)
    {
        if (thrd) {
            thrd->quit();
            thrd->wait();
        }
    }
#endif
    
}


void TaskMgr::init()
{
    for (int i = 0; i < PE_TASK_TYPE_CNT; ++i)
    {
        QThread* thrd = new QThread();
        PELoadWorker* peLoadWorker = new PELoadWorker(PE_TASK_TYPE(i), m_peInfo);
        peLoadWorker->moveToThread(thrd);
        connect(thrd, &QThread::finished, peLoadWorker, &QObject::deleteLater);
        if(PE_TASK_TYPE_LOAD_CONTROLLER == i)
        {
            connect(peLoadWorker, &PELoadWorker::waitAndStartNewTask, this, &TaskMgr::onWaitAndStartNewTask, Qt::DirectConnection);
        }
        else
        {
            connect(peLoadWorker, SIGNAL(relayTask(int, QVariant)), this, SLOT(dispatchTask(int, QVariant)));
            connect(peLoadWorker, &PELoadWorker::updateGUI, this, &TaskMgr::updateGUI);
        }
        m_workList.append(peLoadWorker);
        thrd->start();
    }

}

void TaskMgr::startNewTask(const QString& strPEFileName)
{
    m_peInfo.setStopFlag(true);
    emit m_workList[PE_TASK_TYPE_LOAD_CONTROLLER]->SignalStartNewPETask(strPEFileName);
}



void TaskMgr::onWaitAndStartNewTask(const QString& strPEFile)
{
    qDebug() << "TaskMgr::onWaitAndStartNewTask: thread: " << QThread::currentThread();
    for (int i = PE_TASK_TYPE_LOAD_BASIC; i < PE_TASK_TYPE_CNT; ++i)
    {
        auto work = m_workList[i];
        while (work->isRunning())
        {
            QThread::msleep(10);
        }
        work->initFileName(strPEFile);
    }
    emit m_workList[PE_TASK_TYPE_LOAD_BASIC]->executeTask(PE_TASK_TYPE_LOAD_BASIC, QVariant(0));
}

void TaskMgr::dispatchTask(int nTaskType, QVariant param)
{
    if (nTaskType >= 0 && nTaskType < m_workList.length())
    {
        qDebug() << "TaskMgr::dispatchTask: [" << getPETaskName(nTaskType) << "], thread: " << QThread::currentThread();
        emit m_workList[nTaskType]->executeTask(nTaskType, param);
        return;
    }
    qDebug() << "error nTaskType: " << nTaskType;
}

