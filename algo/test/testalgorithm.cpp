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

    printf("\n");

    printf("[TA] there are %d requests as follows:\n", requests.size());

    for (Requests::iterator i = requests.begin(); i != requests.end(); i++)
    {
        Request * r = *i;
        printf("\t[TA] %d vms, %d storages, %d vswitches, %d tunnels\n",
                r->getMachines().size(),
                r->getStorages().size(),
                r->getVSwitches().size(),
                r->getTunnels().size());
        
    }
}
