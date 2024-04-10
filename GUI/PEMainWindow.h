#ifndef _PE_MAIN_WINDOW_H
#define _PE_MAIN_WINDOW_H

#include <QObject>
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
#include <QDragEnterEvent>
#include <memory>
#include <QThread>
#include "rvatofoawindow.h"
#include "common/qbasiclabel.h"
#include "common/baseWindow.h"
#include "TaskMgr.h"



class QDropEvent;
class QTranslator;
class QTableView;
class PEHeaderItemListWidget;
class DataDirectoryWidget;
class DosWidget;
class sectionTable;
class ExportFunctionWidget;
class importWidget;



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
    //GlobalData m_gd;
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
    importWidget* m_importWidget;
    ExportFunctionWidget* m_exportFunctionWidget;
    QTranslator *m_translator;
    bool m_bTranslator = false;
    QMap<QString, int> m_tabMap;
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
    void addTabItem(const QString&, QWidget*);
    QList<ItemPair> getTabItemPairs();
    void updateALlTabItemName();
private slots:
   void onOpenPE();
   void SwitchLanguage();
   void ShowAboutDialog();
   void onUpdateUI(int nTaskType);
signals:
    void startLoadPEBasic(const QString& fileName);
    
public slots:
      void onShowPEInfo();
      void onShowExportTable();
      void onShowImportTable();
      void onShowRVAToRAWWindow();
      void onClose();
};


#endif
