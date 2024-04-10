#include "dllTree.h"
#include <QDebug>
#include <QScrollBar>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QDateTime>
#include <QMouseEvent>
#include <QHeaderView>
#include <QTreeView>
#include "Util.h"

#include "constants/constStrings.h"

static const int TREE_ITEM_HEIGHT = 20;
static const int TREE_ITEM_BATCH_NUM = 40;




DllTree::DllTree(QWidget *parent) : QWidget(parent), m_vLayout(this)
{
    setObjectName("DllTree");

    m_pTreeView = new QTreeView(this);
    m_treeViewModel = new QStandardItemModel();
    
    m_pTreeView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_pTreeView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_pTreeView->setModel(m_treeViewModel);
    m_pTreeView->setHeaderHidden(true);

    
    
    m_vLayout.addWidget(m_pTreeView);
    
    connect(m_pTreeView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onScrollBarChange(int)));
    
    connect(m_pTreeView, SIGNAL(collapsed(QModelIndex)), this, SLOT(onTreeViewPressed(QModelIndex)));
    connect(m_pTreeView, SIGNAL(pressed(QModelIndex)), this, SLOT(onTreeViewPressed(QModelIndex)));
    connect(m_pTreeView, SIGNAL(expanded(QModelIndex)), this, SLOT(onTreeViewPressed(QModelIndex)));  

    connect(m_pTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
        this, SLOT(selectTreeItem(const QItemSelection&, const QItemSelection&)));
    
    retranslateUi();
    
}

void DllTree::retranslateUi()
{

}

void DllTree::appendItemsFromRange(const IMAGE_EXPORT_INFO_WRAPPER& imgExportInfo, int nBeginIdx, int nEndIdx)
{
    for (int i = nBeginIdx; i < qMin(nEndIdx, m_nTotalItemCnt); ++i)
    {
        WORD wFuncNameOrdinal = 0;
        DWORD dwFuncAddr = 0;
        auto& funcName = imgExportInfo.dllInfo.m_vecFuncName[i];
        bool bRet = imgExportInfo.getFuncByName(funcName, wFuncNameOrdinal, dwFuncAddr);
        QString strFuncNameOrdinal;
        QString strFuncAddr;
        if (bRet)
        {
            strFuncNameOrdinal = ushortToHexStr(wFuncNameOrdinal);
            strFuncAddr = uintToHexStr(dwFuncAddr);
        }
        QString strItem = QString("%1 ord: %2  rva: %3").arg(QString::fromUtf8(funcName.c_str())).arg(strFuncNameOrdinal).arg(strFuncAddr);
        strItem = QString::number(i) + ": " + strItem;
        QStandardItem* item = new QStandardItem(QIcon(":/icon/fn"), strItem);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        item->setSizeHint(QSize(0, TREE_ITEM_HEIGHT));
        m_topLevelItem->appendRow(item);
    }
    
    m_pTreeView->blockSignals(true);
    m_pTreeView->expandAll();
    m_pTreeView->blockSignals(false);

}

void DllTree::updateData(const IMAGE_EXPORT_INFO_WRAPPER& imgExportInfo)
{
    clear();
    m_topLevelItem = new QStandardItem(QIcon(":/icon/module"), QString::fromUtf8(imgExportInfo.dllInfo.m_strDllName.c_str()));
    m_topLevelItem->setFlags(m_topLevelItem->flags() & ~Qt::ItemIsEditable);
    m_topLevelItem->setSizeHint(QSize(0, TREE_ITEM_HEIGHT));
    m_treeViewModel->appendRow(m_topLevelItem);
    m_nTotalItemCnt = imgExportInfo.dllInfo.m_vecFuncName.size();
    if (m_nTotalItemCnt == 0)
    {
        return;
    }
    m_pImgExportInfo = &imgExportInfo;
    appendItemsFromRange(imgExportInfo, 0, qMin(TREE_ITEM_BATCH_NUM, m_nTotalItemCnt));
    
}

void DllTree::clear()
{
    m_treeViewModel->clear();
}

void DllTree::onScrollBarChange(int value)
{
   //qDebug() << "DllTree::scrollbarchange: value: " << value << ", height: " << height()   << ", m_topLevelItem height: " << m_topLevelItem->sizeHint().height();
   // qDebug() << "scrollbar: height: " << m_pTreeView->verticalScrollBar()->height();
    int nVisibleItemCnt = m_pTreeView->verticalScrollBar()->height() / m_topLevelItem->sizeHint().height() - 1;
    int nCurItemCnt = m_topLevelItem->rowCount();
    if (m_nTotalItemCnt > nCurItemCnt)
    {
        int nRemainItemCnt = nCurItemCnt - nVisibleItemCnt;
        if (value == nRemainItemCnt)
        {
            if (m_pImgExportInfo)
            {
                int nBeginIdx = nCurItemCnt;
                int nEndIdx = nCurItemCnt + TREE_ITEM_BATCH_NUM;
                appendItemsFromRange(*m_pImgExportInfo, nBeginIdx, nEndIdx);
            }
        }
    }
}


void DllTree::onTreeViewPressed(QModelIndex modeIndex)
{
    m_pTreeView->resizeColumnToContents(modeIndex.column());
}


void DllTree::selectTreeItem(const QItemSelection& selected, const QItemSelection& deselected)
{
    const QModelIndex index = selected.indexes().at(0);
    
    QStandardItem* selItem = m_treeViewModel->itemFromIndex(index);
   // qDebug() << "selectTreeItem: row: " << index.row() << ", col: " << index.column()   << ", text: " << selItem->text();
    const QModelIndexList deSelIndices = deselected.indexes();
    if (!deSelIndices.isEmpty())
    {
        const QModelIndex deSelIndex = deSelIndices[0];
        QStandardItem* deselItem = m_treeViewModel->itemFromIndex(deSelIndex);
        //qDebug() << "deselectedTreeItem: row: " << deSelIndex.row() << ", col: " << deSelIndex.column() << ", text: " << deselItem->text();
        
    }
    
}

 

