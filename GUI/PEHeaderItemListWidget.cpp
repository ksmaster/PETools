#include "PEHeaderItemListWidget.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include <algorithm>
#include <vector>
#include "common/qbasiclabel.h"
#include "constants/constStrings.h"
#include "Util.h"
#include "PEInfo.h"

 

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


static const QString STR_DLL_CHARACTERISTICS = "DllCharacteristics";


PEHeaderItemListWidget::PEHeaderItemListWidget( QWidget *parent) : ItemPairListWidget(parent)
{
    initUI();
}

QList<ItemPair> PEHeaderItemListWidget::getItemPairs()
{
    QList<ItemPair> peHeaderItemsPairs = {
        { "ImageBase",            tr("ImageBase"), },
        { "EntryPoint(RVA)", tr("EntryPoint(RVA)"), },
        { "EntryPoint(RAW)",      tr("EntryPoint(RAW)"), },
        { STR_DLL_CHARACTERISTICS,   tr("DllCharacteristics"), },
        { STR_CHARACTERISTICS,      tr("Characteristics"), },
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


void PEHeaderItemListWidget::updateItemsData(const CPEInfo& peInfo)
{
    DWORD dwEntryPointRVA = peInfo.getEntryPointRVA();
    setItemData("EntryPoint(RVA)", uintToHexStr(dwEntryPointRVA));
    setItemData("EntryPoint(RAW)", uintToHexStr(peInfo.rvaToFoa(dwEntryPointRVA)));
    setItemData("ImageBase", peInfo.m_bX86 ? uintToHexStr(peInfo.getImageBaseX86()) : uLongLongToHexStr(peInfo.getImageBaseX64()));
    setItemData("SizeOfImage", uintToHexStr(peInfo.getSizeOfImage()));
    setItemData("SectionAlignment", uintToHexStr(peInfo.getSectionAlignment()));
    setItemData("FileAlignment", uintToHexStr(peInfo.getFileAlignment()));
    setItemData("NumberOfSections", ushortToHexStr(peInfo.getNumberOfSections()));
    setItemData("Characteristics", ushortToHexStr(peInfo.getCharacteristics()));
    setItemData("SizeOfHeaders", uintToHexStr(peInfo.getCoffHeaderSize()));
    setItemData("CheckSum", uintToHexStr(peInfo.getCheckSum()));
    setItemData("Machine", ushortToHexStr(peInfo.getMachine()));
    setItemData("SubSystem", ushortToHexStr(peInfo.getSubSystem()));
    int nDllCharacteristics = peInfo.getDllCharacteristics();
    setItemData(STR_DLL_CHARACTERISTICS, ushortToHexStr(nDllCharacteristics));

    if (!m_pDllCharacteristicsTipWidget)
    {
        m_pDllCharacteristicsTipWidget = new DllCharacteristicsTipWidget(this);
        m_pDllCharacteristicsTipWidget->setWindowFlags(Qt::Tool);
        m_pDllCharacteristicsTipWidget->hide();
        setItemTooltip(STR_DLL_CHARACTERISTICS, m_pDllCharacteristicsTipWidget);
    }
    m_pDllCharacteristicsTipWidget->updateData(nDllCharacteristics);
}


