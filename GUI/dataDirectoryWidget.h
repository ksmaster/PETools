#ifndef _DATA_DIRECTORY_WIDGET_H
#define _DATA_DIRECTORY_WIDGET_H
#include <QVBoxLayout>
#include "PEInfo.h"
#include "common/baseTableView.h"


class QStandardItemModel;


class DataDirectoryWidget : public baseTableView
{
    Q_OBJECT

public:
    explicit DataDirectoryWidget(const CPEInfo& _peInfo, QWidget *parent = nullptr);
private:
    QList<QStandardItem*> createRow(int index);
    QList<ColInfo>  getColsInfo();
private:
    const CPEInfo& peInfo;
};

#endif
