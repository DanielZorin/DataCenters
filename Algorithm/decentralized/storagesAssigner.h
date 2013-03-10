#ifndef STORAGES_ASSIGNER_H_
#define STORAGES_ASSIGNER_H_

#include "store.h"
#include "elementsAssigner.h"
#include "request.h"

class StoragesAssigner : public ElementsAssigner
{
public:
    typedef std::map<Request::Storages * , Request * > RequestsStorages;
public:
    StoragesAssigner(Network* network)
    :
        ElementsAssigner(network)
    {
    }

    virtual ~StoragesAssigner();

public:
    // Perform the assignment of storages to memory stores.
    // Returns the requests, for which assignments are succeded, and
    // forms the assigmnets variable, which may be get by the appropriate getter.
    //
    Requests PerformAssignment(Requests& requests);

private:
    // Assign one request's storage.
    // Used in the process of assigning of all storages sets.
    // Return true if assignment succeded.
    //
    bool assignOneRequest(Request::Storages * virtualMachines, Assignment* reqAssignment);

    // Assign one storage.
    // Used in the process of assigning of one request's storages set.
    // Return true if assignment succeded.
    //
    bool assignOneStorage(Store * storage, Assignment* reqAssignment);

protected:
    // Limited exhaustive search.
    //
    bool limitedExhaustiveSearch(Element * element, Assignment* assignment, Request* req);

private:
    // Methods-substeps in the limited exhaustive search algorithm

    // Get the map of all assigned storages of stores with capacity less then element's one
    // and, additionally, all storage's assignments.
    void getAvailableStoreAssignments(Element* element, std::map<Store*, std::vector<Store*> >& storesAssignments, std::map<Store*, Assignment* >& stAssignment, Assignment* assignment);

    bool recursiveExhaustiveSearch(Element * element, Assignment* assignment,
                                    std::map<Store*, std::vector<Store *> >& STsOnStore,
                                    std::map<Store*, Assignment* >& stAssignment,
                                    std::map<Store*, std::vector<Store *> >::iterator curIt,
                                    std::vector<Storage*>& stsSetToAssign,
                                    unsigned depth);

    // Try to reassign the set of elements
    bool tryToAssign(Element * element, Assignment* assignment,
                      std::map<Store*, Assignment* >& stAssignment,
                      std::vector<Storage*>& stsSetToAssign, Store* storeToAssign);

private:
    // Useful variables used during the algorithm

    // Stores with already assigned storages of currently parsed request.
    std::vector<Store* > requestsAssignedStores;
};

#endif