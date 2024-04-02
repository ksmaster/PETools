#include "sectionTable.h"
#include <QDebug>
#include <QHeaderView>
#include <QApplication>
#include <QStyle>
#include <QPainter>
#include <QStandardItemModel>
#include <QScrollBar>
#include "Util.h"

static const int SECTION_CHARACTERISTICS = 7;

QMap<int, QString> priv_map{
    {2, ":/icon/Execute"},
    {4, ":/icon/Read"},
    {8, ":/icon/Write"},
    {6, ":/icon/RX"},
    {10, ":/icon/WX"},
    {12, ":/icon/RW"},
    {14, ":/icon/RWX"},
};

void IconButtonDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    if (index.column() == SECTION_CHARACTERISTICS) {
        const QRect r = option.rect;
        QString strData = qvariant_cast<QString>(index.data(Qt::EditRole));
        bool ok;
        qulonglong nCharacteristics = strData.toULongLong(&ok, 16);
        if (ok && (nCharacteristics > 0)) {
            int nFlags = 0;
            if (nCharacteristics & IMAGE_SCN_MEM_EXECUTE)
            {
                nFlags |= 2;
            }
            if (nCharacteristics & IMAGE_SCN_MEM_READ)
            {
                nFlags |= 4;
            }
            if (nCharacteristics & IMAGE_SCN_MEM_WRITE)
            {
                nFlags |= 8;
            }
            if (priv_map.contains(nFlags))
            {
                QString strPrivImg = priv_map[nFlags];
                QPixmap pixmap(strPrivImg);
                const QPoint pt = QPoint((r.width() - pixmap.width()) / 2, (r.height() - pixmap.height()) / 2);
                painter->drawPixmap(r.topLeft() + pt, pixmap);
            }
        }
        else {
            qDebug() << "bad hex value for dll characteristics: " << strData;
            QStyledItemDelegate::paint(painter, option, index);
        }
    }
    else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QSize IconButtonDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    if (index.column() == 0) {
        return QStyledItemDelegate::sizeHint(option, index);
    }
    else {
        return QStyledItemDelegate::sizeHint(option, index);
    }
}

void IconButtonDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
    QStyledItemDelegate::setModelData(editor, model, index);
}
 

sectionTable::sectionTable(QWidget *parent): baseTableView(parent)
{  
    this->setStyleSheet(getCssContent("sectionTable.css"));
}

QList<ColInfo>  sectionTable::getColsInfo()
{
    QList<ColInfo> colInfos = {
        {tr("Index"), 50, },
        {tr("Name"), 100, },
        {tr("Virtual Offset"), 100, },
        {tr("Virtual Size"), 80, },
        {tr("RAW Offset"), 80, },
        {tr("RAW Size"), 80, },
        {tr("Characteristics"), 100, }, 
        {"", 50,},
    };    
    return colInfos; 
}  

QList<QStandardItem*> sectionTable::createRow(int index)
{
    if (index < 0 || index >= (int)m_vecImgSectionHeader.size())
    {
        return baseTableView::createRow(index);
    }
    auto &imageSecHeader = m_vecImgSectionHeader[index];
    QStringList itemStrList{
        QString::number(index),
        QString::fromUtf8((const char*)imageSecHeader.Name, IMAGE_SIZEOF_SHORT_NAME),
        uintToHexStr(imageSecHeader.VirtualAddress),
        uintToHexStr(imageSecHeader.Misc.VirtualSize),
        uintToHexStr(imageSecHeader.PointerToRawData),
        uintToHexStr(imageSecHeader.SizeOfRawData),
        uintToHexStr(imageSecHeader.Characteristics),
        uintToHexStr(imageSecHeader.Characteristics),//to draw image for Characteristics
    };

    return getStandardItemList(itemStrList);
    
}


void sectionTable::preSetItemDelegate()
{
    if (!m_myStandardDelegate)
    {
        m_myStandardDelegate = new IconButtonDelegate(this);
    }
    this->setItemDelegateForColumn(SECTION_CHARACTERISTICS, m_myStandardDelegate);
}
 
void sectionTable::reloadSections(const std::vector<IMAGE_SECTION_HEADER>& imageSectionHeaders)
{
    m_vecImgSectionHeader = imageSectionHeaders;
    reload(m_vecImgSectionHeader.size());
}

