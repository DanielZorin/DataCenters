#include "virtualMachinesAssigner.h"

#include "algorithm.h"
#include "assignment.h"
#include "criteria.h"
#include "network.h"

#include <vector>
#include <algorithm>

VirtualMachinesAssigner::~VirtualMachinesAssigner()
{
}

// Functions used by sort algorithm to campare resource's weights.
bool requestVirtualMachinesCompare(Request::VirtualMachines* vm1, Request::VirtualMachines* vm2)
{
    return Criteria::requestVirtualMachinesWeight(vm1) < Criteria::requestVirtualMachinesWeight(vm2);
}
bool virtualMachinesCompare(Node * vm1, Node * vm2)
{
    return Criteria::virtualMachineWeight(vm1) < Criteria::virtualMachineWeight(vm2);
}

Requests VirtualMachinesAssigner::PerformAssignment(Requests& requests)
{
    Requests assignedRequests; // the result set

    Requests::iterator it = requests.begin();
    Requests::iterator itEnd = requests.end();

    // generate the assosiative set of nodes
    RequestsVirtualMachines requestsVirtualMachines;

    // form the vector from the set to have an ability to sort it
    std::vector<Request::VirtualMachines * > virtualMachines;
    virtualMachines.reserve(requests.size());
    for ( ; it != itEnd; ++it )
    {
        requestsVirtualMachines[&(*it)->getVirtualMachines()] = (*it);
        virtualMachines.push_back(&(*it)->getVirtualMachines());
    }

    std::sort(virtualMachines.begin(), virtualMachines.end(), requestVirtualMachinesCompare);

    for ( unsigned requestIndex = 0; requestIndex < virtualMachines.size(); ++requestIndex )
    {
        Request::VirtualMachines * vm = virtualMachines[requestIndex];
        // generate new assignment for the chosen virtual machine
        Assignment* reqAssignment = new Assignment();
        requestAssignment[requestsVirtualMachines[vm]] = reqAssignment;

        bool result = assignOneRequest(vm, reqAssignment);
        if ( result ) // virtual machine assigned successfully 
            assignedRequests.insert(requestsVirtualMachines[vm]);
        else
        {
            requestAssignment.erase(requestsVirtualMachines[vm]);
            delete reqAssignment;
        }
    }

    return assignedRequests;
}

bool VirtualMachinesAssigner::assignOneRequest(Request::VirtualMachines * virtualMachines, Assignment* reqAssignment)
{
    // form the vector from the set to have an ability to sort it
    std::vector<Node * > virtualMachinesVec(virtualMachines->begin(), virtualMachines->end());
    std::sort(virtualMachinesVec.begin(), virtualMachinesVec.end(), virtualMachinesCompare);
    for ( unsigned index = 0; index < virtualMachinesVec.size(); ++index )
    {
        bool result = assignOneVirtualMachine(virtualMachinesVec[index], reqAssignment);
        if ( !result ) 
        {
            // trying limited exhaustive search
            result = limitedExhaustiveSearch(virtualMachinesVec[index], reqAssignment, NULL);
            if ( !result )
            {
                printf("    Request assignment failed, removing virtual machines\n");
                // remove assignments
                for ( unsigned i = 0; i < index; ++i )
                {
                    printf("Virtual machine %s is removed from node %s\n", virtualMachinesVec[i]->getName().c_str(), reqAssignment->GetAssignment(virtualMachinesVec[i])->getName().c_str());
                    reqAssignment->GetAssignment(virtualMachinesVec[i])->RemoveAssignment(virtualMachinesVec[i]);
                }

                // tell the upper layer to delete assignment
                return false;
            }
        }
    }

    return true;
}

bool VirtualMachinesAssigner::assignOneVirtualMachine(Node * virtualMachine, Assignment* reqAssignment)
{
    // form the vector from the set to have an ability to sort it
    std::vector<Node * > nodes(network->getNodes().begin(), network->getNodes().end());
    std::sort(nodes.begin(), nodes.end(), virtualMachinesCompare);

    for ( unsigned index = 0; index < nodes.size(); ++index )
    {
        if ( nodes[index]->getCapacity() >= virtualMachine->getCapacity() )
        {
            nodes[index]->assign(*virtualMachine);
            reqAssignment->AddAssignment(virtualMachine, nodes[index]);
            printf("Assigned virtual machine %s on node %s\n", virtualMachine->getName().c_str(), nodes[index]->getName().c_str());
            return true;
        }
    }
    return false;
}

bool decreaseOrder(Node * vm1, Node * vm2)
{
    return Criteria::virtualMachineWeight(vm1) > Criteria::virtualMachineWeight(vm2);
}

