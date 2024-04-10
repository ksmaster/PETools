#include "importTable.h"
#include <QDebug>
#include <QHeaderView>
#include <QApplication>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QScrollBar>

#include "Util.h"
#include "constants/constStrings.h"

static const int IMPORT_DLL_NAME_COL_WIDTH = 120;

importTable::importTable(const CPEInfo& _peInfo, QWidget *parent): baseTableView(parent), peInfo(_peInfo)
{  
    //this->setStyleSheet(getCssContent("importTable.css"));
    this->setSelectionMode(SelectionMode::SingleSelection);
}

QList<ColInfo>  importTable::getColsInfo()
{
    QList<ColInfo> colInfos = {
        {tr("Name"), IMPORT_DLL_NAME_COL_WIDTH, },
       
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
        
    };
    return getStandardItemList(itemStrList);
}



ImportItemListWidget::ImportItemListWidget(QWidget* parent) : ItemPairListWidget(parent, 2)
{
    initUI();
}

QList<ItemPair> ImportItemListWidget::getItemPairs()
{
    return {
        { STR_ORG_FIRST_TRUNK,        tr("OrigFirstTrunk"), },
        { STR_TIME_DATE_STAMP,        tr("TimeDateStamp"), },
        { STR_FORWARDER_CHAIN,        tr("ForwarderChain"), },
        { STR_FIRST_THUNK,            tr("FirstThunk"), },
    };
}


void ImportItemListWidget::updateItemsData(const IMAGE_IMPORT_DESCRIPTOR_Wrapper& imageSecHeader)
{
    setItemData(STR_ORG_FIRST_TRUNK, uintToHexStr(imageSecHeader.m_imgImportDesc.OriginalFirstThunk));
    setItemData(STR_TIME_DATE_STAMP, uintToHexStr(imageSecHeader.m_imgImportDesc.TimeDateStamp));
    setItemData(STR_FORWARDER_CHAIN, uintToHexStr(imageSecHeader.m_imgImportDesc.ForwarderChain));
    setItemData(STR_FIRST_THUNK, uintToHexStr(imageSecHeader.m_imgImportDesc.FirstThunk));
}




importHintNameTable::importHintNameTable(const CPEInfo& _peInfo, QWidget* parent) : baseTableView(parent), peInfo(_peInfo)
{

}

QList<ColInfo> importHintNameTable::getColsInfo()
{
    QList<ColInfo> colInfos = {
        {tr("IAT Elem rva"),         80,  },
        {tr("IAT Elem"),  100,  },
        {tr("ILT Elem"),  100,  },
        {tr("Name"),  160,  },
        {tr("Hint"),  40,  },
        {tr("Ordinal Number"),  120,  },
    };
    return colInfos;
}

QList<QStandardItem*> importHintNameTable::createRow(int index)
{
    ImageImportNameInfoWrapper imageImportNameInfoWrapper;
    if (index < 0 
        || (false== peInfo.getImageImportWrapperInfo(m_strDllName.toUtf8().constData(), imageImportNameInfoWrapper))
        || (index >= (int)imageImportNameInfoWrapper.size())
        )
    {
        return baseTableView::createRow(index);
    }
    ULONGLONG ullIATItem = 0;
    if (index < (int)imageImportNameInfoWrapper.m_vecIAT.size())
    {
        ullIATItem = imageImportNameInfoWrapper.m_vecIAT[index];
    }
    ULONGLONG ullILTItem = 0;
    if (index < (int)imageImportNameInfoWrapper.m_vecILT.size())
    {
        ullILTItem = imageImportNameInfoWrapper.m_vecILT[index];
    }
    DWORD wOrdinal = 0;
    if (index < (int)imageImportNameInfoWrapper.m_vecOrdinal.size())
    {
        wOrdinal = imageImportNameInfoWrapper.m_vecOrdinal[index];
    }
    auto& importHintName = imageImportNameInfoWrapper.m_vecHintNameTable[index];
    QStringList itemStrList{
        uintToHexStr(imageImportNameInfoWrapper.m_firstThunk + index * (peInfo.m_bX86 ? 4 : 8)),
        peInfo.m_bX86 ? uintToHexStr(ullIATItem) : uLongLongToHexStr(ullIATItem),
        peInfo.m_bX86 ? uintToHexStr(ullILTItem) : uLongLongToHexStr(ullILTItem),
        QString::fromUtf8(importHintName.strName.c_str()),
        wOrdinal < 0xFFFF ? "":ushortToHexStr(importHintName.wHint),
        wOrdinal < 0xFFFF ? ushortToHexStr(wOrdinal) : "",
    };

    return getStandardItemList(itemStrList);
}




void importHintNameTable::reloadDllHintName(const QString& strDllName)
{
    m_strDllName = strDllName;
    ImageImportNameInfoWrapper imageImportNameInfoWrapper;
    peInfo.getImageImportWrapperInfo(m_strDllName.toUtf8().constData(), imageImportNameInfoWrapper);
    reload(imageImportNameInfoWrapper.size());
}


importWidget::importWidget(const CPEInfo& _peInfo, QWidget* parent): QWidget(parent), peInfo(_peInfo), m_hImportLayout(this)
{
    m_importTbl = new importTable(_peInfo, this);
    m_importTbl->setFixedWidth(IMPORT_DLL_NAME_COL_WIDTH);
    m_hImportLayout.addWidget(m_importTbl);
    QWidget* pRightWidget = new QWidget(this);
    QVBoxLayout* pRightVLayout = new QVBoxLayout(pRightWidget);
    m_importItemList = new ImportItemListWidget(pRightWidget);
    pRightVLayout->addWidget(m_importItemList, 1);

    m_importHintNameTable = new importHintNameTable(_peInfo, pRightWidget);
    pRightVLayout->addWidget(m_importHintNameTable, 4);


    m_hImportLayout.addWidget(pRightWidget);

    connect(m_importTbl->selectionModel(), &QItemSelectionModel::selectionChanged,this, &importWidget::onSelItemChanged);
}


void importWidget::reload(int nSize)
{
    m_importTbl->reload(nSize);
    if (nSize > 0)
    {
        //set m_importTbl select the first one
        m_importTbl->setSel(0);
    }
    else
    {
        m_importItemList->clearData();
        m_importHintNameTable->hide();
    }
    
}

void importWidget::onSelItemChanged(const QItemSelection& itemSel, const QItemSelection&itemDelSel)
{
    const QModelIndex index = itemSel.indexes().at(0);
    int nRow = index.row();
    qDebug() << "nRow: " << nRow;
    auto& imageImportInfo = peInfo.getImageImportInfo();
    if ( ((int)imageImportInfo.size() > nRow) && (nRow >=0) )
    {
        m_importItemList->updateItemsData(imageImportInfo[nRow]);
        m_importHintNameTable->reloadDllHintName(QString::fromUtf8(imageImportInfo[nRow].m_strDllName.c_str()));
        //m_importHintNameTable->show();
    }
    
}
