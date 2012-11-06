#ifndef CRITERIA_H_
#define CRITERIA_H_

#include "request.h"

// Class Criteria, which keeps criterias of greedy procedures of the algorithm.
// No object of these class can be created, all methods are static.
class Criteria
{
private:
    // Class with just static criterias represented,
    // with different implementations of criterias proposed.
    Criteria() {}
    ~Criteria() {}

public:
    // Static function, represent criterias of greedy procedures of the algorithm.

    // The weight of the request virtual machines set to be assigned first.
    static long requestVirtualMachinesWeight(Request::VirtualMachines * virtualMachines);

    // The weight of the request storages set to be assigned first.
    static long requestStoragesWeight(Request::Storages * storages);

    // The weight of one virtual machine to be assigned first.
    static long virtualMachineWeight(Node * virtualMachine);

    // The weight of one storage to be assigned first.
    static long storageWeight(Store * storage);

    // The depth of the limited exhaustive search procedure.
    static unsigned exhaustiveSearchDepth();
};

#endif