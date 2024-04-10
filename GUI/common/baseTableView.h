#ifndef _BASE_TABLE_VIEW_H_
#define _BASE_TABLE_VIEW_H_

#include <QTableView>


struct ColInfo
{
	QString strHead;
	int  nColWidth;
};
 
class QStandardItem;
class QStandardItemModel;


class baseTableView : public QTableView
{
    Q_OBJECT
public:
    explicit baseTableView(QWidget *parent = nullptr);
    void reload(int nRowCnt);
    void setSel(int nRow);
protected:
	virtual QList<ColInfo>  getColsInfo();
    virtual QList<QStandardItem*> createRow(int index);
    virtual void preSetItemDelegate();
    static QList<QStandardItem*> getStandardItemList(const QStringList &itemStrList);
    void showToolTip(const QModelIndex&);
private slots:
   
private:
    QStandardItemModel* m_model;
};

#endif

