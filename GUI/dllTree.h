#ifndef _DLL_TREE_H_
#define _DLL_TREE_H_
#include <QTreeView>
#include <string>
#include <QVBoxLayout>
#include "PEInfo.h"


class QModelIndex; 
class QStandardItemModel;
class QStandardItem;
class QItemSelection;



class DllTree : public QWidget
{
    Q_OBJECT
public:
    explicit DllTree(QWidget *parent = nullptr);
    void updateData(const IMAGE_EXPORT_INFO_WRAPPER&);
    void clear();
private:
    void retranslateUi();
    void appendItemsFromRange(const IMAGE_EXPORT_INFO_WRAPPER& imgExportInfo, int nBeginIdx, int nEndIdx);
private slots:
    void selectTreeItem(const QItemSelection& selected, const QItemSelection& deselected);
    void onTreeViewPressed(QModelIndex modeIndex);
    void onScrollBarChange(int value);

private:
    QVBoxLayout m_vLayout;
    QTreeView* m_pTreeView;
    QStandardItemModel* m_treeViewModel;
    QStandardItem* m_topLevelItem;
    int m_nTotalItemCnt = 0;
    const IMAGE_EXPORT_INFO_WRAPPER* m_pImgExportInfo =nullptr;
};

#endif

