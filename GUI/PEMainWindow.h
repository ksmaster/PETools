#ifndef _PE_MAIN_WINDOW_H
#define _PE_MAIN_WINDOW_H

#include <QObject>
#include <QWidget>
#include <QVariant>
#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QList>
#include <QMap>
#include <QWidget>
#include <QDragEnterEvent>
#include <memory>
#include <QThread>
#include "rvatofoawindow.h"
#include "globaldata.h"
#include "common/qbasiclabel.h"
#include "common/baseWindow.h"



class QDropEvent;
class QTranslator;
class QTableView;
class PEHeaderItemListWidget;
class DataDirectoryWidget;
class DosWidget;
class sectionTable;



class PEMainWindow: public BaseWindow
{
	Q_OBJECT
public:
	PEMainWindow(QWidget* parent=nullptr);
protected:
    void dragEnterEvent(QDragEnterEvent* ev);
    void dropEvent(QDropEvent* ev);
private:
    void initMenuBar();
    void onStartLoadPE(const QString& fileName);
    void retranslateUi();
    void updateDataDirectory();
    
    
private:
    GlobalData m_gd;
    QVBoxLayout* _mainLayout;
    PEHeaderItemListWidget* m_itemListWidget; 
    DataDirectoryWidget* m_dataDirectoryTbl;
    DosWidget* m_dosWidget;
    QLabel* m_lblPEPath;
    QMenuBar* m_menubar;
    QWidget* m_statusbar;
    QTabWidget* m_tabWidget = nullptr;
    QLabel* m_lblDataDirectory;
    sectionTable* m_secTbl;
    QTranslator *m_translator;
    bool m_bTranslator = false;
    int m_nTabSectionHeader = -1;
private:
    QMenu* m_pPEInfoMenu;
    QMenu* m_pHelpMenu;
    QAction* m_pActionOpenPE;
    QAction* m_pActionSWitchLanguage;
    QAction* m_pActionAbout;
    QPushButton* m_btnRVATrans;
    QPushButton* m_btnClose;
private:
	void initUI();
private slots:
   void ShowPEMainWindow();
   void SwitchLanguage();
   void ShowAboutDialog();
signals:
    void startLoadPEBasic(const QString& fileName);
public slots:
      void onShowPEInfo();
      void OnShowRVAToRAWWindow();
      void OnClose();
};


#endif
