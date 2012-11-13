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
    std::vector<Request *> prioritizeRequests();
    std::vector<Node *> prioritizeVms(Request::VirtualMachines &);

    std::vector<Node *> getVMAssignmentCandidates(Node *);

    Result buildVMAssignment(Request *);
    Result buildStorageAssignment(Request *);

public:
    virtual Algorithm::Result schedule();
private:
    Assignment * currentAssignment;

    NodeManager nodeManager;
    StoreManager storeManager;
    NetworkManager networkManager;
};

#endif // CENTRALIZEDALGORITHM_H
