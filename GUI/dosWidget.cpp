#include "dosWidget.h"
#include <QDebug>
#include "Util.h"



DosWidget::DosWidget(QWidget *parent) :
    BaseWindow(parent),
    m_hLayout(this)
{
    setObjectName("dosWidget");
    m_hLayout.setContentsMargins(10, 0, 10, 0);
    m_hLayout.setSpacing(0);
    QWidget* pDosLeftWidget = new QWidget(this);
    pDosLeftWidget->setObjectName("dosLeftWidget");
    pDosLeftWidget->setFixedWidth(400);
    QVBoxLayout* vLeftLayout = new QVBoxLayout(pDosLeftWidget);
    vLeftLayout->setContentsMargins(0, 0, 0, 0);
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(0);
   
    m_lblSizeOfHeader = new QLabel(this);
    gridLayout->addWidget(m_lblSizeOfHeader, 0, 0);
    m_lblSizeOfHeaderData = new QLabel(this);
    gridLayout->addWidget(m_lblSizeOfHeaderData, 0, 1);
    m_lblEntryPoint = new QLabel(this);
    gridLayout->addWidget(m_lblEntryPoint, 1, 0);

    m_lblEntryPointData = new QLabel(this);
    gridLayout->addWidget(m_lblEntryPointData, 1,1);

    m_lblRelocation = new QLabel(this);
    gridLayout->addWidget(m_lblRelocation, 2, 0);

    m_lblRelocationData = new QLabel(this);
    gridLayout->addWidget(m_lblRelocationData, 2, 1);

    m_lblRelocOffset = new QLabel(this);
    gridLayout->addWidget(m_lblRelocOffset, 3, 0);

    m_lblRelocOffsetData = new QLabel(this);
    gridLayout->addWidget(m_lblRelocOffsetData, 3, 1);

    vLeftLayout->addLayout(gridLayout);
    vLeftLayout->addSpacing(340);

    m_hLayout.addWidget(pDosLeftWidget);

    QVBoxLayout* vRightLayout = new QVBoxLayout();
    vRightLayout->setContentsMargins(0, 10, 10, 0);
    
    m_grpBoxEntryPoint = new QGroupBox(this);
    m_grpBoxEntryPoint->setFixedSize(240, 100);
    QGridLayout* gridGrpBoxLayout = new QGridLayout(m_grpBoxEntryPoint);
    gridGrpBoxLayout->setContentsMargins(10, 0, 10, 0);
    gridGrpBoxLayout->setSpacing(0);
    m_lblCS = new QLabel(this);
    gridGrpBoxLayout->addWidget(m_lblCS,0,0);
    m_lblCSValue = new QLabel(this);
    gridGrpBoxLayout->addWidget(m_lblCSValue, 0, 1);

    m_lblIP = new QLabel(this);
    gridGrpBoxLayout->addWidget(m_lblIP, 1,0);
    m_lblIPValue = new QLabel(this);
    gridGrpBoxLayout->addWidget(m_lblIPValue, 1, 1);

    vRightLayout->addWidget(m_grpBoxEntryPoint, Qt::AlignRight);
    vRightLayout->addSpacing(10);
    m_lblTips = new QLabel(this);
    m_lblTips->setFixedSize(300, 30);
    vRightLayout->addWidget(m_lblTips, Qt::AlignRight);
    vRightLayout->addStretch();
    
    m_hLayout.addLayout(vRightLayout);
    m_hLayout.addStretch();
    
    retranslateUi();
    
}

 
void DosWidget::retranslateUi()
{
    QString strSizeOfHeader = tr("Size Of Header");
    m_lblSizeOfHeader->setText(strSizeOfHeader);
    QString strEntryPoint = tr("Entry Point");
    m_lblEntryPoint->setText(strEntryPoint);
    m_lblRelocation->setText(tr("Relocation"));
    m_lblRelocOffset->setText(tr("Relocation File address"));
    m_grpBoxEntryPoint->setTitle(strEntryPoint);
    m_lblCS->setText("CS");
    m_lblIP->setText("IP");
    m_lblTips->setText(QString("%1 = (CS*10h + IP + %2)").arg(strEntryPoint).arg(strSizeOfHeader));
    
}

void DosWidget::updateData(const IMAGE_DOS_HEADER& dosHeader)
{
    quint16 wSizeOfHeader = sizeof(dosHeader);
    m_lblSizeOfHeaderData->setText(ushortToHexStr(wSizeOfHeader));
    quint32 dwEntryPoint = dosHeader.e_cs * 0x10 + dosHeader.e_ip + wSizeOfHeader;
    quint32 dwRelocations = dosHeader.e_crlc;
    m_lblEntryPointData->setText(uintToHexStr(dwEntryPoint));
    m_lblRelocationData->setText(uintToHexStr(dwRelocations));
    m_lblRelocOffsetData->setText(ushortToHexStr(dosHeader.e_lfarlc));
    m_lblCSValue->setText(ushortToHexStr(dosHeader.e_cs));
    m_lblIPValue->setText(ushortToHexStr(dosHeader.e_ip));
    
}
 

