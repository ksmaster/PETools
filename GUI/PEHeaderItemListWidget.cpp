#include "PEHeaderItemListWidget.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include <algorithm>
#include <vector>
#include "common/qbasiclabel.h"
#include "Util.h"
#include "globaldata.h"

 

DllCharacteristicsTipWidget::DllCharacteristicsTipWidget(QWidget* parent): BaseWindow(parent), m_gridLayout(this)
{
    std::vector<int> a;
    this->setFixedSize(360, 230);
    m_gridLayout.setContentsMargins(10, 5, 0, 5);
    m_gridLayout.setSpacing(0);
    m_chkDynamicBase = new QCheckBox(this);
    m_gridLayout.addWidget(m_chkDynamicBase, 0, 0);

    m_chkDEP = new QCheckBox(this);
    m_gridLayout.addWidget(m_chkDEP, 0, 1);

    m_chkNoSEH = new QCheckBox(this);
    m_gridLayout.addWidget(m_chkNoSEH, 1, 0);

    m_chkCFG = new QCheckBox(this);
    m_gridLayout.addWidget(m_chkCFG, 1, 1);

    m_chkForceIntegrity = new QCheckBox(this);
    m_gridLayout.addWidget(m_chkForceIntegrity, 2, 0);

    m_chkNoIsloation = new QCheckBox(this);
    m_gridLayout.addWidget(m_chkNoIsloation, 2, 1);

    m_chkWDM = new QCheckBox(this);
    m_gridLayout.addWidget(m_chkWDM, 3, 0);

    m_chkNoBind = new QCheckBox(this);
    m_gridLayout.addWidget(m_chkNoBind, 3, 1);

    m_chkTerminalServerAware = new QCheckBox(this);
    m_gridLayout.addWidget(m_chkTerminalServerAware, 4, 0);
   
    retranslateUi();
}

void DllCharacteristicsTipWidget::updateData(int nDllCharacteristics)
{
//#define IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA    0x0020  // Image can handle a high entropy 64-bit virtual address space.
//#define IMAGE_DLLCHARACTERISTICS_APPCONTAINER 0x1000     // Image should execute in an AppContainer
    m_chkDynamicBase->setChecked(nDllCharacteristics & IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE);
    m_chkForceIntegrity->setChecked(nDllCharacteristics & IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY);
    m_chkDEP->setChecked(nDllCharacteristics & IMAGE_DLLCHARACTERISTICS_NX_COMPAT);
    m_chkNoIsloation->setChecked(nDllCharacteristics & IMAGE_DLLCHARACTERISTICS_NO_ISOLATION);

    // Image does not use SEH.  No SE handler may reside in this image
    m_chkNoSEH->setChecked(nDllCharacteristics & IMAGE_DLLCHARACTERISTICS_NO_SEH);

    m_chkNoBind->setChecked(nDllCharacteristics & IMAGE_DLLCHARACTERISTICS_NO_BIND);// Do not bind this image.
    m_chkWDM->setChecked(nDllCharacteristics & IMAGE_DLLCHARACTERISTICS_WDM_DRIVER);// Driver uses WDM model
    m_chkCFG->setChecked(nDllCharacteristics & IMAGE_DLLCHARACTERISTICS_GUARD_CF);//Image supports Control Flow Guard.
    m_chkTerminalServerAware->setChecked(nDllCharacteristics& IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE);
}

void DllCharacteristicsTipWidget::retranslateUi()
{
    this->setWindowTitle(tr("Dll Characteristics"));
    m_chkDynamicBase->setText(tr("Dynamic Base"));
    m_chkForceIntegrity->setText(tr("Force Integrity"));
    m_chkDEP->setText(tr("Nx Compatible(DEP Compatible)"));
    m_chkNoIsloation->setText(tr("No Isloation"));
    m_chkNoSEH->setText(tr("No SEH"));
    m_chkNoBind->setText(tr("No Bind"));
    m_chkWDM->setText(tr("WDM"));
    m_chkCFG->setText(tr("Control Flow Guard"));
    m_chkTerminalServerAware->setText(tr("Terminal Server Aware"));
}




PEHeaderItemWindow::PEHeaderItemWindow(const QString& strItemName, QWidget* parent): HoverWindow(parent), m_layout(this)
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

void PEHeaderItemWindow::setNameLblText(const QString& str)
{
    item->setText(str);
}
void PEHeaderItemWindow::setValueLblText(const QString& str)
{
    data->setText(str);
}

PEHeaderItemWindow::~PEHeaderItemWindow()
{
    //qDebug() << "~PEHeaderItemWindow";
}


