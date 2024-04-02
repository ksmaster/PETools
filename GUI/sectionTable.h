#ifndef _SECTION_TABLE_WIDGET__H
#define _SECTION_TABLE_WIDGET__H
#include <vector>
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#elif __linux__
#include "PELinux.h"
#endif
#include "common/baseTableView.h"


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




class sectionTable : public baseTableView
{
    Q_OBJECT
public:
    explicit sectionTable(QWidget *parent = nullptr);
    void reloadSections(const std::vector<IMAGE_SECTION_HEADER>&);
private:
	QList<QStandardItem*> createRow(int index);
	QList<ColInfo>  getColsInfo();
	void preSetItemDelegate();
private slots:
    
private:
	QStyledItemDelegate* m_myStandardDelegate = nullptr;
	std::vector<IMAGE_SECTION_HEADER> m_vecImgSectionHeader;
};

#endif

