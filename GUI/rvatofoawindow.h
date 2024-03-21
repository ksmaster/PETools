#ifndef RVATOFOAWINDOW_H
#define RVATOFOAWINDOW_H
#include <QDialog>
#include <QWidget>
#include <QRadioButton>
#include <QButtonGroup>
#include <QGridLayout>
#include <QLabel>
#include "PEInfo.h"
#include "common/hexlineedit.h"
#include "common/baseWindow.h"


class CPEInfo;


class RvaToFoaWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RvaToFoaWindow(CPEInfo &peInfo, QWidget *parent = nullptr);
    ~RvaToFoaWindow();
    void init();
    void reload();
    void onShow();
    void retranslateUi();
private slots:
    void onRadioBtnClicked();
    void on_lineEditFOA_textChanged(const QString &arg1);
    void on_lineEditRVA_textChanged(const QString& arg1);
    void on_lineEditVA_textChanged(const QString& arg1);
    void on_lineEditImageBase_textChanged(const QString &arg1);
private:
    HexLineEdit* lineEditFOA;
    HexLineEdit* lineEditRVA;
    HexLineEdit* lineEditVA;
    HexLineEdit* lineEditImageBase;
    QRadioButton* m_radioBtnFOA;
    QRadioButton* m_radioBtnRVA;
    QRadioButton* m_radioBtnVaName;
    QGridLayout* gridLayout;
    QLabel* m_lblImageBaseName;
   
    QButtonGroup* buttonGroup;
    CPEInfo &m_peInfoRef;
    qulonglong m_uImageBase;
   
};

#endif

