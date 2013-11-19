#include "runner.h"
#include <iostream>
using std::cerr;
using std::endl;

int main(int argc, char ** argv)
{
    if ( argc < 3 || argc > 5 )
    {
        cerr << "Usage: " << argv[0] << " <input_file> <-w/-c> [output_file = input_file] [algorithm = [a|c|d]]" << endl;
        return 1;
    }

    return runQtShell(argc, argv);

}
