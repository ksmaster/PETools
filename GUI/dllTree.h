#ifndef _DLL_TREE_H_
#define _DLL_TREE_H_
#include <QTreeView>
#include <QVBoxLayout>
#include "PEInfo.h"


class QModelIndex; 
class QStandardItemModel;
class QItemSelection;
class QTreeView;



class DllTree : public QWidget
{
    Q_OBJECT
public:
    explicit DllTree(QWidget *parent = nullptr);
    void updateData(const IMAGE_EXPORT_INFO_WRAPPER&);
private:
    void retranslateUi();
private slots:
    void selectTreeItem(const QItemSelection& selected, const QItemSelection& deselected);
    
private:
    QVBoxLayout m_vLayout;
    QTreeView* m_pTreeView;
    QStandardItemModel* m_treeViewModel;
   
};

#endif

