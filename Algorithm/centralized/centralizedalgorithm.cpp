#include "centralizedalgorithm.h"
#include "request.h"
#include "assignment.h"

#include "network.h"
#include "criteria_cen.h"
#include "node.h"
#include "store.h"
#include "link.h"

#include <algorithm>
#include <iostream>
using std::cerr;
using std::endl;
using std::set;
using std::vector;


CentralizedAlgorithm::CentralizedAlgorithm(Network * n, Requests const& r, Version v)
: 
    Algorithm(n, r),
    version(v),
    nodeManager(getNetwork().getNodes()),
    storeManager(getNetwork().getStores()),
    networkManager(getNetwork())
{
    cerr << "[CA] Constructed centralized algorithm to assign " << r.size() << " requests." << endl; 
    cerr << "[CA] packing mode is " << (version == NET_PACK ? "net essential" : "net ignorant")<< endl;
}

Algorithm::Result CentralizedAlgorithm::schedule()
{
    vector<Request *> prioritizedRequests = prioritize<Request>(requests);     

    int assignTries = 0;
    int assignSuccesses = 0;

    for (vector<Request *>::iterator i = prioritizedRequests.begin(),
            e = prioritizedRequests.end();
            i != e ; i++ )
    {
        Assignment * assignment;
        assignTries++;
        switch(version)
        {
        case NET_PACK:
            assignment = assignRequestNet(*i);
            break;
        case NEUTRAL_PACK:
        default:
            assignment = assignRequestPlain(*i);
            break;
        }
        if ( assignment != 0 )
        {
            assignments.insert(assignment);
            assignSuccesses++;
        }
        cerr <<"[CA] Assigned " << assignSuccesses << " of " << assignTries << " requests" << endl;
    }

    cerr << "[CA] Scheduled " << assignments.size() << " of " << requests.size() << " requests" << endl;

    if ( assignments.size() == requests.size() )
        return SUCCESS;
    else if ( assignments.size() != 0 )
        return PARTIAL;
    else
        return FAILURE;
}

Assignment * CentralizedAlgorithm::assignRequestPlain(Request * request)
{
    currentAssignment = new Assignment(request); 

    cerr << "[CA] Building assignment" << endl;

    Result assignmentResult;

    cerr << "[CA]\tAssigning virtual machines" << endl;
    assignmentResult = buildVMAssignment(request);
    if ( assignmentResult != SUCCESS )
    {
        cerr << "[CA]\tAssigning failed" << endl;
        cleanUpAssignment(currentAssignment);
        return 0;
    }
    cerr << "[CA]\tAssigning succeeded" << endl;

    cerr << "[CA]\tAssigning storages" << endl;
    assignmentResult = buildStorageAssignment(request);
    if ( assignmentResult != SUCCESS )
    {
        cerr << "[CA]\tAssigning failed" << endl;
        cleanUpAssignment(currentAssignment);
        return 0;
    }
    cerr << "[CA]\tAssigning succeeded" << endl;

    return currentAssignment;
}

Assignment * CentralizedAlgorithm::assignRequestNet(Request * request)
{
    return 0;
}

void CentralizedAlgorithm::cleanUpAssignment(Assignment * assignment)
{
    assignment->forcedCleanup();
    delete assignment;
}

struct Comparator
{
    bool operator() (Request * i, Request * j) 
    {
        return (CriteriaCen::weight(i) > CriteriaCen::weight(j))
            || (CriteriaCen::computationalCount(i) < CriteriaCen::computationalCount(j));   
    }

    bool operator() (Node * i, Node * j)
    {
        return CriteriaCen::weight(i) > CriteriaCen::weight(j);
    }

    bool operator() (Store * i, Store * j)
    {
        return CriteriaCen::weight(i) > CriteriaCen::weight(j);
    }
} comparator; 


template <class T> vector<T *> CentralizedAlgorithm::prioritize(set<T *> & input)
{
    vector<T*> result;
    for (typename set<T *>::iterator i = input.begin(); i != input.end(); i++)
    {
        result.push_back(*i);
    }

    std::sort(result.begin(), result.end(), comparator);
    return result;
}

Algorithm::Result CentralizedAlgorithm::buildVMAssignment(Request * request)
{
    Request::VirtualMachines & vms = request->getVirtualMachines(); 
    vector<Node *> prioritizedVms = prioritize<Node>(vms);

    for (vector<Node *>::iterator i = prioritizedVms.begin(); i != prioritizedVms.end(); i++)
    {
        if ( assignVM(*i, request) != SUCCESS)
            return FAILURE;
    }
    
    return SUCCESS;
}

