#include "decentralizedAlgorithm.h"

#include <algorithm>
#include <vector>

#include "node.h"
#include "store.h"
#include "networkingElement.h"
#include "request.h"
#include "assignment.h"
#include "network.h"

DecentralizedAlgorithm::~DecentralizedAlgorithm()
{
    Assignments::iterator it = assignments.begin();
    while ( it != assignments.end() )
        delete (*it);
    assignments.clear();
}

DecentralizedAlgorithm::ResultEnum::Result DecentralizedAlgorithm::schedule()
{
    Requests::iterator it = requests.begin();
    Requests::iterator itEnd = requests.end();

    // generate the set of nodes
    RequestsVirtualMachines virtualMachines;
    for ( ; it != itEnd; ++it )
    {
        virtualMachines.insert(&(*it)->getVirtualMachines());
    }

    Requests assignedRequests = assignComputationalElements(virtualMachines);

    // generate the set of stores
    it = assignedRequests.begin();
    itEnd = assignedRequests.end();

    RequestsStorages storages;
    for ( ; it != itEnd; ++it )
    {
        storages.insert(&((*it)->getStorages()));
    }

    assignedRequests = assignComputationalElements(storages);

    // generate the set of virtual links
    it = assignedRequests.begin();
    itEnd = assignedRequests.end();

    RequestsVirtualLinks virtualLinks;
    for ( ; it != itEnd; ++it )
    {
        virtualLinks.insert(&((*it)->getVirtualLinks()));
    }

    ResultEnum::Result result = assignVirtualLinks(virtualLinks);

    return result;
}

bool requestVirtualMachinesCompare(Request::VirtualMachines* vm1, Request::VirtualMachines* vm2)
{
    return DecentralizedAlgorithm::requestVirtualMachinesWeight(vm1) < DecentralizedAlgorithm::requestVirtualMachinesWeight(vm2);
}
bool virtualMachinesCompare(Node * vm1, Node * vm2)
{
    return DecentralizedAlgorithm::virtualMachineWeight(vm1) < DecentralizedAlgorithm::virtualMachineWeight(vm2);
}

Algorithm::Requests DecentralizedAlgorithm::assignComputationalElements(RequestsVirtualMachines & requestVirtualMachines)
{
    // form the vector from the set to have an ability to sort it
    std::vector<Request::VirtualMachines * > virtualMachines(requestVirtualMachines.begin(), requestVirtualMachines.end());
    std::sort(virtualMachines.begin(), virtualMachines.end(), requestVirtualMachinesCompare);

    for ( unsigned requestIndex = 0; requestIndex < virtualMachines.size(); ++requestIndex )
    {
        bool result = assignOneRequestResourses(virtualMachines[requestIndex]);
    }
    return Requests();
}

Algorithm::Requests DecentralizedAlgorithm::assignComputationalElements(RequestsStorages& requestStorages)
{
    return Requests();
}

Algorithm::ResultEnum::Result DecentralizedAlgorithm::assignVirtualLinks(RequestsVirtualLinks& virtualLinks)
{
    return ResultEnum::SUCCESS;
}

bool DecentralizedAlgorithm::assignOneRequestResourses(Request::VirtualMachines * virtualMachines)
{
    Assignment* localAssignment = new Assignment();
    // form the vector from the set to have an ability to sort it
    std::vector<Node * > virtualMachinesVec(virtualMachines->begin(), virtualMachines->end());
    std::sort(virtualMachinesVec.begin(), virtualMachinesVec.end(), virtualMachinesCompare);
    for ( unsigned index = 0; index < virtualMachinesVec.size(); ++index )
    {
        bool result = assignOneComputationalElement(virtualMachinesVec[index], *localAssignment);
        // TODO: insert the operation of limited exaustive search here in case result = false
        if ( !result )
        {
            // remove assignments
            for ( unsigned i = 0; i < index; ++i )
                localAssignment->GetAssignment(virtualMachinesVec[i])->RemoveAssignment(virtualMachinesVec[i]);

            delete localAssignment;
            return false;
        }
    }

    // insert assignment
    assignments.insert(localAssignment);
}

bool DecentralizedAlgorithm::assignOneComputationalElement(ComputationalElement* element, Assignment& localAssignment)
{
    // form the vector from the set to have an ability to sort it
    std::vector<Node * > nodes(network->getNodes().begin(), network->getNodes().end());
    std::sort(nodes.begin(), nodes.end(), virtualMachinesCompare);

    for ( unsigned index = 0; index < nodes.size(); ++index )
    {
        if ( nodes[index]->getCapacity() >= element->getCapacity() )
        {
            nodes[index]->Assign(*element);
            localAssignment.AddAssignment((Node*)element, nodes[index]);
            return true;
        }
    }
    return false;
}

long DecentralizedAlgorithm::requestVirtualMachinesWeight(Request::VirtualMachines* virtualMachines)
{
    int result = 0;
    Request::VirtualMachines::iterator it = virtualMachines->begin();
    Request::VirtualMachines::iterator itEnd = virtualMachines->end();

    for ( ; it != itEnd; ++it )
    {
        result += (*it)->getCapacity();
    }

    return result;
}

long DecentralizedAlgorithm::virtualMachineWeight(Node* virtualMachine)
{
    return virtualMachine->getCapacity();
}
