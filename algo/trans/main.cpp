#include "defs.h"
#include "snapshot.h"

#include <QString>

#include <stdio.h>

int main(int argc, char ** argv)
{
    if ( argc != 3 )
    {
        printf("Usage: %s <initial configuration> <final configuration>\n", *argv);
        return 1;
    }

   Snapshot from, to;
   from.read(argv[1]);
   to.read(argv[2]);

   return 0;
}
