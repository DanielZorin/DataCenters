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


CentralizedAlgorithm::CentralizedAlgorithm(Network * n, Requests const& r)
: 
    Algorithm(n, r),
    nodeManager(getNetwork().getNodes()),
    storeManager(getNetwork().getStores()),
    networkManager(getNetwork())
{
    cerr << "Constructed centralized algorithm to assign " << r.size() << " requests." << endl; 
}

Algorithm::Result CentralizedAlgorithm::schedule()
{
    vector<Request *> prioritizedRequests = prioritize<Request>(requests);     

    int assignedRequests = 0;
    int assigningTries = 0;

    for (vector<Request *>::iterator i = prioritizedRequests.begin(),
            e = prioritizedRequests.end();
            i != e ; i++ )
    {
        Request * request = *i;
        currentAssignment = new Assignment(request); 

        cerr << "[CA] Building assignment" << endl;
        assigningTries++;

        Result assignmentResult;

        cerr << "[CA]\tAssigning virtual machines" << endl;
        assignmentResult = buildVMAssignment(request);
        if ( assignmentResult != SUCCESS )
        {
            delete currentAssignment;
            cerr << "[CA]\tAssigning failed" << endl;
            cerr << "[CA]Assigned " << assignedRequests << " from " << assigningTries << endl;
            continue;
        }
        cerr << "[CA]\tAssigning succeeded" << endl;

        cerr << "[CA]\tAssigning storages" << endl;
        assignmentResult = buildStorageAssignment(request);
        if ( assignmentResult != SUCCESS )
        {
            delete currentAssignment;
            cerr << "[CA]\tAssigning failed" << endl;
            cerr << "[CA]Assigned " << assignedRequests << " from " << assigningTries << endl;
            continue;
        }
        cerr << "[CA]\tAssigning succeeded" << endl;
        assignedRequests++;
        cerr << "[CA]Assigned " << assignedRequests << " from " << assigningTries << endl;

        assignments.insert(currentAssignment);

    }

    cerr << "Scheduled " << assignments.size() << " of " << requests.size() << " requests" << endl;

    if ( assignments.size() == requests.size() )
        return SUCCESS;
    else if ( assignments.size() != 0 )
        return PARTIAL;
    else
        return FAILURE;
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
        Node * w = *i;
        cerr << "[CA]\tCurrent wm weight is " << CriteriaCen::weight(w) << endl; 
        Nodes assignedLinkedNodes = getAssignedLinkedNodes(w, request);
        Nodes assignmentCandidates;
        if ( assignedLinkedNodes.empty() )
        {
            assignmentCandidates = nodeManager.getVMAssignmentCandidates(w);
            vector<Node *> prioritizedNodes = prioritize<Node>(assignmentCandidates);
            vector<Node *>::iterator n = prioritizedNodes.begin();
            for ( ; n != prioritizedNodes.end(); n++ )
            {
                cerr << "[CA]\t\tCurrent node weight is " << CriteriaCen::weight(*n) << endl; 
                if ( tryToAssignVM(w, *n) == SUCCESS )
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

                Links vlinks = getConnectedVirtualLinks(w, request);

                for ( ; n != prioritizedNodes.end(); n++ )
                {
                    if ( tryToAssignPathes(w, *n, vlinks) == SUCCESS )
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

            if ( tryToAssignVM(w, candidate) == FAILURE )
                return FAILURE;

        }


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
        Store * s = *i;
        Nodes assignedLinkedNodes = getAssignedLinkedNodes(s, request);
        Store * candidate = 0;
        networkManager.setSearchSpace(assignedLinkedNodes);
        Stores assignmentCandidates = networkManager.getStoreCandidates();
        while ( !assignmentCandidates.empty() )
        {
            vector<Store *> prioritizedStores = prioritize<Store>(assignmentCandidates);
            vector<Store *>::iterator store = prioritizedStores.begin();

            Links vlinks = getConnectedVirtualLinks(s, request);
            for ( ; store != prioritizedStores.end(); store++ )
            {
                if ( tryToAssignPathes(s, *store, vlinks) == SUCCESS )
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

        if ( tryToAssignStorage(s, candidate) == FAILURE )
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
