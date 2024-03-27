#include "itemPairListWidget.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include <algorithm>
#include <vector>
#include "common/qbasiclabel.h"
#include "Util.h"


ItemPairWindow::ItemPairWindow(const QString& strItemName, QWidget* parent): HoverWindow(parent), m_layout(this)
{
    m_layout.setContentsMargins(0, 0, 0, 0);
    QLabel* lblPEHeaderItem = new QBasicLabel(this);
    int nPEHeaderItemHeight = 30;
    lblPEHeaderItem->setObjectName("peHeaderLbl");
    lblPEHeaderItem->setFixedSize(100, nPEHeaderItemHeight);
    //lblPEHeaderItem->setMouseTracking(true);
    m_layout.addWidget(lblPEHeaderItem);

    QLabel* lblName = new QLabel(strItemName, this);
    lblName->setFixedSize(140, nPEHeaderItemHeight);
    lblName->setTextInteractionFlags(Qt::TextSelectableByMouse);
    //lblName->setMouseTracking(true);
    item = lblName;
    data = lblPEHeaderItem;
    m_layout.addWidget(lblName);
}

void ItemPairWindow::setNameLblText(const QString& str)
{
    item->setText(str);
}
void ItemPairWindow::setValueLblText(const QString& str)
{
    data->setText(str);
}

ItemPairWindow::~ItemPairWindow()
{
    //qDebug() << "~PEHeaderItemWindow";
}


void ItemPairWindow::onHoverEvent(QMouseEvent* ev)
{
    if (m_tooltipWindow && (!data->text().isEmpty()))
    {
        if (!m_tooltipWindow->isVisible())
        {
            QWidget* parent = parentWidget();
            if (!parent)
            {
                return;
            }
            QPoint globalPt = parent->mapToGlobal(pos());
            int nOffset = 10;
            QPoint ptOffset(-nOffset, nOffset);
            
            int xPos = globalPt.x() - m_tooltipWindow->width() + ptOffset.x();
            int yPos = globalPt.y() + ptOffset.y();
            QDesktopWidget* desktop = QApplication::desktop();
            QRect screenGeometry = desktop->screenGeometry();
            int screenWidth = screenGeometry.width();
            int screenHeight = screenGeometry.height();
            if (xPos <= 0)
            {
                xPos = globalPt.x();
                if (xPos < nOffset)
                {
                    xPos = nOffset;
                }
                yPos = globalPt.y() - m_tooltipWindow->height() - height() - nOffset;
                if (yPos < 0)
                {
                    yPos = globalPt.y() + height() + nOffset;
                }
            }
            else
            {
                if (xPos >= (screenWidth - m_tooltipWindow->width()))
                {
                    xPos = globalPt.x() - m_tooltipWindow->width() - nOffset;
                }
                if (yPos <= 0)
                {
                    yPos = globalPt.y() - m_tooltipWindow->height() - nOffset;
                    if (yPos < nOffset)
                    {
                        yPos = nOffset;
                    }
                }
                else
                {
                    int nBottomWinToolbarHeight = 40;
                    if (yPos >= (screenHeight - m_tooltipWindow->height() - nOffset - nBottomWinToolbarHeight))
                    {
                        yPos = globalPt.y() - m_tooltipWindow->height() - nOffset;
                    }
                }
                
            }
            QPoint pt(xPos, yPos);
            m_tooltipWindow->move(pt);
            m_tooltipWindow->show();
        }
    }
}

void ItemPairWindow::onHoverLeave()
{
    if (m_tooltipWindow)
    {
        if (m_tooltipWindow->isVisible())
        {
            m_tooltipWindow->hide();
        }
    }
}


ItemPairListWidget::ItemPairListWidget(QWidget* parent, int nCol) : BaseWindow(parent), m_nCol(nCol), m_vboxLayout(this)
{
    m_vboxLayout.setContentsMargins(0, 0, 0, 0);
}

void ItemPairListWidget::retranslateUi()
{
    refreshAllItemsLblText();
}

void ItemPairListWidget::initUI()
{
    int nColIdx = 0;
    int nLineIdx = 0;
    int nTotalCol = m_nCol;
    if (nTotalCol <= 0)
    {
        nTotalCol = 1;
    }
    for (auto& itemPair : getItemPairs())
    {
        ItemPairWindow* item = new ItemPairWindow(itemPair.value, this);
        m_mapItems[itemPair.key] = item;
        m_vboxLayout.addWidget(item, nLineIdx, nColIdx);
        ++nColIdx;
        if (nColIdx >= nTotalCol)
        {
            ++nLineIdx;
            nColIdx = 0;
        }
    }
}



void ItemPairListWidget::refreshAllItemsLblText()
{
    for (auto& item : getItemPairs())
    {
        this->setItemLblText(item.key, item.value);
    }
}

void ItemPairListWidget::clearData()
{
    for (auto& item : m_mapItems.values())
    {
        item->setValueLblText("");
    }
}

void ItemPairListWidget::setItemData(const QString& itemName, const QString& data)
{
    if (m_mapItems.contains(itemName))
    {
        auto& item = m_mapItems[itemName];
        if (item)
        {
            item->setValueLblText(data);
        }
    }
}

void ItemPairListWidget::setItemTooltip(const QString& itemName, QWidget* tooltip)
{
    if (m_mapItems.contains(itemName))
    {
        auto& item = m_mapItems[itemName];
        if (item && tooltip)
        {
            item->setToolTip(tooltip);
        }
    }
}

void ItemPairListWidget::setItemLblText(const QString& itemName, const QString& str)
{
    if (m_mapItems.contains(itemName))
    {
        auto& item = m_mapItems[itemName];
        if (item)
        {
            item->setNameLblText(str);
        }
    }
}


