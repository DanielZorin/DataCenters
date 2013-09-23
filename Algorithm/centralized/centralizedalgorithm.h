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
    CentralizedAlgorithm(Network * n, Requests const & r);
private:
    template <class T> std::vector<T*> prioritize(std::set<T*> &);

    Result buildVMAssignment(Request *);
    Result buildStorageAssignment(Request *);
    Result tryToAssignVM(Node *, Node *);
    Result tryToAssignStorage(Store *, Store *);
    Result tryToAssignPathes(Element * assignee, Element * target, Links & links);

    Nodes getAssignedLinkedNodes(Element * node, Request * request);
    Links getConnectedVirtualLinks(Element * node, Request * request);
public:
    virtual Result schedule();
private:
    Assignment * currentAssignment;

    NodeManager nodeManager;
    StoreManager storeManager;
    NetworkManager networkManager;
};

#endif // CENTRALIZEDALGORITHM_H
