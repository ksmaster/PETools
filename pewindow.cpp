#include "pewindow.h"
#include "./ui_pewindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QDragEnterEvent>
#include <QMimeData>
#include <memory>
#include "Util.h"


PEWindow::PEWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PEWindow)
    ,m_gd(this)

{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/icon/pe.ico"));
    setAcceptDrops(true);
    QMenu * pPEInfoMenu = ui->menubar->addMenu(tr("File"));
    pPEInfoMenu->addAction(tr("Open PE File"), this, &PEWindow::ShowPEWindow);
    m_gd.init();
    PELoadWorker * peLoadWorker = m_gd.GetPELoadWorker();
    if(peLoadWorker) {
       connect(peLoadWorker, &PELoadWorker::startShowPEInfoOnGUI, this, &PEWindow::ShowPEBasicResult);
       connect(this, &PEWindow::startLoadPEBasic, peLoadWorker, &PELoadWorker::loadPEBasic);
    }
     qDebug() << "PEWindow: " <<  ", ThreadID: " << QThread::currentThreadId();

}

void PEWindow::onStartLoadPE(const QString &fileName) {
    qDebug() << "Selected file path : " << fileName << ", ThreadID: " << QThread::currentThreadId();
    ui->lblPEPath->setText(fileName);
    emit startLoadPEBasic(fileName);
    //start pe loader thread
}

void PEWindow::ShowPEWindow()
{
   qDebug()<< "ShowPEWindow called";
   QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/", tr("Exe Files (*.exe);;DLL libraries (*.dll)"));
   if (fileName.isEmpty()) {
       qDebug() << "Selected empty file path : ";
       return;
   }
#ifdef Q_OS_WIN
   // fileName.replace("/", QDir::separator());
    fileName.replace("/", "\\");
#endif
   onStartLoadPE(fileName);

}

void PEWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void PEWindow::dropEvent(QDropEvent *event)
{
    foreach (const QUrl &url, event->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        qDebug() << "Dropped file:" << fileName;
        onStartLoadPE(fileName);
    }
}

void PEWindow::ShowPEBasicResult() {
    CPEInfo &peInfo = m_gd.GetPEInfo();

    DWORD dwEntryPointRVA = peInfo.getEntryPointRVA();
    ui->lblEntryPointRVA->setText(uintToHexStr(dwEntryPointRVA));
    ui->lblEntryPointRAW->setText(uintToHexStr(peInfo.rvaToFoa(dwEntryPointRVA)));
    ui->lblImageBase->setText(uintToHexStr(peInfo.getImageBaseX86()));
    ui->lblSizeOfImage->setText(uintToHexStr(peInfo.getSizeOfImage()));
    ui->lblSectionAlignment->setText(uintToHexStr(peInfo.getSectionAlignment()));
    ui->lblFileAlignment->setText(uintToHexStr(peInfo.getFileAlignment()));
    ui->lblNumberOfSections->setText(ushortToHexStr(peInfo.getNumberOfSections()));
    ui->lblCharacteristics->setText(ushortToHexStr(peInfo.getCharacteristics()));
    ui->lblSizeOfHeaders->setText(uintToHexStr(peInfo.getCoffHeaderSize()));
    ui->lblCheckSum->setText(uintToHexStr(peInfo.getCheckSum()));
    ui->lblMachine->setText(ushortToHexStr(peInfo.getMachine()));
    ui->lblSubSystem->setText(ushortToHexStr(peInfo.getSubSystem()));
    ui->lblDllCharacteristics->setText(ushortToHexStr(peInfo.getDllCharacteristics()));
    ui->lblDllCharacteristics->setText(ushortToHexStr(peInfo.getDllCharacteristics()));
}

PEWindow::~PEWindow()
{
    delete ui;
}

