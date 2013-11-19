#include "mainwindow.h"
#include "runner.h"

int runQtShell(int & argc, char ** argv)
{
    QApplication app(argc, argv);
    OurMainWindow* mw = new OurMainWindow(argc, argv);
    return app.exec();
}
