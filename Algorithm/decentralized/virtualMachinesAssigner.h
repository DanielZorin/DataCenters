#ifndef VIRTUAL_MACHINES_ASSIGNER_H_
#define VIRTUAL_MACHINES_ASSIGNER_H_

#include "node.h"
#include "elementsAssigner.h"
#include "request.h"

class VirtualMachinesAssigner : public ElementsAssigner
{
public:
    typedef std::map<Request::VirtualMachines * , Request * > RequestsVirtualMachines;
public:
    VirtualMachinesAssigner(Network* network)
    :
        ElementsAssigner(network)
    {
    }

    virtual ~VirtualMachinesAssigner();

public:
    // Perform the assignment of virtual machines to computational nodes.
    // Returns the requests, for which assignments are succeded, and
    // forms the assigmnets variable, which may be get by the appropriate getter.
    //
    Requests PerformAssignment(Requests& requests);

private:
    // Assign one request's virtual machine.
    // Used in the process of assigning of all virtual machines sets.
    // Return true if assignment succeded.
    //
    bool assignOneRequest(Request::VirtualMachines * virtualMachines, Assignment* reqAssignment);

    // Assign one virtual machine.
    // Used in the process of assigning of one request's virtual machines set.
    // Return true if assignment succeded.
    //
    bool assignOneVirtualMachine(Node * virtualMachine, Assignment* reqAssignment);

protected:
    // Limited exhaustive search.
    //
    bool limitedExhaustiveSearch(Element * element, Assignment* assignment, Request* req);

private:
    // Methods-substeps in the limited exhaustive search algorithm

    // Get the map of all assigned virtual machines of nodes with capacity less then element's one
    // and, additionally, all vm's assignments.
    void getAvailableNodeAssignments(Element* element, std::map<Node*, std::vector<Node*> >& nodesAssignments, std::map<Node*, Assignment* >& vmAssignment, Assignment* assignment);
};

#endif