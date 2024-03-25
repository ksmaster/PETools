#include "rvatofoawindow.h"
#include <QDebug>
#include "Util.h"



RvaToFoaWindow::RvaToFoaWindow(CPEInfo &peInfo,QWidget *parent) :
    QDialog(parent),
    m_peInfoRef(peInfo)
{
    setObjectName("rvaToFoa");
    setFixedSize(493, 300);
    setAttribute(Qt::WA_DeleteOnClose);
    setModal(true);
    setWindowFlags(windowFlags() & (~Qt::WindowContextHelpButtonHint));
    gridLayout = new QGridLayout(this);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(0);
    gridLayout->setMargin(0);
    buttonGroup = new QButtonGroup(this);
    buttonGroup->setObjectName("buttonGroup");
    
    int nItemWidth = 200;
    int nItemHeight = 40;

    m_radioBtnFOA = new QRadioButton(this);    
    m_radioBtnFOA->setObjectName("radioBtn");
    
    
    gridLayout->addWidget(m_radioBtnFOA, 0, 0, Qt::AlignLeft);
    buttonGroup->addButton(m_radioBtnFOA);

    lineEditFOA = new HexLineEdit(this);
    lineEditFOA->setObjectName("hexLineEdit");
    lineEditFOA->setFixedSize(nItemWidth, nItemHeight);
    gridLayout->addWidget(lineEditFOA, 0, 1, Qt::AlignLeft);

    m_radioBtnRVA = new QRadioButton(this);
    m_radioBtnRVA->setObjectName("radioBtn");
    
    gridLayout->addWidget(m_radioBtnRVA, 1, 0, Qt::AlignLeft);
    buttonGroup->addButton(m_radioBtnRVA);
   
    lineEditRVA = new HexLineEdit(this);
    lineEditRVA->setObjectName("hexLineEdit");
    lineEditRVA->setFixedSize(nItemWidth, nItemHeight);
    gridLayout->addWidget(lineEditRVA, 1, 1, Qt::AlignLeft);

   
    m_radioBtnVaName = new QRadioButton(this);
    m_radioBtnVaName->setObjectName("radioBtn");
    
    gridLayout->addWidget(m_radioBtnVaName, 2, 0, Qt::AlignLeft);
    buttonGroup->addButton(m_radioBtnVaName);

    connect(m_radioBtnFOA, &QRadioButton::clicked, this, &RvaToFoaWindow::onRadioBtnClicked);
    connect(m_radioBtnRVA, &QRadioButton::clicked, this, &RvaToFoaWindow::onRadioBtnClicked);
    connect(m_radioBtnVaName, &QRadioButton::clicked, this, &RvaToFoaWindow::onRadioBtnClicked);

    m_radioBtnFOA->setChecked(true);

    lineEditVA = new HexLineEdit(this);
    lineEditVA->setObjectName("hexLineEdit");
    lineEditVA->setFixedSize(nItemWidth, nItemHeight);
    gridLayout->addWidget(lineEditVA, 2, 1, Qt::AlignLeft);

    m_lblImageBaseName = new QLabel(this);
    m_lblImageBaseName->setObjectName("rvaLbl");
    
    gridLayout->addWidget(m_lblImageBaseName, 3, 0, Qt::AlignLeft);
     
    lineEditImageBase = new HexLineEdit(this);
    lineEditImageBase->setObjectName("hexLineEdit");
    lineEditImageBase->setFixedSize(nItemWidth, nItemHeight);
    

    connect(lineEditFOA, &HexLineEdit::textChanged, this, &RvaToFoaWindow::on_lineEditFOA_textChanged);
    connect(lineEditRVA, &HexLineEdit::textChanged, this, &RvaToFoaWindow::on_lineEditRVA_textChanged);
    connect(lineEditVA, &HexLineEdit::textChanged, this, &RvaToFoaWindow::on_lineEditVA_textChanged);
    connect(lineEditImageBase, &HexLineEdit::textChanged, this, &RvaToFoaWindow::on_lineEditImageBase_textChanged);

    onRadioBtnClicked();
    gridLayout->addWidget(lineEditImageBase, 3, 1, Qt::AlignLeft);
    retranslateUi();
    this->setStyleSheet(getCssContent(":/css/rvaToFoaWindow"));
}

 
void RvaToFoaWindow::retranslateUi()
{
    m_radioBtnFOA->setText(tr("File Offset"));
    m_radioBtnRVA->setText(tr("Relative Virtual Address"));
    m_radioBtnVaName->setText(tr("Virtual Address"));
    m_lblImageBaseName->setText(tr("ImageBase"));
    QString strTrans = tr("RVA") + "<=>" + tr("RAW");
    setWindowTitle(strTrans);
}
 
RvaToFoaWindow::~RvaToFoaWindow()
{
    qDebug() << "~RvaToFoaWindow";
}


void RvaToFoaWindow::reload()
{
    init();

}

