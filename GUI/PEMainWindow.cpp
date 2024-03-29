#include "PEMainWindow.h"
#include <QMenu>
#include <QDebug>
#include <QPushButton>
#include <QGridLayout>
#include <QThread>
#include <QFileDialog>
#include <QMenuBar>
#include <QMimeData>
#include <QStatusBar>
#include <QDropEvent>
#include <QTranslator>
#include <QFile>
#include <QFileInfo>
#include "dataDirectoryWidget.h"
#include "PEHeaderItemListWidget.h"
#include "dosWidget.h"
#include "sectionTable.h"
#include "exportFunctionWidget.h"

#include "Util.h"




PEMainWindow::PEMainWindow(QWidget* parent) :BaseWindow(parent), m_gd(this)
{
    qDebug() << "PEMainWindow thread: " << QThread::currentThread();
    m_translator = new QTranslator();
    initUI();
    
    m_gd.init();
    PELoadWorker* peLoadWorker = m_gd.GetPELoadWorker();
    if (peLoadWorker) {
        connect(peLoadWorker, &PELoadWorker::startShowPEInfoOnGUI, this, &PEMainWindow::onShowPEInfo);
        connect(peLoadWorker, &PELoadWorker::startShowExportTableOnGUI, this, &PEMainWindow::onShowExportTable);
        connect(this, &PEMainWindow::startLoadPEBasic, peLoadWorker, &PELoadWorker::loadPEBasic);
    }
    qDebug() << "PEMainWindow: " << ", ThreadID: " << QThread::currentThreadId();
}

 
void PEMainWindow::initUI()
{
    setAcceptDrops(true);
    setFixedSize(800, 600);
    setAcceptDrops(true);

    setObjectName("PEMainWindow");

    _mainLayout = new QVBoxLayout(this);
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    _mainLayout->setSpacing(0);
    initMenuBar();

    m_lblPEPath = new QLabel(this);
    m_lblPEPath->setObjectName("lblPEPath");
    m_lblPEPath->setFixedHeight(30);
    m_lblPEPath->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_lblPEPath->setFrameShape(QFrame::Box);
    _mainLayout->addWidget(m_lblPEPath, 1);

    

    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setObjectName("tabWidget");
    //tabWidget->setGeometry(QRect(0, 50, 561, 441));
    _mainLayout->addWidget(m_tabWidget, 10);
    QWidget* tabPEHeader = new QWidget(m_tabWidget);
    tabPEHeader->setObjectName("tabPEHeader");
    QHBoxLayout* hLayoutHeader = new QHBoxLayout(tabPEHeader);
    
    m_itemListWidget = new PEHeaderItemListWidget(tabPEHeader);
    m_itemListWidget->setObjectName("peHeaderItemList");

    hLayoutHeader->addWidget(m_itemListWidget, 3);
    QWidget* rightWidget = new QWidget(tabPEHeader);
    QVBoxLayout* vLayoutRightWidget = new QVBoxLayout(rightWidget);
    vLayoutRightWidget->setContentsMargins(0, 0, 0, 2);
     
    m_lblDataDirectory = new QLabel(this);
    m_lblDataDirectory->setObjectName("dataDirectory");
    m_lblDataDirectory->setFixedHeight(24);
    vLayoutRightWidget->addWidget(m_lblDataDirectory);
   
    m_dataDirectoryTbl = new DataDirectoryWidget(rightWidget);
    m_dataDirectoryTbl->setObjectName("dataDirectoryTbl");
    vLayoutRightWidget->addWidget(m_dataDirectoryTbl);

    
   
    hLayoutHeader->addWidget(rightWidget, 7);

    m_tabWidget->addTab(tabPEHeader, QString("Header"));


    m_dosWidget = new DosWidget(m_tabWidget);
    m_dosWidget->setObjectName("Dos");
    m_tabWidget->addTab(m_dosWidget, QString("Dos"));

    m_secTbl = new sectionTable(m_tabWidget);
    m_nTabSectionHeader = m_tabWidget->addTab(m_secTbl, "");

    m_exportFunctionWidget = new ExportFunctionWidget(m_tabWidget);
    m_nTabExportFunction = m_tabWidget->addTab(m_exportFunctionWidget, "");



    int nStatusBarHeight = 30;
    m_statusbar = new QWidget(this);
    m_statusbar->setObjectName("statusbar");
    m_statusbar->setFixedHeight(nStatusBarHeight);
    QHBoxLayout* pStatusBarLayout = new QHBoxLayout(m_statusbar);
    pStatusBarLayout->setContentsMargins(10, 0, 10, 0);
    _mainLayout->addWidget(m_statusbar);
    
    m_btnRVATrans = new QPushButton(this);
    m_btnRVATrans->setFixedWidth(180);
    m_btnRVATrans->setEnabled(false);
    connect(m_btnRVATrans, &QPushButton::clicked, this, &PEMainWindow::onShowRVAToRAWWindow);
    pStatusBarLayout->addWidget(m_btnRVATrans);
    pStatusBarLayout->addStretch();
    m_btnClose = new QPushButton("", this);
    m_btnClose->setObjectName("closeBtn");
    m_btnClose->setFixedWidth(60);
    connect(m_btnClose, &QPushButton::clicked, this, &PEMainWindow::onClose);
    pStatusBarLayout->addWidget(m_btnClose);
    
    retranslateUi();
    this->setStyleSheet(getCssContent(":/css/mainwindow"));
}



