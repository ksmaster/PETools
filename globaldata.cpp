#include "globaldata.h"
#include "mythread.h"
#include "peloadworker.h"


GlobalData::GlobalData(QObject *parent):m_pThread(nullptr),m_parent(parent)
{

}


void GlobalData::init() {
    m_pThread = new MyThread(m_parent);
    m_peLoadWorker = new PELoadWorker(m_peInfo);
    m_peLoadWorker->moveToThread(m_pThread);
    connect(m_pThread, &QThread::finished, m_peLoadWorker, &QObject::deleteLater);
    m_pThread->start();
}


GlobalData::~GlobalData() {
    if(m_pThread) {
        m_pThread->quit();
        m_pThread->wait();
    }
}
