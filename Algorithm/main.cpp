#include <iostream>
using std::cerr;
using std::endl;
#include "mainwindow_moc.h"

int main(int argc, char ** argv)
{
    if ( argc < 2 || argc > 4 )
    {
        cerr << "Usage: " << argv[0] << " <input_file> [output_file = input_file] [algorithm = [a|c|d]]" << endl;
        return 1;
    }

	QApplication app(argc, argv);
	OurMainWindow* mw = new OurMainWindow(argc, argv);
	return app.exec();
}
