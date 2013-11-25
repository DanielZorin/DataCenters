#include "testrouter.h"

#include "common/link.h"

#include <stdio.h>

bool TestRouter::route()
{
    if ( !validateInput() )
        return false;

    printf("[test] Requested to route link %s from node %s to node %s\n",
            link->getName().c_str(),
            link->getFirst()->getName().c_str(),
            link->getSecond()->getName().c_str()); 
    return true;
}
