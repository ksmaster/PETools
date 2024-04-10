#include <QDebug>
#include <QHeaderView>
#include <QApplication>
#include <QStandardItem>
#include <QStyle>
#include <QStandardItemModel>
#include <QScrollBar>
#include <QToolTip>
#include "baseTableView.h"
#include "Util.h"

 

baseTableView::baseTableView(QWidget *parent): QTableView(parent)
{  
    m_model = new QStandardItemModel();
    this->setModel(m_model);
    this->horizontalScrollBar()->setDisabled(true);
    this->horizontalScrollBar()->hide();
    QScrollBar* verticalScrollbar = this->verticalScrollBar();
    //verticalScrollbar->setStyleSheet(getCssContent(":/css/scrollBar.css"));
    verticalScrollbar->setFixedWidth(10);
    //this->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //this->horizontalHeader()->setStretchLastSection(false);
    //this->setStyleSheet(getCssContent("baseTableView.css"));
    this->setMouseTracking(true);
    this->setWordWrap(false);
    connect(this, &QTableView::entered, this, &baseTableView::showToolTip);
}

QList<ColInfo>  baseTableView::getColsInfo()
{
    QList<ColInfo> colInfos = {
       
    };    
    return colInfos; 
}  

QList<QStandardItem*> baseTableView::createRow(int index)
{
    QList<QStandardItem*> row = {

    };
    return row;
}

QList<QStandardItem*> baseTableView::getStandardItemList(const QStringList& itemStrList)
{
    QList<QStandardItem*> items;
    for (auto &itemStr : itemStrList)
    {
        QStandardItem* item = new QStandardItem(itemStr);
      //  item->setData(Qt::AlignLeft|Qt::AlignVCenter, Qt::TextAlignmentRole);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        items.append(item);
    }
    return items;
}

void baseTableView::preSetItemDelegate()
{

}
 
void baseTableView::reload(int nRowCnt)
{
    m_model->clear();
    QStringList headStrList;
    QList<ColInfo> colsInfo = getColsInfo();
    for (auto& colInfo : colsInfo)
    {
        headStrList << colInfo.strHead;
    }
    m_model->setHorizontalHeaderLabels(headStrList);
    int nIdx = 0;
    
    for (auto& colInfo : colsInfo)
    {
        setColumnWidth(nIdx++, colInfo.nColWidth);
    }

    verticalHeader()->setDefaultSectionSize(24);
    verticalHeader()->hide();
    horizontalHeader()->setDefaultAlignment(Qt::AlignLeft| Qt::AlignVCenter);
    preSetItemDelegate();
    for (int i = 0; i < nRowCnt; ++i)
    {
        auto& item = createRow(i);
        if (item.length() > 0)
        {
            m_model->appendRow(item);
         }
    }   
}


void baseTableView::setSel(int nRow)
{
    QModelIndex index = m_model->index(nRow, 0);
    selectionModel()->select(index, QItemSelectionModel::Select);
}

void baseTableView::showToolTip(const QModelIndex& index) {

    if (!index.isValid()) {

        qDebug() << "Invalid index";

        return;

    }

    QToolTip::showText(QCursor::pos(), index.data().toString());

}