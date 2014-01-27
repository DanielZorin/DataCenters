#include "testalgorithm.h"

#include "network.h"
#include "request.h"

#include <stdio.h>

void TestAlgorithm::schedule() {
   printf("[TA] there are %d elements available in network, including:\n"
         "%d computers, %d stores, %d switches and %d links\n",
         network->availableElements().size(),
         network->getComputers().size(),
         network->getStores().size(),
         network->getSwitches().size(),
         network->getLinks().size());
}
