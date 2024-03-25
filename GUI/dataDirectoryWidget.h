#ifndef _DATA_DIRECTORY_WIDGET_H
#define _DATA_DIRECTORY_WIDGET_H
#include <QWidget>
#include <QRadioButton>
#include <QButtonGroup>
#include <QMap>
#include <QLabel>
#include <QVBoxLayout>
#include <QTableView>
#include "PEInfo.h"


class QStandardItemModel;


class DataDirectoryWidget : public QTableView
{
    Q_OBJECT

public:
    explicit DataDirectoryWidget(QWidget *parent = nullptr);
    ~DataDirectoryWidget();
    void reloadAllData(const CPEInfo& peInfo);
private:
    void setHeader();
    void addDataDirectory(const QString& strName, const CPEInfo& peInfo, const IMAGE_DATA_DIRECTORY&);
private:
    QStandardItemModel* m_model;
private:
   
};

#endif
