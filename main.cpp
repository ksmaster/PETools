#include <memory>
#include <QApplication>
#include <QTranslator>
//#include "pewindow.h"
#include "PEMainWindow.h"
#include "globaldata.h"


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icon/pe"));
    PEMainWindow w;
    w.show();
    return a.exec();
}
