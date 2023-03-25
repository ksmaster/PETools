#include "peloadworker.h"
#include <QThread>
#include <QDebug>

PELoadWorker::PELoadWorker(CPEInfo &peInfo, QObject *parent) : QObject(parent),m_bPEBasicLoaded(false),m_peInfoRef(peInfo)
{

}


void PELoadWorker::loadPEBasic(const QString &fileName)
{
    qDebug() << "loadPEBasic: " << fileName  << ", thread id: " << QThread::currentThreadId();
#if defined(WIN32)
     m_peInfoRef.LoadPE(reinterpret_cast<LPCTSTR>(fileName.utf16()));
#elif __linux__
     qDebug() << "before calling:  fileName length: " << fileName.length();
     m_peInfoRef.LoadPE(fileName.toUtf8().constData());
#endif
    emit startShowPEInfoOnGUI();
}
