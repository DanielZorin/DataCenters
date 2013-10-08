#ifndef CENTRALIZEDALGORITHM_H
#define CENTRALIZEDALGORITHM_H

#include "publicdefs.h"
#include "algorithm.h"
#include "request.h"

#include "nodeManager.h"
#include "storeManager.h"
#include "networkManager.h"

#include <vector>

class CentralizedAlgorithm : public Algorithm
{
private:
    CentralizedAlgorithm();
public:
    enum Version
    {
        NEUTRAL_PACK,
        NET_PACK
    };
    CentralizedAlgorithm(Network * n, Requests const & r, Version v = NET_PACK);
private:
    template <class T> std::vector<T*> prioritize(std::set<T*> &);

    Assignment * assignRequestNet(Request * request);
    Assignment * assignRequestPlain(Request * request);
    void cleanUpAssignment(Assignment * assignment);

    Result assignVM(Node * vm, Request * request);
    Result assignStorage(Store * storage, Request * request);

    Result buildVMAssignment(Request *);
    Result buildStorageAssignment(Request *);
    Result tryToAssignVM(Node *, Node *);
    Result tryToAssignStorage(Store *, Store *);
    Result tryToAssignPathes(Element * assignee, Element * target, Links & links);

    Nodes getAssignedLinkedNodes(Element * node, Request * request);
    Links getConnectedVirtualLinks(Element * node, Request * request);
    Elements getChanneledVirtualResources(Element * element, Request * request);
public:
    virtual Result schedule();
private:
    Assignment * currentAssignment;
    Version version;

    NodeManager nodeManager;
    StoreManager storeManager;
    NetworkManager networkManager;
};

#endif // CENTRALIZEDALGORITHM_H
