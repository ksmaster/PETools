#include <memory>
#include <QApplication>
#include "pewindow.h"
#include "globaldata.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PEWindow w;
    w.show();
    return a.exec();
}
