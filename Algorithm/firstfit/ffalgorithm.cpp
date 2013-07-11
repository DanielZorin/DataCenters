#include "ffalgorithm.h"
#include "request.h"
#include "assignment.h"

#include "network.h"
#include "node.h"
#include "store.h"
#include "link.h"

#include "decentralized/virtualLinkRouter.h"

FirstFitAlgorithm::FirstFitAlgorithm(Network * n, Requests const & r)
:
    Algorithm(n, r)
{}

Algorithm::Result FirstFitAlgorithm::schedule()
{
    for (Requests::iterator i = requests.begin(); i != requests.end(); i++)
    {
        Request * request = *i;
        Assignment * assignment = new Assignment(request);

        Nodes & vms = request->getVirtualMachines();
        for (Nodes::iterator vmi = vms.begin(); vmi != vms.end(); vmi++)
        {
            Node * vm = *vmi;
            Nodes & nodes = network->getNodes();
            Nodes::iterator n;
            for (n = nodes.begin(); n != nodes.end(); n++)
            {
                Node * node = *n;
                if ( node->isAssignmentPossible(*vm) )
                {
                    node->assign(*vm);
                    assignment->AddAssignment(vm, node);
                    break;
                }  
            }
            
            if ( n == nodes.end() )
            {
                delete assignment;
                continue;
            }
        }

        Stores & storages = request->getStorages();
        for (Stores::iterator s = storages.begin(); s != storages.end(); s++)
        {
            Store * storage = *s;
            Stores & stores = network->getStores();
            Stores::iterator si;
            for ( si = stores.begin(); si != stores.end(); si++)
            {
                Store * store = *si;
                if ( store->isAssignmentPossible(*storage) )
                {
                    store->assign(*storage);
                    assignment->AddAssignment(storage, store);
                    break;
                }
            }

            if ( si == storages.end() )
            {
                delete assignment;
                continue;
            }
        }

        Links & vlinks = request->getVirtualLinks();
        for (Links::iterator l = vlinks.begin(); l != vlinks.end(); l++)
        {
            NetPath netPath = VirtualLinkRouter::routeDejkstra(*l, network);
            if ( netPath.size() == 0 )
            {
                delete assignment;
                continue;
            } 

            assignment->AddAssignment(*l, netPath);
        }

        assignments.insert(assignment);
    }

    if ( assignments.size() == requests.size() )
        return SUCCESS;
    else if ( assignments.size() != 0 )
        return PARTIAL;
    else
        return FAILURE;
}
