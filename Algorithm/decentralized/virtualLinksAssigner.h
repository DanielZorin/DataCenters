#ifndef VIRTUAL_LINKS_ASSIGNER_H_
#define VIRTUAL_LINKS_ASSIGNER_H_

#include "algorithm.h"
#include "link.h"
#include "elementsAssigner.h"
#include "request.h"
#include "assignment.h"

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
    bool limitedExhaustiveSearch(Element * element, Assignment* assignment, Request* req);

    // Try to find replication.
    bool replicate(VirtualLink* virtualLink, Assignment* assignment, Request* req);

    // Reassign already assigned virtual link of current request after replication.
    void reassignAfterReplication(VirtualLink* virtualLink, Store* replication, Assignment* assignment, Request* req);

private:
    // Get the map of all assigned virtual links and their assignments.
    void getAllVirtualLinksAssignments(Element* element,
        std::map<VirtualLink*, Assignment* >& vlAssignment, std::map<VirtualLink*, Request* >& vlRequest,
        Assignment* assignment, Request* req);

    // The recursive search of links to reassign, 
    // used in the limited exhaustive search algorithm.
    // Level is the depth of the algorithm, if it is equal to 0, it is
    // the lowest level of the search.
    bool recursiveExhaustiveSearch(VirtualLink * virtualLink, Assignment* assignment,
                                 std::map<VirtualLink*, Assignment* >& vlAssignment,
                                 std::map<VirtualLink*, Request* >& vlRequest,
                                 std::map<VirtualLink*, Assignment* >::iterator curIt,
                                 Links& removedVirtualLinks, int level);

    // Get the physical link associated to the virtual link
    Link getPhysicalLink(VirtualLink* virtualLink, Request* req);

    // Get the appropriate physical resource, on which the virtual resource us assigned
    Element * getAssigned(Element * virtualResource, Request* req);

public:
    // Remove the assignment of the request specified.
    // It is expected that removing of virtualLinks is not necessary
    // because it virtual links are assigned on the last step
    void removeAssignment(Request * req);

    // Get all replications
    Assignment::Replications& getReplicationsOfAssignment(Assignment* assignment)
    {
        return replicationsOfAssignment[assignment];
    }

private:
    // Virtual machines and storages assignments to know
    // the assignments of virtual links vertexes.
    RequestAssignment* virtualMachinesAssignments;
    RequestAssignment* storagesAssignments;

    // Replications.
    Assignment::Replications replications;

    // Replication-storage map
    std::map<Element* , Replication*> replicationOfStorage;

    // Assignment-replication map (to remove replications if
    // assignment failed).
    std::map<Assignment* , Assignment::Replications > replicationsOfAssignment;
    /*
private:
    // Methods-substeps in the limited exhaustive search algorithm

    // Get the map of all assigned virtual links with capacity less then element's one
    // and, additionally, all vm's assignments.
    void getAvailableNodeAssignments(Element* element, std::map<Node*, std::vector<Node*> >& nodesAssignments, std::map<Node*, Assignment* >& vmAssignment, Assignment* assignment);
    */
};

#endif