Algorithm::Result CentralizedAlgorithm::assignVM(Node * vm, Request * request)
{
    Nodes assignedLinkedNodes = getAssignedLinkedNodes(vm, request);
    Nodes assignmentCandidates;
    if ( assignedLinkedNodes.empty() )
    {
        assignmentCandidates = nodeManager.getVMAssignmentCandidates(vm);
        vector<Node *> prioritizedNodes = prioritize<Node>(assignmentCandidates);
        vector<Node *>::iterator n = prioritizedNodes.begin();
        for ( ; n != prioritizedNodes.end(); n++ )
        {
            if ( tryToAssignVM(vm, *n) == SUCCESS )
                break;
        }
        if ( n == prioritizedNodes.end() )
            return FAILURE;
    }
    else
    {
        networkManager.setSearchSpace(assignedLinkedNodes);
        Node * candidate = 0;
        assignmentCandidates = networkManager.getNodeCandidates();
        while ( !assignmentCandidates.empty() )
        {
            vector<Node *> prioritizedNodes = prioritize<Node>(assignmentCandidates);
            vector<Node *>::iterator n = prioritizedNodes.begin();

            Links vlinks = getConnectedVirtualLinks(vm, request);

            for ( ; n != prioritizedNodes.end(); n++ )
            {
                if ( tryToAssignPathes(vm, *n, vlinks) == SUCCESS )
                    break;
            }

            if ( n != prioritizedNodes.end() )
            {
                candidate = *n;
                break;
            }

            assignmentCandidates = networkManager.getNodeCandidates();
        }

        if ( assignmentCandidates.empty() )
            return FAILURE;

        if ( candidate == 0 )
            return FAILURE;

        if ( tryToAssignVM(vm, candidate) == FAILURE )
            return FAILURE;

    }
    
    return SUCCESS;

}

Links CentralizedAlgorithm::getConnectedVirtualLinks(Element * element, Request * request)
{
   Links result;
   Links & links = request->getVirtualLinks();
   for ( Links::iterator i = links.begin(); i != links.end(); i++)
   {
      Link * link = *i;
      if ( link->connectsElement(element) )
         result.insert(link);
   }
   return result;
}



Nodes CentralizedAlgorithm::getAssignedLinkedNodes(Element * e, Request * request)
{
   Nodes result;
   Links links = getConnectedVirtualLinks(e, request);
   for (Links::iterator i = links.begin(); i != links.end(); i++)
   {
      Link * link = *i;
      Element * element = link->getAdjacentElement(e);

      if ( element == 0 )
          continue;

      if ( !element->isNode() )
         continue;

      Node * connectedVM = (Node *)element;
      Node * host = currentAssignment->GetAssignment(connectedVM);
      if ( host == 0 )
         continue;

      result.insert(host);
      
   }
   return result;
}

Algorithm::Result CentralizedAlgorithm::tryToAssignVM(Node * vm, Node * node)
{
    if ( ! node->isAssignmentPossible(*vm) )
        return FAILURE;

    node->assign(*vm);
    currentAssignment->AddAssignment(vm, node);
    return SUCCESS;
}

Algorithm::Result CentralizedAlgorithm::buildStorageAssignment(Request * request)
{
    Request::Storages & stores = request->getStorages();
    vector<Store *> prioritizedStores = prioritize<Store>(stores);

    for (vector<Store *>::iterator i = prioritizedStores.begin(); i != prioritizedStores.end(); i++)
    {
        if ( assignStorage(*i, request) != SUCCESS )
            return FAILURE;
    }

    return SUCCESS;
}

Algorithm::Result CentralizedAlgorithm::assignStorage(Store * storage, Request * request)
{
    Nodes assignedLinkedNodes = getAssignedLinkedNodes(storage, request);
    Stores assignmentCandidates;
    if ( assignedLinkedNodes.empty() )
    {
        assignmentCandidates = storeManager.getStoreAssignmentCandidates(storage);
        vector<Store *> prioritizedStores = prioritize<Store>(assignmentCandidates);
        vector<Store *>::iterator st = prioritizedStores.begin();
        for ( ; st != prioritizedStores.end(); st++ )
        {
            if ( tryToAssignStorage(storage, *st) == SUCCESS )
                break;
        } 

        if ( st == prioritizedStores.end() )
            return FAILURE;
    }
    else
    {
        networkManager.setSearchSpace(assignedLinkedNodes);
        Store * candidate = 0;
        assignmentCandidates = networkManager.getStoreCandidates();
        while ( !assignmentCandidates.empty() )
        {
            vector<Store *> prioritizedStores = prioritize<Store>(assignmentCandidates);
            vector<Store *>::iterator store = prioritizedStores.begin();

            Links vlinks = getConnectedVirtualLinks(storage, request);
            for ( ; store != prioritizedStores.end(); store++ )
            {
                if ( tryToAssignPathes(storage, *store, vlinks) == SUCCESS )
                    break;
            }

            if ( store != prioritizedStores.end() )
            {
                candidate = *store;
                break;
            }

            assignmentCandidates = networkManager.getStoreCandidates();
        }


        if ( assignmentCandidates.empty() )
            return FAILURE;

        if ( candidate == 0 )
            return FAILURE;

        if ( tryToAssignStorage(storage, candidate) == FAILURE )
            return FAILURE;
    }

    return SUCCESS;

}

Algorithm::Result CentralizedAlgorithm::tryToAssignStorage(Store * storage, Store * store)
{
    if ( ! store->isAssignmentPossible(*storage) )
        return FAILURE;

    store->assign(*storage);
    currentAssignment->AddAssignment(storage, store);
    return SUCCESS;
}

Algorithm::Result CentralizedAlgorithm::tryToAssignPathes(Element * assignee, Element * target, Links & links)
{
    for ( Links::iterator l = links.begin(); l != links.end(); l++ )
    {
        Link * vlink = *l;
        Element * assigned = vlink->getAdjacentElement(assignee);
        Element * source = currentAssignment->GetAssignment(assigned);

        if ( assigned == 0 )
        {
            networkManager.cleanUpLinks(links, currentAssignment);
            return FAILURE;
        }

        if ( networkManager.buildPath(source, target, vlink, currentAssignment) == FAILURE )
        {
            networkManager.cleanUpLinks(links, currentAssignment);
            return FAILURE;
        }
          
    }
    return SUCCESS;
}
