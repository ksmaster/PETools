#include "dllTree.h"
#include <QDebug>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QDateTime>
#include <QTreeView>
#include "Util.h"

#include "constants/constStrings.h"



DllTree::DllTree(QWidget *parent) : QWidget(parent), m_vLayout(this)
{
    setObjectName("DllTree");

    m_pTreeView = new QTreeView(this);
    m_treeViewModel = new QStandardItemModel();
    
    // 填充数据模型
    

    m_pTreeView->setModel(m_treeViewModel);
    m_pTreeView->setHeaderHidden(true);
    m_vLayout.addWidget(m_pTreeView);


    connect(m_pTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
        this, SLOT(selectTreeItem(const QItemSelection&, const QItemSelection&)));

    retranslateUi();
    
}


void DllTree::retranslateUi()
{

}

void DllTree::updateData(const IMAGE_EXPORT_INFO_WRAPPER& imgExportInfo)
{
    m_treeViewModel->clear();
    QStandardItem* topLevelItem = new QStandardItem(QIcon(":/icon/module"), QString::fromUtf8(imgExportInfo.m_strDllName.c_str()));
    topLevelItem->setFlags(topLevelItem->flags() & ~Qt::ItemIsEditable);
    m_treeViewModel->appendRow(topLevelItem);
    for (auto& funcName : imgExportInfo.m_vecFuncName)
    {
        WORD wFuncNameOrdinal = 0;
        DWORD dwFuncAddr = 0;
        bool bRet = imgExportInfo.getFuncByName(funcName, wFuncNameOrdinal, dwFuncAddr);
        QString strFuncNameOrdinal;
        QString strFuncAddr;
        if (bRet)
        {
            strFuncNameOrdinal = ushortToHexStr(wFuncNameOrdinal);
            strFuncAddr = uintToHexStr(dwFuncAddr);
        }
        QString strItem = QString("%1 ord: %2  rva: %3").arg(QString::fromUtf8(funcName.c_str())).arg(strFuncNameOrdinal).arg(strFuncAddr);
        QStandardItem* item = new QStandardItem(QIcon(":/icon/fn"), strItem);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        topLevelItem->appendRow(item);
    }
}


void DllTree::selectTreeItem(const QItemSelection& selected, const QItemSelection& deselected)
{
    const QModelIndex index = selected.indexes().at(0);
    
    QStandardItem* selItem = m_treeViewModel->itemFromIndex(index);
    qDebug() << "selectTreeItem: row: " << index.row() << ", col: " << index.column()
        << ", text: " << selItem->text();
    const QModelIndexList deSelIndices = deselected.indexes();
    if (!deSelIndices.isEmpty())
    {
        const QModelIndex deSelIndex = deSelIndices[0];
        QStandardItem* deselItem = m_treeViewModel->itemFromIndex(deSelIndex);
        qDebug() << "deselectedTreeItem: row: " << deSelIndex.row() << ", col: " << deSelIndex.column() 
            << ", text: " << deselItem->text();
        
    }
    
}

 

