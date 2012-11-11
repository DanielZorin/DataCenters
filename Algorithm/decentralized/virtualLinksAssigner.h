#ifndef VIRTUAL_LINKS_ASSIGNER_H_
#define VIRTUAL_LINKS_ASSIGNER_H_

#include "link.h"
#include "elementsAssigner.h"
#include "request.h"

class VirtualLinksAssigner : public ElementsAssigner
{
public:
    typedef std::map<Request::VirtualLinks * , Request * > RequestsVirtualLinks;
public:
    VirtualLinksAssigner(Network* network, RequestAssignment* virtualMachinesAssignments, 
        RequestAssignment* storagesAssignments)
    :
        ElementsAssigner(network),
        virtualMachinesAssignments(virtualMachinesAssignments),
        storagesAssignments(storagesAssignments)
    {
    }

    virtual ~VirtualLinksAssigner();

public:
    // Perform the assignment of virtual links to resource network.
    // Returns the requests, for which assignments are succeded, and
    // forms the assigmnets variable, which may be get by the appropriate getter.
    //
    Requests PerformAssignment(Requests& requests);

private:
    // Assign one request's virtual links.
    // Used in the process of assigning of all virtual link sets.
    // Return true if assignment succeded.
    //
    bool assignOneRequest(Request::VirtualLinks * virtualLinks, Assignment* reqAssignment, Request* req);

    // Assign one virtual link.
    // The kPath (may be switched to dejkstra) algorithm is used in this step.
    // Used in the process of assigning of one request's virtual links set.
    // Return true if assignment succeded.
    //
    bool assignOneVirtualLink(Link * virtualLink, Link * physicalLink, Assignment* reqAssignment);

    // Add virtual link with network path found to the assignment.
    void AddVirtualLink(VirtualLink * virtualLink, NetPath* path, Assignment * assignment);

    // Remove virtual link assigned with NetPath from the assignment
    void RemoveVirtualLink(VirtualLink * virtualLink, Assignment * assignment);

protected:
    // Limited exhaustive search.
    //
    bool limitedExhaustiveSearch(Element * element, Assignment* assignment);

private:
    // Get the appropriate physical resource, on which the virtual resource us assigned
    Element * getAssigned(Element * virtualResource, Request* req);

private:
    // Virtual machines and storages assignments to know
    // the assignments of virtual links vertexes.
    RequestAssignment* virtualMachinesAssignments;
    RequestAssignment* storagesAssignments;
    /*
private:
    // Methods-substeps in the limited exhaustive search algorithm

    // Get the map of all assigned virtual links with capacity less then element's one
    // and, additionally, all vm's assignments.
    void getAvailableNodeAssignments(Element* element, std::map<Node*, std::vector<Node*> >& nodesAssignments, std::map<Node*, Assignment* >& vmAssignment, Assignment* assignment);
    */
};

#endif