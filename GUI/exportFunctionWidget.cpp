#include "exportFunctionWidget.h"
#include <QDebug>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QDateTime>
#include <QTreeView>
#include "Util.h"
#include "dllTree.h"
#include "constants/constStrings.h"


ExportDirectoryItems::ExportDirectoryItems(QWidget* parent) : ItemPairListWidget(parent, 3)
{
    setObjectName("ExportDirectoryItems");
    initUI();
}


QList<ItemPair> ExportDirectoryItems::getItemPairs()
{
    QList<ItemPair> exportDirItemsPairs = {
        { STR_CHARACTERISTICS,      tr("Characteristics"), },
        { "TimeDateStamp",            tr("TimeDateStamp"), },
        { "Name",                    tr("Name"), },
        { "MajorVersion",              tr("MajorVersion"), },
        { "MinorVersion",          tr("MinorVersion"), },
        { "Base",                   tr("Base"), },
        { "NumberOfFunctions",     tr("NumberOfFunctions"), },
        { "NumberOfNames",          tr("NumberOfNames"), },
        { "AddressOfFunctions",             tr("AddressOfFunctions"), },
        { "AddressOfNames",             tr("AddressOfNames"), },
        { "AddressOfNameOrdinals",             tr("AddressOfNameOrdinals"), },
    };
    return exportDirItemsPairs;
}
void ExportDirectoryItems::updateData(const IMAGE_EXPORT_DIRECTORY& imageExportDir)
{
    qint64 timestamp = imageExportDir.TimeDateStamp;

    // Convert timestamp to QDateTime
    QDateTime dateTime = QDateTime::fromSecsSinceEpoch(timestamp);

    // Convert QDateTime to string
    QString dateString = dateTime.toString("yyyy-MM-dd hh:mm:ss");
    setItemData("TimeDateStamp", dateString);
    setItemData("MajorVersion", QString::number(imageExportDir.MajorVersion));
    setItemData("MinorVersion", QString::number(imageExportDir.MinorVersion));
    setItemData("NumberOfFunctions", QString::number(imageExportDir.NumberOfFunctions));
    setItemData("NumberOfNames", QString::number(imageExportDir.NumberOfNames));

    setItemData("Name", uintToHexStr(imageExportDir.Name));
    setItemData("Base", uintToHexStr(imageExportDir.Base));
    setItemData(STR_CHARACTERISTICS, uintToHexStr(imageExportDir.Characteristics));
    setItemData("AddressOfFunctions", uintToHexStr(imageExportDir.AddressOfFunctions));
    setItemData("AddressOfNames", uintToHexStr(imageExportDir.AddressOfNames));
    setItemData("AddressOfNameOrdinals", uintToHexStr(imageExportDir.AddressOfNameOrdinals));
}


ExportFunctionWidget::ExportFunctionWidget(QWidget *parent) : BaseWindow(parent), m_vLayout(this)
{
    setObjectName("ExportFunctionWidget");
    m_exportDirItems = new ExportDirectoryItems(this);
    m_vLayout.addWidget(m_exportDirItems);

    m_exportDllTree = new DllTree(this);
    m_exportDllTree->setObjectName("exportDllTree");
    m_vLayout.addWidget(m_exportDllTree);

    m_lblNoExportHint = new QLabel(this);
    m_lblNoExportHint->setObjectName("noExportHint");
    m_lblNoExportHint->setFixedHeight(400);
    m_vLayout.addWidget(m_lblNoExportHint);
    m_lblNoExportHint->hide();
    setStyleSheet(getCssContent(":/css/exportFunctionWidget"));
    retranslateUi();
    clear();
}


void ExportFunctionWidget::retranslateUi()
{
    m_lblNoExportHint->setText(tr("No Export Functions"));
}

void ExportFunctionWidget::clear()
{
    m_exportDirItems->clearData();
    m_exportDllTree->hide();
    m_lblNoExportHint->show();
}

void ExportFunctionWidget::reloadExportDirectoryInfo(const IMAGE_EXPORT_INFO_WRAPPER& imgExportInfo)
{
    clear();
    if (!imgExportInfo.m_bLoaded)
    {
        return;
    }
    m_lblNoExportHint->hide();
    m_exportDirItems->updateData(imgExportInfo.m_imgExportDirectory);
    m_exportDllTree->updateData(imgExportInfo);
    m_exportDllTree->show();
}


 

