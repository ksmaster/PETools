#ifndef __ITEM_LIST_WIDGET_H
#define __ITEM_LIST_WIDGET_H
#include <QWidget>
#include <QRadioButton>
#include <QButtonGroup>
#include <QDialog>
#include <QMap>
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include "common/hoverWindow.h"
#include "common/baseWindow.h"




class ItemPairWindow : public HoverWindow
{
    Q_OBJECT
public:
    explicit ItemPairWindow(const QString& strItemName, QWidget* parent = nullptr);
    ~ItemPairWindow();
   
    void setToolTip(QWidget* toolTipWindow) {
        m_tooltipWindow = toolTipWindow;
    }
    void setNameLblText(const QString&);
    void setValueLblText(const QString&);
protected:
    void onHoverEvent(QMouseEvent*);
    void onHoverLeave();
private:
    QWidget* m_tooltipWindow=nullptr;
    QLabel* item;
    QLabel *data;
    QHBoxLayout m_layout;
};


struct ItemPair
{
    QString key;
    QString value;
};

class ItemPairListWidget : public BaseWindow
{
    Q_OBJECT

public:
    explicit ItemPairListWidget(QWidget* parent = nullptr, int nCol = 1);
    void clearData();
    void refreshAllItemsLblText();
    void setItemLblText(const QString& itemName, const QString& data);
    void setItemData(const QString& itemName, const QString& data);
    void setItemTooltip(const QString& itemName, QWidget *tooltip);
protected:
    void retranslateUi();
    virtual QList<ItemPair> getItemPairs() { return {}; }
    void initUI();
private:
    
private:
    int m_nCol = 0;
    QMap<QString, ItemPairWindow*> m_mapItems;
    QGridLayout m_vboxLayout;

};


#endif
