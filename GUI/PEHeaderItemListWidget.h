#ifndef _PE_HEADER_ITEM_LIST_WIDGET_H
#define _PE_HEADER_ITEM_LIST_WIDGET_H
#include <QCheckBox>
#include <QGridLayout>
#include "common/itemPairListWidget.h"


class CPEInfo;



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


class PEHeaderItemListWidget : public ItemPairListWidget
{
    Q_OBJECT

public:
    explicit PEHeaderItemListWidget(QWidget *parent = nullptr);
    void updateItemsData(const CPEInfo&);
private:
    QList<ItemPair> getItemPairs();
private:
    DllCharacteristicsTipWidget* m_pDllCharacteristicsTipWidget = nullptr;
};

#endif
