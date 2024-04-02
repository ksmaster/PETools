#ifndef _EXPORT_FUNCTION_WIDGET_H_
#define _EXPORT_FUNCTION_WIDGET_H_
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "common/baseWindow.h"
#include "common/itemPairListWidget.h"
#include "PEInfo.h"

class QModelIndex; 
class QStandardItemModel;
class QItemSelection;
class QTreeView;
class DllTree;



class ExportDirectoryItems : public ItemPairListWidget
{
    Q_OBJECT
public: 
    explicit ExportDirectoryItems(QWidget* parent = nullptr);
    void updateData(const IMAGE_EXPORT_DIRECTORY&);
   
private:
    QList<ItemPair> getItemPairs();
};


class ExportFunctionWidget : public BaseWindow
{
    Q_OBJECT
public:
    explicit ExportFunctionWidget(QWidget *parent = nullptr);
    void retranslateUi();
    void reloadExportDirectoryInfo(const IMAGE_EXPORT_INFO_WRAPPER& );
    void clear();
private:
    QVBoxLayout m_vLayout;
    ExportDirectoryItems *m_exportDirItems;
    DllTree* m_exportDllTree;
    QLabel* m_lblNoExportHint;
   
};

#endif

