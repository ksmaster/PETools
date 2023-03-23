#ifndef PEWINDOW_H
#define PEWINDOW_H

#include <QMainWindow>
#include <memory>
#include <QThread>
#include "globaldata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PEWindow; }
QT_END_NAMESPACE

class PEWindow : public QMainWindow
{
    Q_OBJECT

public:
    PEWindow( QWidget *parent = nullptr);
    ~PEWindow();
private:
    void ShowPEWindow();
private:
    Ui::PEWindow *ui;
    GlobalData m_gd;
protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void onStartLoadPE(const QString &fileName);
 signals:
    void startLoadPEBasic(const QString &fileName);
  public slots:
    void ShowPEBasicResult();
};
#endif // PEWINDOW_H