void RvaToFoaWindow::init() {
    m_uImageBase = m_peInfoRef.m_bX86 ? m_peInfoRef.getImageBaseX86(): m_peInfoRef.getImageBaseX64();
    lineEditFOA->setMaxLength(8);
    lineEditRVA->setMaxLength(8);
    lineEditVA->setMaxLength(m_peInfoRef.m_bX86 ? 8 : 16);
    lineEditImageBase->setMaxLength(m_peInfoRef.m_bX86 ? 8:16);
    lineEditImageBase->setText(m_peInfoRef.m_bX86 ? uintToHexStr(m_uImageBase) : uLongLongToHexStr(m_uImageBase));
}

void RvaToFoaWindow::onShow() {
    if(m_peInfoRef.loaded()) {
        init();
    }
    show();
}

void RvaToFoaWindow::onRadioBtnClicked()
{
    lineEditFOA->setEnabled(false);
    lineEditRVA->setEnabled(false);
    lineEditVA->setEnabled(false);
    lineEditImageBase->setEnabled(false);
    if (m_radioBtnFOA->isChecked())
    {
        lineEditFOA->setEnabled(true);
    }
    else if(m_radioBtnRVA->isChecked())
    {
        lineEditRVA->setEnabled(true);
    }
    else
    {
        lineEditVA->setEnabled(true);
        lineEditImageBase->setEnabled(true);
    }
}



void RvaToFoaWindow::on_lineEditFOA_textChanged(const QString &strHexFOA)
{
    if (!m_radioBtnFOA->isChecked())
    {
        return;
    }
    bool ok;
    qint32 dwFOA = strHexFOA.toULong(&ok, 16);
    qint32 dwRVA = m_peInfoRef.foaToRva(dwFOA);
    if(dwRVA != 0xFFFFFFFF) {
        lineEditRVA->setText(uintToHexStr(dwRVA));
        qulonglong va = m_uImageBase + dwRVA;
        lineEditVA->setText(m_peInfoRef.m_bX86 ? uintToHexStr(va) : uLongLongToHexStr(va));
    } else {
        this->lineEditRVA->setText(QString("F").repeated(8));
        this->lineEditVA->setText(QString("F").repeated(m_peInfoRef.m_bX86 ? 8:16));
    }
}

void RvaToFoaWindow::on_lineEditRVA_textChanged(const QString &arg1)
{   
    if (!m_radioBtnRVA->isChecked())
    {
        return;
    }
    bool ok;
    DWORD dwRVA = arg1.toUInt(&ok, 16);
    DWORD dwFOA = m_peInfoRef.rvaToFoa(dwRVA);
    if(dwFOA != 0xFFFFFFFF) {
        this->lineEditFOA->setText(uintToHexStr(dwFOA));
        qulonglong va = m_uImageBase + dwRVA;
        this->lineEditVA->setText(m_peInfoRef.m_bX86 ? uintToHexStr(va) : uLongLongToHexStr(va));
    } else {
        this->lineEditFOA->setText("FFFFFFFF");
        this->lineEditVA->setText("FFFFFFFF");
    }
}


void RvaToFoaWindow::on_lineEditVA_textChanged(const QString& strHexVA)
{
    if (!m_radioBtnVaName->isChecked())
    {
        return;
    }
    bool ok;
    qulonglong va = 0;
    if (m_peInfoRef.m_bX86)
    {
        va = strHexVA.toULong(&ok, 16);
    }
    else
    {
        va = strHexVA.toULongLong(&ok, 16);
    }
    if ( (va > m_uImageBase) && (va < (m_uImageBase + 0xFFFFFFFF) ))
    {
        DWORD dwRVA = va - m_uImageBase;
        DWORD dwFOA = m_peInfoRef.rvaToFoa(dwRVA);
        if (dwFOA != 0xFFFFFFFF) {
            this->lineEditFOA->setText(uintToHexStr(dwFOA));
            this->lineEditRVA->setText(uintToHexStr(dwRVA));
            return;
        }
    }
    this->lineEditFOA->setText(QString("F").repeated(8));
    this->lineEditRVA->setText(QString("F").repeated(8));
}


void RvaToFoaWindow::on_lineEditImageBase_textChanged(const QString& strHexImageBase)
{
    if (!m_radioBtnVaName->isChecked())
    {
        return;
    }
    bool ok;
    if (m_peInfoRef.m_bX86)
    {
        m_uImageBase = strHexImageBase.toULong(&ok, 16);
    }
    else
    {
        m_uImageBase = strHexImageBase.toULongLong(&ok, 16);
    }
    QString strRVA = lineEditRVA->text();
    if (strRVA.isEmpty() || (strRVA == QString("F").repeated(8)))
    {
        return;
    }
    DWORD dwRVA = strRVA.toUInt(&ok, 16);
    qulonglong va = m_uImageBase + dwRVA;
    lineEditVA->setText(m_peInfoRef.m_bX86 ? uintToHexStr(va) : uLongLongToHexStr(va));
}