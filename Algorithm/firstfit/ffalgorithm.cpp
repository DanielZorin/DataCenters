#include "ffalgorithm.h"
#include "request.h"
#include "assignment.h"

#include "network.h"
#include "node.h"
#include "store.h"
#include "link.h"

#include "decentralized/virtualLinkRouter.h"
#include <stdio.h>

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
        Algorithm::Result result = SUCCESS;

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
                result = FAILURE;
                break;
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
                result = FAILURE;
                break;
            }
        }

        Links & vlinks = request->getVirtualLinks();
        for (Links::iterator l = vlinks.begin(); l != vlinks.end(); l++)
        {
            Link * dummy = createDummyLink(*l, assignment);

            if ( dummy == 0 )
            {
                result = FAILURE;
                break;
            }

            NetPath netPath = VirtualLinkRouter::routeDejkstra(dummy, network);
            delete dummy;

            if ( netPath.size() == 0 )
            {
                result = FAILURE;
                break;
            } 

            for ( NetPath::iterator i = netPath.begin(); i != netPath.end(); i++)
            {
               (*i)->assign(**l);
            }
            assignment->AddAssignment(*l, netPath);
        }

        if ( result == FAILURE )
            delete assignment;
        else
            assignments.insert(assignment);
    }

    printf("Assigned total of %d from %d requests", assignments.size(), requests.size());
    if ( assignments.size() == requests.size() )
        return SUCCESS;
    else if ( assignments.size() != 0 )
        return PARTIAL;
    else
        return FAILURE;
}

Link * FirstFitAlgorithm::createDummyLink(Link * virtualLink, Assignment * assignment)
{
    if ( virtualLink == 0 )
        return 0;

    Element * first = getCastedAssignment(virtualLink->getFirst(), assignment);
    Element * second = getCastedAssignment(virtualLink->getSecond(), assignment);

    if ( first == 0 || second == 0 )
        return 0;

    Link * dummyLink = new Link("dummy_link", virtualLink->getCapacity(), virtualLink->getMaxCapacity());
    dummyLink->bindElements(first, second);
    return dummyLink;
}

Element * FirstFitAlgorithm::getCastedAssignment(Element * element, Assignment * assignment)
{
    if ( element->isNode() )
        return assignment->GetAssignment((Node *)element);
    if ( element->isStore() )
        return assignment->GetAssignment((Store *)element);
    return 0;
}
