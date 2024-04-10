#ifndef _IMPORT_TABLE__H
#define _IMPORT_TABLE__H
#include <vector>
#include <vector>
#include <QHBoxLayout>
#include "PEInfo.h"
#include "common/baseTableView.h"
#include "common/itemPairListWidget.h"

class QStandardItemModel;




class importTable : public baseTableView
{
    Q_OBJECT
public:
    explicit importTable(const CPEInfo& _peInfo, QWidget *parent = nullptr);
private:
	QList<QStandardItem*> createRow(int index);
	QList<ColInfo>  getColsInfo();
private slots:
    
private:
	const CPEInfo& peInfo;
};


class ImportItemListWidget : public ItemPairListWidget
{
	Q_OBJECT

public:
	explicit ImportItemListWidget(QWidget* parent = nullptr);
	void updateItemsData(const IMAGE_IMPORT_DESCRIPTOR_Wrapper&);
private:
	QList<ItemPair> getItemPairs();
};



class importHintNameTable: public baseTableView
{
	Q_OBJECT

public:
	explicit importHintNameTable(const CPEInfo& _peInfo, QWidget* parent = nullptr);
	void reloadDllHintName(const QString&);
private:
	QList<QStandardItem*> createRow(int index);
	QList<ColInfo>  getColsInfo();
private:
	const CPEInfo& peInfo;
	QString m_strDllName;
};

class importWidget : public QWidget
{
	Q_OBJECT
public:
	explicit importWidget(const CPEInfo& _peInfo, QWidget* parent = nullptr);
	void reload(int nSize);
	void onSelItemChanged(const QItemSelection&, const QItemSelection&);
private:
	const CPEInfo& peInfo;
	QHBoxLayout m_hImportLayout;
	importTable* m_importTbl;
	ImportItemListWidget* m_importItemList;
	importHintNameTable* m_importHintNameTable;
};

#endif