void PEHeaderItemWindow::onHoverEvent(QMouseEvent* ev)
{
    if (m_tooltipWindow)
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

void PEHeaderItemWindow::onHoverLeave()
{
    if (m_tooltipWindow)
    {
        if (m_tooltipWindow->isVisible())
        {
            m_tooltipWindow->hide();
        }
    }
}


PEHeaderItemListWidget::PEHeaderItemListWidget( QWidget *parent) : QWidget(parent), m_vboxLayout(this)
{
    m_vboxLayout.setContentsMargins(0,0,0,0);
    
    for(auto &itemPair: getPEHeaderItemPairs())
    {
        PEHeaderItemWindow* item = new PEHeaderItemWindow(itemPair.value, this);
        m_mapPEHeader[itemPair.key] = item;
        m_vboxLayout.addWidget(item);
    }
}

QList<ItemPair> PEHeaderItemListWidget::getPEHeaderItemPairs()
{
    QList<ItemPair> peHeaderItemsPairs = {
        { "ImageBase",            tr("ImageBase"), },
        { "EntryPoint(RVA)", tr("EntryPoint(RVA)"), },
        { "EntryPoint(RAW)",      tr("EntryPoint(RAW)"), },
        { "DllCharacteristics",   tr("DllCharacteristics"), },
        { "Characteristics",      tr("Characteristics"), },
        { "SubSystem",            tr("SubSystem"), },
        { "Machine",              tr("Machine"), },
        { "SizeOfImage",          tr("SizeOfImage"), },
        { "SectionAlignment",     tr("SectionAlignment"), },
        { "FileAlignment",        tr("FileAlignment"), },
        { "NumberOfSections",     tr("NumberOfSections"), },
        { "SizeOfHeaders",        tr("SizeOfHeaders"), },
        { "CheckSum",             tr("CheckSum"), },
        
        
    };
    return peHeaderItemsPairs;
}


void PEHeaderItemListWidget::updatePEHeaderItemLbls()
{
    for (auto &item : getPEHeaderItemPairs())
    {
        this->updateItemLblText(item.key, item.value);
    }
}

void PEHeaderItemListWidget::updateItemsData(const CPEInfo& peInfo)
{
    DWORD dwEntryPointRVA = peInfo.getEntryPointRVA();
    updateItemData("EntryPoint(RVA)", uintToHexStr(dwEntryPointRVA));
    updateItemData("EntryPoint(RAW)", uintToHexStr(peInfo.rvaToFoa(dwEntryPointRVA)));
    updateItemData("ImageBase", peInfo.m_bX86 ? uintToHexStr(peInfo.getImageBaseX86()) : uLongLongToHexStr(peInfo.getImageBaseX64()));
    updateItemData("SizeOfImage", uintToHexStr(peInfo.getSizeOfImage()));
    updateItemData("SectionAlignment", uintToHexStr(peInfo.getSectionAlignment()));
    updateItemData("FileAlignment", uintToHexStr(peInfo.getFileAlignment()));
    updateItemData("NumberOfSections", ushortToHexStr(peInfo.getNumberOfSections()));
    updateItemData("Characteristics", ushortToHexStr(peInfo.getCharacteristics()));
    updateItemData("SizeOfHeaders", uintToHexStr(peInfo.getCoffHeaderSize()));
    updateItemData("CheckSum", uintToHexStr(peInfo.getCheckSum()));
    updateItemData("Machine", ushortToHexStr(peInfo.getMachine()));
    updateItemData("SubSystem", ushortToHexStr(peInfo.getSubSystem()));
    updateItemData("DllCharacteristics", ushortToHexStr(peInfo.getDllCharacteristics()));
}

void PEHeaderItemListWidget::updateItemData(const QString& itemName, const QString& data)
{
    if (m_mapPEHeader.contains(itemName))
    {
        auto& item = m_mapPEHeader[itemName];
        if (item)
        {
            item->setValueLblText(data);
            if ("DllCharacteristics" == itemName)
            {
                if (!m_pDllCharacteristicsTipWidget)
                {
                    m_pDllCharacteristicsTipWidget = new DllCharacteristicsTipWidget(this);
                    m_pDllCharacteristicsTipWidget->setWindowFlags(Qt::Tool);
                    m_pDllCharacteristicsTipWidget->hide();
                    item->setToolTip(m_pDllCharacteristicsTipWidget);
                }
                bool ok;
                int nDllCharacteristics = data.toInt(&ok, 16);
                if (ok) {
                    m_pDllCharacteristicsTipWidget->updateData(nDllCharacteristics);
                }
                else {
                    qDebug() << "bad hex value for dll characteristics";
                }
                
            }
        }
    }
}

void PEHeaderItemListWidget::updateItemLblText(const QString& itemName, const QString& str)
{
    if (m_mapPEHeader.contains(itemName))
    {
        auto& item = m_mapPEHeader[itemName];
        if (item)
        {
            item->setNameLblText(str);
        }
    }
}


PEHeaderItemListWidget::~PEHeaderItemListWidget()
{
    
}

