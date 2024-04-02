#include "importTable.h"
#include <QDebug>
#include <QHeaderView>
#include <QApplication>
#include <QStyle>
#include <QPainter>
#include <QStandardItemModel>
#include <QScrollBar>
#include "Util.h"

 

importTable::importTable(const CPEInfo& _peInfo, QWidget *parent): baseTableView(parent), peInfo(_peInfo)
{  
    //this->setStyleSheet(getCssContent("importTable.css"));
}

QList<ColInfo>  importTable::getColsInfo()
{
    QList<ColInfo> colInfos = {
        {tr("Name"), 120, },
        {tr("OrigFirstTrunk"), 120, },
        {tr("TimeDateStamp"), 120, },
        {tr("ForwarderChain"), 120, },
        {tr("FirstThunk"), 120, },
    };    
    return colInfos; 
}  

QList<QStandardItem*> importTable::createRow(int index)
{
    auto& vecImgSectionHeader = peInfo.getImageImportInfo();
    if (index < 0 || index >= (int)vecImgSectionHeader.size())
    {
        return baseTableView::createRow(index);
    }
    auto &imageSecHeader = vecImgSectionHeader[index];
    QStringList itemStrList{
        QString::fromUtf8(imageSecHeader.m_strDllName.c_str()),
        uintToHexStr(imageSecHeader.m_imgImportDesc.OriginalFirstThunk),
        uintToHexStr(imageSecHeader.m_imgImportDesc.TimeDateStamp),
        uintToHexStr(imageSecHeader.m_imgImportDesc.ForwarderChain),
        uintToHexStr(imageSecHeader.m_imgImportDesc.FirstThunk),
    };
    return getStandardItemList(itemStrList);
}


