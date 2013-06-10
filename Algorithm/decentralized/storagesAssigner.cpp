#include "storagesAssigner.h"

#include "algorithm.h"
#include "assignment.h"
#include "criteria.h"
#include "network.h"

#include <vector>
#include <algorithm>
#include <iostream>

StoragesAssigner::~StoragesAssigner()
{
}

// Functions used by sort algorithm to compare resource's weights.
bool requestStoragesCompare(Request::Storages* st1, Request::Storages* st2)
{
    return Criteria::requestStoragesWeight(st1) > Criteria::requestStoragesWeight(st2);
}
bool storagesCompare(Store * st1, Store * st2)
{
    return Criteria::storageWeight(st1) > Criteria::storageWeight(st2);
}
bool storesCompare(Store * st1, Store * st2)
{
    if ( Criteria::getPackMode() == Criteria::NETWORK_CRITICAL )
        return Criteria::storageWeight(st1) > Criteria::storageWeight(st2);
    return Criteria::storageWeight(st1) < Criteria::storageWeight(st2);
}

Requests StoragesAssigner::PerformAssignment(Requests& requests)
{
    Requests assignedRequests; // the result set

    Requests::iterator it = requests.begin();
    Requests::iterator itEnd = requests.end();

    // generate the assosiative set of nodes
    RequestsStorages requestsStorages;

    // form the vector from the set to have an ability to sort it
    std::vector<Request::Storages * > storages;
    storages.reserve(requests.size());
    for ( ; it != itEnd; ++it )
    {
        requestsStorages[&(*it)->getStorages()] = (*it);
        storages.push_back(&(*it)->getStorages());
    }

    std::sort(storages.begin(), storages.end(), requestStoragesCompare);

    for ( unsigned requestIndex = 0; requestIndex < storages.size(); ++requestIndex )
    {
        Request::Storages * st = storages[requestIndex];
        // generate new assignment for the chosen storage
        Assignment* reqAssignment = new Assignment();
        requestAssignment[requestsStorages[st]] = reqAssignment;

        bool result = assignOneRequest(st, reqAssignment);
        if ( result ) // virtual machine assigned successfully 
            assignedRequests.insert(requestsStorages[st]);
        else
        {
            requestAssignment.erase(requestsStorages[st]);
            delete reqAssignment;
        }
    }

    return assignedRequests;
}

bool StoragesAssigner::assignOneRequest(Request::Storages * storages, Assignment* reqAssignment)
{
    // form the vector from the set to have an ability to sort it
    std::vector<Store * > storagesVec(storages->begin(), storages->end());
    std::sort(storagesVec.begin(), storagesVec.end(), storagesCompare);
    for ( unsigned index = 0; index < storagesVec.size(); ++index )
    {
        bool result = assignOneStorage(storagesVec[index], reqAssignment);
        if ( !result ) 
        {
            result = limitedExhaustiveSearch(storagesVec[index], reqAssignment, NULL);
            if ( !result )
            {
                // remove assignments
                for ( unsigned i = 0; i < index; ++i )
                {
                    reqAssignment->GetAssignment(storagesVec[i])->RemoveAssignment(storagesVec[i]);
                }

                // tell the upper layer to delete assignment
                return false;
            }
        }
    }

    return true;
}

bool StoragesAssigner::assignOneStorage(Store * storage, Assignment* reqAssignment)
{
    // assignment failed, trying other storages
    // form the vector from the set to have an ability to sort it
    // !! getting the set of stores of the appropriate type
    Stores::const_iterator it = network->getStores().begin();
    Stores::const_iterator itEnd = network->getStores().end();

    std::vector<Store * > stores;
    for ( ; it != itEnd; ++it )
    {
        if ( storage->getTypeOfStore() == (*it)->getTypeOfStore() )
            stores.push_back(*it);
    }

    std::sort(stores.begin(), stores.end(), storesCompare);

    for ( unsigned index = 0; index < stores.size(); ++index )
    {
        if ( stores[index]->getCapacity() >= storage->getCapacity() )
        {
            stores[index]->assign(*storage);
            reqAssignment->AddAssignment(storage, stores[index]);
            return true;
        }
    }
    return false;
}

bool decreaseOrder(Store * st1, Store * st2)
{
    return Criteria::storageWeight(st1) > Criteria::storageWeight(st2);
}

bool StoragesAssigner::limitedExhaustiveSearch(Element * element, Assignment* assignment, Request* req)
{
    // first, forming all assigned virtual machines of nodes
    // it is essential to reassign only elements with capacity less then
    // element's capacity.
    std::map<Store*, std::vector<Store *> > STsOnNode;
    std::map<Store*, Assignment* > stAssignment;
    getAvailableStoreAssignments(element, STsOnNode, stAssignment, assignment);

    std::cerr << "     Exhaustive search\n";
    for ( unsigned depth = 1; depth <= Criteria::exhaustiveSearchDepthComputational(); ++depth )
    {
        std::cerr << "            depth = " << depth << "\n";
        
        std::vector<Storage*> vec;
        vec.reserve(depth);
        if ( recursiveExhaustiveSearch(element, assignment, STsOnNode, stAssignment, STsOnNode.begin(),
                vec, depth) )
            return true;
    }
    return false;
}