bool VirtualMachinesAssigner::limitedExhaustiveSearch(Element * element, Assignment* assignment, Request* req)
{
    printf("  Request assignment failed, trying limited exhaustive search\n");

    // first, forming all assigned virtual machines of nodes
    // it is essential to reassign only elements with capacity less then
    // element's capacity.
    std::map<Node*, std::vector<Node *> > VMsOnNode;
    std::map<Node*, Assignment* > vmAssignment;
    getAvailableNodeAssignments(element, VMsOnNode, vmAssignment, assignment);

    // searching of all available nodes, in which, after removing
    // all of nodes with capacity less then element's one, the space is enough
    // for assigning the element
    std::map<Node*, std::vector<Node *> >::iterator it = VMsOnNode.begin();
    std::map<Node*, std::vector<Node *> >::iterator itEnd = VMsOnNode.end();
    for ( ; it != itEnd; ++it )
    {
        unsigned long capacitySum = it->first->getCapacity();

        // sorting in the decrease order
        std::sort(it->second.begin(), it->second.end(), decreaseOrder);
        std::vector<Node *>::iterator vmIt = it->second.begin();
        std::vector<Node *>::iterator vmItEnd = it->second.end();
        for ( ; vmIt != vmItEnd; ++vmIt )
            capacitySum += (*vmIt)->getCapacity();

        if ( capacitySum >= element->getCapacity() )
        {
            // removing all vms, assigning the current element and trying to
            // reassign removed vms.
            for ( vmIt = it->second.begin(); vmIt != vmItEnd; ++vmIt )
            {
                vmAssignment[*vmIt]->RemoveAssignment(*vmIt);
                it->first->RemoveAssignment(*vmIt);
            }

            it->first->assign(*element);

            vmIt = it->second.begin();
            bool assigned = true;
            for ( vmIt = it->second.begin(); vmIt != vmItEnd; ++vmIt )
            {
                bool result = assignOneVirtualMachine(*vmIt, vmAssignment[*vmIt]);
                if ( !result )
                {
                    // attempt failed
                    // retrieving assignments
                    std::vector<Node *>::iterator vmAssignedIt = it->second.begin();
                    for ( ; vmAssignedIt != vmIt; ++vmAssignedIt )
                    {
                        vmAssignment[*vmAssignedIt]->GetAssignment(*vmAssignedIt)->RemoveAssignment(*vmAssignedIt);
                        vmAssignment[*vmAssignedIt]->RemoveAssignment(*vmAssignedIt);
                    }
                    it->first->RemoveAssignment(element);
                    assigned = false;
                    break;
                }
            }

            if ( assigned )
            {
                printf("  Limited exhaustive search succeded, assigning vm %s to node %s\n", element->getName().c_str(), it->first->getName().c_str());
                printf("  Reassignments:\n");
                for ( vmIt = it->second.begin(); vmIt != vmItEnd; ++vmIt )
                {
                    printf("    Vm %s on node %s\n", (*vmIt)->getName().c_str(), vmAssignment[*vmIt]->GetAssignment(*vmIt)->getName().c_str());
                }
                assignment->AddAssignment(static_cast<Node*>(element), it->first);
                return true;
            }
        }
    }
    return false;
}

void VirtualMachinesAssigner::getAvailableNodeAssignments(Element* element, std::map<Node*, std::vector<Node*> >& VMsOnNode, std::map<Node*, Assignment* >& vmAssignment, Assignment* assignment)
{
    Nodes::iterator nodesIt = network->getNodes().begin();
    Nodes::iterator nodesItEnd = network->getNodes().end();
    for ( ; nodesIt != nodesItEnd; ++nodesIt )
    {
        // get list of assignments of current vm first
        Nodes vms = assignment->GetAssigned(*nodesIt);

        Nodes::iterator vmIt = vms.begin();
        Nodes::iterator vmItEnd = vms.end();

        // inserting only vms with capacity less then the element's one
        VMsOnNode[*nodesIt] = std::vector<Node *>();
        for ( ; vmIt != vmItEnd; ++vmIt )
            if ( (*vmIt)->getCapacity() < element->getCapacity() )
            {
                VMsOnNode[*nodesIt].push_back(*vmIt);
                vmAssignment[*vmIt] = assignment;
            }

        // going through all other assigned requests
        RequestAssignment::iterator it = requestAssignment.begin();
        RequestAssignment::iterator itEnd = requestAssignment.end();
        for ( ; it != itEnd; ++it )
        {
            Nodes vms = it->second->GetAssigned(*nodesIt);
            vmIt = vms.begin();
            vmItEnd = vms.end();
            for ( ; vmIt != vmItEnd; ++vmIt )
                if ( (*vmIt)->getCapacity() < element->getCapacity() )
                {
                    VMsOnNode[*nodesIt].push_back(*vmIt);
                    vmAssignment[*vmIt] = it->second;
                }
        }
    }
}