void PEMainWindow::retranslateUi()
{
    m_pPEInfoMenu->setTitle(tr("File"));
    m_pActionOpenPE->setText(tr("Open PE File"));

    m_pHelpMenu->setTitle(tr("Help"));
    m_pActionSWitchLanguage->setText(tr("Switch Language"));
    m_pActionAbout->setText(tr("About"));
    QString strTrans = tr("RVA") + "<=>" + tr("RAW");
    m_btnRVATrans->setText(strTrans);
    m_btnClose->setText(tr("Close"));
    m_lblDataDirectory->setText(tr("DATA DIRECTORY"));
    
    updateDataDirectory();

    m_tabWidget->setTabText(m_nTabSectionHeader, tr("Section"));
    m_tabWidget->setTabText(m_nTabExportFunction, tr("Export Functions"));


    CPEInfo& peInfo = m_gd.GetPEInfo();
    if (!peInfo.loaded())
    {
        return;
    }
    m_secTbl->reloadSections(peInfo.getImageSectionHeaders());
    
   

}

void PEMainWindow::updateDataDirectory()
{
    CPEInfo& peInfo = m_gd.GetPEInfo();
    if (!peInfo.loaded())
    {
        return;
    }
    m_dataDirectoryTbl->reloadAllData(peInfo);
}


void PEMainWindow::onShowPEInfo() {
    CPEInfo& peInfo = m_gd.GetPEInfo();
    bool bLoaded = peInfo.loaded();
    m_btnRVATrans->setEnabled(bLoaded);
    QString strTitle = "PETools";
    if (bLoaded)
    {
        QString strArchInfo = peInfo.getImageBaseX86() ? "32bit" : "64bit";
        QString strFilePath = m_lblPEPath->text();

        QFile file(strFilePath);
        QFileInfo fileInfo(file);
        QString fileName = fileInfo.fileName();
        strTitle += QString("PETools - [%1] - [%2]").arg(fileName).arg(strArchInfo);
    }
    setWindowTitle(strTitle);
   
    //show PE header
    m_itemListWidget->updateItemsData(peInfo);
    updateDataDirectory();

    //show  DOS Header
    m_dosWidget->updateData(peInfo.getImageDosHeader());
    m_secTbl->reloadSections(peInfo.getImageSectionHeaders());
}


void PEMainWindow::onShowExportTable()
{
    CPEInfo& peInfo = m_gd.GetPEInfo();
    if (peInfo.loaded())
    {
        m_exportFunctionWidget->reloadExportDirectoryInfo(peInfo.getImageExportInfo());
    }
    
}

void PEMainWindow::onShowRVAToRAWWindow() {
    RvaToFoaWindow *rvaToFoaWindow = new RvaToFoaWindow(m_gd.GetPEInfo());
    //rvaToFoaWindow->setWindowFlag(Qt::FramelessWindowHint);
    
    rvaToFoaWindow->onShow();
}

void PEMainWindow::onClose()
{
    close();
}

void PEMainWindow::initMenuBar()
{
    m_menubar = new QMenuBar(this);
    m_menubar->setObjectName("menubar");
    m_menubar->setFixedSize(300,26);
    _mainLayout->addWidget(m_menubar);

    m_pPEInfoMenu = m_menubar->addMenu("");
    m_pActionOpenPE = m_pPEInfoMenu->addAction("", this, &PEMainWindow::onOpenPE);
    //setText
    m_pHelpMenu = m_menubar->addMenu("");
    m_pActionSWitchLanguage = m_pHelpMenu->addAction("", this, &PEMainWindow::SwitchLanguage);

    m_pActionAbout = m_pHelpMenu->addAction("", this, &PEMainWindow::ShowAboutDialog);
    m_pActionAbout->setVisible(false);
}





void PEMainWindow::onOpenPE()
{
    qDebug() << "onOpenPE called";
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/", tr("Exe Files (*.exe);;DLL libraries (*.dll)"));
    if (fileName.isEmpty()) {
        qDebug() << "Selected empty file path";
        return;
    }

    onStartLoadPE(fileName);
}





void PEMainWindow::SwitchLanguage()
{
    
    if (!m_bTranslator)
    {
        QString strPMPath = QCoreApplication::applicationDirPath() + "/langs/zh.qm";
        m_translator->load(strPMPath);
        qApp->installTranslator(m_translator);
        
    }
    else
    {
        qApp->removeTranslator(m_translator);        
    }
    m_bTranslator = !m_bTranslator;
    retranslateUi();
}


void PEMainWindow::ShowAboutDialog()
{

}



void PEMainWindow::onStartLoadPE(const QString& fileName) {
    qDebug() << "Selected file path : " << fileName << ", ThreadID: " << QThread::currentThreadId();
    QString strLblText = fileName;
#ifdef Q_OS_WIN
    // fileName.replace("/", QDir::separator());
    strLblText.replace("/", "\\");
#endif
    m_lblPEPath->setText(strLblText);
    emit startLoadPEBasic(fileName);
    //start pe loader thread
}



void PEMainWindow::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void PEMainWindow::dropEvent(QDropEvent* event)
{
    foreach(const QUrl & url, event->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        qDebug() << "Dropped file:" << fileName;
        onStartLoadPE(fileName);
        return;
    }
}