bool StoragesAssigner::recursiveExhaustiveSearch(Element * element, Assignment* assignment,
                                                std::map<Store*, std::vector<Store *> >& STsOnNode,
                                                std::map<Store*, Assignment* >& stAssignment,
                                                std::map<Store*, std::vector<Store *> >::iterator curIt,
                                                std::vector<Storage*>& stsSetToAssign,
                                                unsigned depth)
{
    if ( curIt == STsOnNode.end() )
        return false;

    for ( ; curIt != STsOnNode.end(); ++curIt )
    {
        // removing all vms of node
        std::vector<Store *>::iterator it = curIt->second.begin();
        std::vector<Store *>::iterator itEnd = curIt->second.end();

        if ( it != itEnd )
        {
            std::vector<Storage*>vec = stsSetToAssign;
            for ( ; it != itEnd; ++it )
            {
                vec.push_back(*it);
                stAssignment[*it]->RemoveAssignment(*it);
                curIt->first->RemoveAssignment(*it);
            }

            if ( depth == 1 )
            {
                if ( tryToAssign(element, assignment, stAssignment, vec, curIt->first) )
                    return true;
            } else {
                std::map<Store*, std::vector<Store *> >::iterator nextIt = curIt;
                ++nextIt;
                if ( recursiveExhaustiveSearch(element, assignment, STsOnNode, stAssignment, nextIt, 
                        vec, depth - 1) )
                    return true;
            }

            // assign again
            it = curIt->second.begin();
            for ( ; it != itEnd; ++it )
            {
                stAssignment[*it]->AddAssignment(*it, curIt->first);
                curIt->first->assign(*(*it));
            }
        }
    }
    return false;
}

bool StoragesAssigner::tryToAssign(Element * element, Assignment* assignment,
                                  std::map<Store*, Assignment* >& stAssignment,
                                  std::vector<Storage*>& stsSetToAssign, Store* storeToAssign)
{
    storeToAssign->assign(*element);

    // sorting in the decrease order
    std::sort(stsSetToAssign.begin(), stsSetToAssign.end(), decreaseOrder);
    std::vector<Storage *>::iterator stIt = stsSetToAssign.begin();
    std::vector<Storage *>::iterator stItEnd = stsSetToAssign.end();

    bool assigned = true;
    for ( ; stIt != stItEnd; ++stIt )
    {
        bool result = assignOneStorage(*stIt, stAssignment[*stIt]);
        if ( !result )
        {
            // attempt failed
            // retrieving assignments
            storeToAssign->RemoveAssignment(element);
            std::vector<Store *>::iterator stAssignedIt = stsSetToAssign.begin();
            for ( ; stAssignedIt != stIt; ++stAssignedIt )
            {
                stAssignment[*stAssignedIt]->GetAssignment(*stAssignedIt)->RemoveAssignment(*stAssignedIt);
                stAssignment[*stAssignedIt]->RemoveAssignment(*stAssignedIt);
            }

            assigned = false;
            break;
        }
    }

    if ( assigned )
    {
        assignment->AddAssignment(static_cast<Store*>(element), storeToAssign);
        return true;
    }
    return false;
}

void StoragesAssigner::getAvailableStoreAssignments(Element* element, std::map<Store*, std::vector<Store*> >& STsOnStore, std::map<Store*, Assignment* >& stAssignment, Assignment* assignment)
{
    Stores::iterator storesIt = network->getStores().begin();
    Stores::iterator storesItEnd = network->getStores().end();
    for ( ; storesIt != storesItEnd; ++storesIt )
    {
        if ( (*storesIt)->getTypeOfStore() == static_cast<Store*>(element)->getTypeOfStore() )
        {
            // inserting only vms with capacity less then the element's one
            STsOnStore[*storesIt] = std::vector<Store *>();

            // going through all other assigned requests
            RequestAssignment::iterator it = requestAssignment.begin();
            RequestAssignment::iterator itEnd = requestAssignment.end();
            for ( ; it != itEnd; ++it )
            {
                Stores storages = it->second->GetAssigned(*storesIt);
                Stores::iterator stIt = storages.begin();
                Stores::iterator stItEnd = storages.end();
                for ( ; stIt != stItEnd; ++stIt )
                {
                    STsOnStore[*storesIt].push_back(*stIt);
                    stAssignment[*stIt] = it->second;
                }
            }
        }
    }
}
