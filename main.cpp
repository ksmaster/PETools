#include <memory>
#include <QApplication>
#include <QTranslator>
//#include "pewindow.h"
#include "PEMainWindow.h"


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icon/pe"));
    PEMainWindow w;
    w.show();
    int nRet = a.exec();
    return nRet;
}
