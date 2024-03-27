#ifndef _DOS_WIDGET__H
#define _DOS_WIDGET__H
#include <QDialog>
#include <QWidget>
#include <QRadioButton>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include "PEInfo.h"
#include "common/baseWindow.h"



class DosWidget : public BaseWindow
{
    Q_OBJECT
public:
    explicit DosWidget(QWidget *parent = nullptr);
    void retranslateUi();
    void updateData(const IMAGE_DOS_HEADER&);
private slots:
   
private:
    QLabel* m_lblSizeOfHeader;
    QLabel* m_lblSizeOfHeaderData;
    QLabel* m_lblEntryPoint;
    QLabel* m_lblEntryPointData;
    QLabel* m_lblRelocation;
    QLabel* m_lblRelocationData;
    QLabel* m_lblRelocOffset;
    QLabel* m_lblRelocOffsetData;
    QGroupBox* m_grpBoxEntryPoint;
    QLabel* m_lblCS;
    QLabel* m_lblCSValue;
    QLabel* m_lblIP;
    QLabel* m_lblIPValue;
    QLabel* m_lblTips;
    QHBoxLayout m_hLayout;
   
};

#endif

