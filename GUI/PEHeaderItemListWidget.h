#ifndef _PE_HEADER_ITEM_LIST_WIDGET_H
#define _PE_HEADER_ITEM_LIST_WIDGET_H
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


class CPEInfo;

struct ItemPair
{
    QString key;
    QString value;
};

class DllCharacteristicsTipWidget : public BaseWindow
{
    Q_OBJECT
public:
    explicit DllCharacteristicsTipWidget(QWidget* parent = nullptr);
    void updateData(int nDllCharacteristics);
protected:
    void retranslateUi();
private:
    QGridLayout m_gridLayout;
    QCheckBox* m_chkDynamicBase;
    QCheckBox* m_chkForceIntegrity;
    QCheckBox* m_chkDEP;
    QCheckBox* m_chkNoIsloation;
    QCheckBox* m_chkNoSEH;
    QCheckBox* m_chkNoBind;
    QCheckBox* m_chkWDM;
    QCheckBox* m_chkCFG;
    QCheckBox* m_chkTerminalServerAware;
};


class PEHeaderItemWindow : public HoverWindow
{
    Q_OBJECT
public:
    explicit PEHeaderItemWindow(const QString& strItemName, QWidget* parent = nullptr);
    ~PEHeaderItemWindow();
   
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

class PEHeaderItemListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PEHeaderItemListWidget(QWidget *parent = nullptr);
    ~PEHeaderItemListWidget();
   
    void updateItemsData(const CPEInfo&);
    void updateItemData(const QString& itemName, const QString& data);
    void updateItemLblText(const QString& itemName, const QString& data);
    void updatePEHeaderItemLbls();
private:
    static QList<ItemPair> getPEHeaderItemPairs();
private:
    QMap<QString, PEHeaderItemWindow*> m_mapPEHeader;
    QVBoxLayout m_vboxLayout;
    DllCharacteristicsTipWidget* m_pDllCharacteristicsTipWidget = nullptr;
};

#endif
