#include "peloadworker.h"
#include <QThread>
#include <QDebug>

PELoadWorker::PELoadWorker(CPEInfo &peInfo, QObject *parent) : QObject(parent),m_bPEBasicLoaded(false),m_peInfoRef(peInfo)
{

}


void PELoadWorker::loadPEBasic(const QString &fileName)
{
    if(m_bPEBasicLoaded) {
        return;
    }
    m_bPEBasicLoaded = true;
    qDebug() << "loadPEBasic: " << fileName  << ", thread id: " << QThread::currentThreadId();
    m_peInfoRef.LoadPE(reinterpret_cast<LPCTSTR>(fileName.utf16()));
    emit startShowPEInfoOnGUI();
}
