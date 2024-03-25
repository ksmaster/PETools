#ifndef _SECTION_TABLE_WIDGET__H
#define _SECTION_TABLE_WIDGET__H
#include <vector>
#include <QTableView>
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#elif __linux__
#include "PELinux.h"
#endif



class QStandardItemModel;


#include <QStyledItemDelegate>

class IconButtonDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	IconButtonDelegate(QWidget* parent = 0) : QStyledItemDelegate(parent) {}

	void paint(QPainter* painter, const QStyleOptionViewItem& option,
		const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option,
		const QModelIndex& index) const;
	void setModelData(QWidget* editor, QAbstractItemModel* model,
		const QModelIndex& index) const;

private slots:
};


struct ColInfo
{
	QString strHead;
	int  nColWidth;
};


class sectionTable : public QTableView
{
    Q_OBJECT
public:
    explicit sectionTable(QWidget *parent = nullptr);
    void reloadSections(const std::vector<IMAGE_SECTION_HEADER>&);
private:
	QList<ColInfo>  getColsInfo();
private slots:
    void setHeader();
    void addItem(unsigned int index, const IMAGE_SECTION_HEADER& sectionHeader);
private:
    QStandardItemModel* m_model;
	QStyledItemDelegate* m_myStandardDelegate = nullptr;
};

#endif

