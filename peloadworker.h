#ifndef PELOADWORKER_H
#define PELOADWORKER_H

#include <QObject>
#include "PEInfo.h"

class PELoadWorker : public QObject
{
    Q_OBJECT
public:
    explicit PELoadWorker(CPEInfo &peInfo,QObject *parent = nullptr);
private:
    bool m_bPEBasicLoaded;
    QString m_strPEFileName;
    CPEInfo &m_peInfoRef;
 signals:
    void startShowPEInfoOnGUI();
public slots:
 void loadPEBasic(const QString &fileName);
};

#endif // PELOADWORKER_H
