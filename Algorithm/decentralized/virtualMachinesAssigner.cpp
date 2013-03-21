#include "virtualMachinesAssigner.h"

#include "algorithm.h"
#include "assignment.h"
#include "criteria.h"
#include "network.h"

#include <vector>
#include <algorithm>
#include <iostream>

VirtualMachinesAssigner::~VirtualMachinesAssigner()
{
}

// Functions used by sort algorithm to campare resource's weights.
bool requestVirtualMachinesCompare(Request::VirtualMachines* vm1, Request::VirtualMachines* vm2)
{
    return Criteria::requestVirtualMachinesWeight(vm1) > Criteria::requestVirtualMachinesWeight(vm2);
}
bool virtualMachinesCompare(Node * vm1, Node * vm2)
{
    return Criteria::virtualMachineWeight(vm1) > Criteria::virtualMachineWeight(vm2);
}
bool nodesCompare(Node * n1, Node * n2)
{
    if ( Criteria::getPackMode() == Criteria::NETWORK_CRITICAL )
        return Criteria::virtualMachineWeight(n1) > Criteria::virtualMachineWeight(n2);
    return Criteria::virtualMachineWeight(n1) < Criteria::virtualMachineWeight(n2);
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
                // remove assignments
                for ( unsigned i = 0; i < index; ++i )
                {
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
    // assignment failed, trying other nodes

    // form the vector from the set to have an ability to sort it
    std::vector<Node * > nodes(network->getNodes().begin(), network->getNodes().end());
    std::sort(nodes.begin(), nodes.end(), nodesCompare);

    for ( unsigned index = 0; index < nodes.size(); ++index )
    {
        if ( nodes[index]->isAssignmentPossible(*virtualMachine) )
        {
            nodes[index]->assign(*virtualMachine);
            reqAssignment->AddAssignment(virtualMachine, nodes[index]);
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
    // first, forming all assigned virtual machines of nodes
    // it is essential to reassign only elements with capacity less then
    // element's capacity.
    std::map<Node*, std::vector<Node *> > VMsOnNode;
    std::map<Node*, Assignment* > vmAssignment;
    getAvailableNodeAssignments(element, VMsOnNode, vmAssignment, assignment);

    std::cerr << "     Exhaustive search\n";
    for ( unsigned depth = 1; depth <= Criteria::exhaustiveSearchDepthComputational(); ++depth )
    {
        std::cerr << "            depth = " << depth << "\n";
        
        std::vector<VirtualMachine*> vec;
        vec.reserve(depth);
        if ( recursiveExhaustiveSearch(element, assignment, VMsOnNode, vmAssignment, VMsOnNode.begin(),
                vec, depth) )
            return true;
    }
    return false;
}

bool VirtualMachinesAssigner::recursiveExhaustiveSearch(Element * element, Assignment* assignment,
                                                        std::map<Node*, std::vector<Node *> >& VMsOnNode,
                                                        std::map<Node*, Assignment* >& vmAssignment,
                                                        std::map<Node*, std::vector<Node *> >::iterator curIt,
                                                        std::vector<VirtualMachine*>& vmsSetToAssign,
                                                        unsigned depth)
{
    if ( curIt == VMsOnNode.end() )
        return false;

    for ( ; curIt != VMsOnNode.end(); ++curIt )
    {
        // removing all vms of node
        std::vector<Node *>::iterator it = curIt->second.begin();
        std::vector<Node *>::iterator itEnd = curIt->second.end();

        if ( it != itEnd )
        {
            std::vector<VirtualMachine*>vec = vmsSetToAssign;
            for ( ; it != itEnd; ++it )
            {
                vec.push_back(*it);
                vmAssignment[*it]->RemoveAssignment(*it);
                curIt->first->RemoveAssignment(*it);
            }

            if ( depth == 1 )
            {
                if ( tryToAssign(element, assignment, vmAssignment, vec, curIt->first) )
                    return true;
            } else {
                std::map<Node*, std::vector<Node *> >::iterator nextIt = curIt;
                ++nextIt;
                if ( recursiveExhaustiveSearch(element, assignment, VMsOnNode, vmAssignment, nextIt, 
                        vec, depth - 1) )
                    return true;
            }

            // assign again
            it = curIt->second.begin();
            for ( ; it != itEnd; ++it )
            {
                vmAssignment[*it]->AddAssignment(*it, curIt->first);
                curIt->first->assign(*(*it));
            }
        }
    }
    return false;
}

bool VirtualMachinesAssigner::tryToAssign(Element * element, Assignment* assignment,
                                          std::map<Node*, Assignment* >& vmAssignment,
                                          std::vector<VirtualMachine*>& vmsSetToAssign, Node* nodeToAssign)
{
    nodeToAssign->assign(*element);

    // sorting in the decrease order
    std::sort(vmsSetToAssign.begin(), vmsSetToAssign.end(), decreaseOrder);
    std::vector<Node *>::iterator vmIt = vmsSetToAssign.begin();
    std::vector<Node *>::iterator vmItEnd = vmsSetToAssign.end();

    bool assigned = true;
    for ( ; vmIt != vmItEnd; ++vmIt )
    {
        bool result = assignOneVirtualMachine(*vmIt, vmAssignment[*vmIt]);
        if ( !result )
        {
            // attempt failed
            // retrieving assignments
            nodeToAssign->RemoveAssignment(element);
            std::vector<Node *>::iterator vmAssignedIt = vmsSetToAssign.begin();
            for ( ; vmAssignedIt != vmIt; ++vmAssignedIt )
            {
                vmAssignment[*vmAssignedIt]->GetAssignment(*vmAssignedIt)->RemoveAssignment(*vmAssignedIt);
                vmAssignment[*vmAssignedIt]->RemoveAssignment(*vmAssignedIt);
            }

            assigned = false;
            break;
        }
    }

    if ( assigned )
    {
        assignment->AddAssignment(static_cast<Node*>(element), nodeToAssign);
        return true;
    }
    return false;
}

void VirtualMachinesAssigner::getAvailableNodeAssignments(Element* element, std::map<Node*, std::vector<Node*> >& VMsOnNode, std::map<Node*, Assignment* >& vmAssignment, Assignment* assignment)
{
    Nodes::iterator nodesIt = network->getNodes().begin();
    Nodes::iterator nodesItEnd = network->getNodes().end();
    for ( ; nodesIt != nodesItEnd; ++nodesIt )
    {
        // inserting only vms with capacity less then the element's one
        VMsOnNode[*nodesIt] = std::vector<Node *>();

        // going through all other assigned requests
        RequestAssignment::iterator it = requestAssignment.begin();
        RequestAssignment::iterator itEnd = requestAssignment.end();
        for ( ; it != itEnd; ++it )
        {
            Nodes vms = it->second->GetAssigned(*nodesIt);
            Nodes::iterator vmIt = vms.begin();
            Nodes::iterator vmItEnd = vms.end();
            for ( ; vmIt != vmItEnd; ++vmIt )
            {
                VMsOnNode[*nodesIt].push_back(*vmIt);
                vmAssignment[*vmIt] = it->second;
            }
        }
    }
}
