#include <stdio.h>
#include "computer.h"


int main(int argc, char ** argv)
{
    if ( argc < 3 || argc > 5 )
    {
        printf("Usage: %s <input_file> <-w/-c> [output_file = input_file] [algorithm = [c|d]]\n", *argv);
        return 1;
    }

    return 0;
}
