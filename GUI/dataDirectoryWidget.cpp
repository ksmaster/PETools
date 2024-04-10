#include "dataDirectoryWidget.h"
#include <QDebug>
#include <QHeaderView>
#include <QScrollBar>
#include <QStandardItemModel>
#include "common/qbasiclabel.h"
#include "Util.h"



DataDirectoryWidget::DataDirectoryWidget(const CPEInfo& _peInfo, QWidget *parent): baseTableView(parent), peInfo(_peInfo)
{
   
}

QList<ColInfo> DataDirectoryWidget::getColsInfo()
{
    QList<ColInfo> colInfos = {
        {"",         160,  },
        {tr("RVA"),  120,  },
        {tr("FOA"),  120,  },
        {tr("Size"), 120,  },
    };
    return colInfos;
}

QList<QStandardItem*> DataDirectoryWidget::createRow(int index)
{
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
    if (index >= strDataDirectoryList.length())
    {
        return baseTableView::createRow(index);
    }

    const IMAGE_DATA_DIRECTORY* pDataDir = peInfo.getDataDirectory(index);
    if (nullptr == pDataDir) {
        return baseTableView::createRow(index);
    }

    QString strName;
    if (index >= 0 && index < strDataDirectoryList.length())
    {
        strName = strDataDirectoryList[index];
    }

    QStringList dataDirectoryItemStrList{
        strName,
        uintToHexStr(pDataDir->VirtualAddress),
        uintToHexStr(peInfo.rvaToFoa(pDataDir->VirtualAddress)),
        uintToHexStr(pDataDir->Size),
    };
    return getStandardItemList(dataDirectoryItemStrList);
}
