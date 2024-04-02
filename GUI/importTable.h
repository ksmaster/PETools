#ifndef _IMPORT_TABLE__H
#define _IMPORT_TABLE__H
#include <vector>
#include "PEInfo.h"
#include "common/baseTableView.h"


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

#endif

