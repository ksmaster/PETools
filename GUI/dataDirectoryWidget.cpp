#include "dataDirectoryWidget.h"
#include <QDebug>
#include <QHeaderView>
#include <QScrollBar>
#include <QStandardItemModel>
#include "common/qbasiclabel.h"
#include "Util.h"



DataDirectoryWidget::DataDirectoryWidget(QWidget *parent): QTableView(parent)
{
    m_model = new QStandardItemModel();
    this->setModel(m_model);
    this->horizontalScrollBar()->setDisabled(true);
    this->horizontalScrollBar()->hide();
    QScrollBar* verticalScrollbar = this->verticalScrollBar();
    //verticalScrollbar->setStyleSheet(getCssContent(":/css/scrollBar.css"));
    verticalScrollbar->setFixedWidth(10);
}


void DataDirectoryWidget::setHeader()
{
    m_model->clear();
    QStringList headStrList;
    headStrList << QStringLiteral("") << tr("RVA") << tr("FOA") << tr("Size");
    m_model->setHorizontalHeaderLabels(headStrList);
    int nWidth = 120;
    setColumnWidth(0, nWidth + 20);
    setColumnWidth(1, nWidth);
    setColumnWidth(2, nWidth);
    setColumnWidth(3, nWidth);
    verticalHeader()->setDefaultSectionSize(24);
    verticalHeader()->hide();
    horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
   
}

void DataDirectoryWidget::addDataDirectory(const QString& strName, const CPEInfo& peInfo, const IMAGE_DATA_DIRECTORY& imgDataDirectory)
{
    QList<QStandardItem*> dataDirectoryitems;
    QStringList dataDirectoryItemStrList{
        strName,
        uintToHexStr(imgDataDirectory.VirtualAddress),
        uintToHexStr(peInfo.rvaToFoa(imgDataDirectory.VirtualAddress)),
        uintToHexStr(imgDataDirectory.Size),
    };
    for (auto itemStr : dataDirectoryItemStrList)
    {
        QStandardItem* item = new QStandardItem(itemStr);
        item->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        dataDirectoryitems.append(item);
    }
    m_model->appendRow(dataDirectoryitems);
    int nWidth = 120;
    setColumnWidth(0, nWidth + 20);
    setColumnWidth(1, nWidth);
    setColumnWidth(2, nWidth);
    setColumnWidth(3, nWidth);
}

void DataDirectoryWidget::reloadAllData(const CPEInfo& peInfo)
{
    //show datadirectory index
    //IMAGE_DIRECTORY_ENTRY_EXPORT
    DWORD dwDataDirElemCnt = peInfo.getDataDirectoryElemCnt();
    QStringList strDataDirectoryList = {
        tr("Export"),
        tr("Import"),
        tr("Resource"),
        tr("Exception"),
        tr("Security"),
        tr("BaseRelocation"),
        tr("Debug"),
        tr("CopyrightOrArchitecture"),
        tr("Globalptr"),
        tr("Tls"),
        tr("LoadCfg"),
        tr("BoundImport"),
        tr("IAT"),
        tr("DelayImport"),
        tr("ComDescriptor"),
        //tr("Reserved"),
    };
    setHeader();
    if (dwDataDirElemCnt > (DWORD)strDataDirectoryList.length())
    {
        dwDataDirElemCnt = strDataDirectoryList.length();
    }
    for (WORD idx = 0; idx < dwDataDirElemCnt; ++idx) {
        const IMAGE_DATA_DIRECTORY* pDataDir = peInfo.getDataDirectory(idx);
        if (nullptr == pDataDir) {
            return;
        }
        this->addDataDirectory(strDataDirectoryList[idx], peInfo, *pDataDir);
    }
}



DataDirectoryWidget::~DataDirectoryWidget()
{
}

