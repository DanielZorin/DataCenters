#ifndef CRITERIA_H_
#define CRITERIA_H_

#include "request.h"

// Class Criteria, which keeps criterias of greedy procedures of the algorithm.
// No object of these class can be created, all methods are static.
class Criteria
{
public:
    // Mode of performing bin packing.
    // There are two modes: BFD - best fit decreasing, and CriticalNetwork.
    //    BFD - standart algorithm is used, this mode is chosen when network is not critical resource.
    //    CriticalNetwork is chosen when network is critical resource, so vms and storages are packed
    //      appropriately.
    //
    enum PackMode
    {
        BFD = 0,
        NETWORK_CRITICAL
    };

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

    // The weight of the request virtual links set to be assigned first.
    static long requestVirtualLinksWeight(Request::VirtualLinks * virtualLinks);

    // The weight of one virtual machine to be assigned first.
    static long virtualMachineWeight(Node * virtualMachine);

    // The weight of one storage to be assigned first.
    static long storageWeight(Store * storage);

    // The weight of one virtual link to be assigned first.
    static long virtualLinkWeight(Link * storage);

    // The depth of the limited exhaustive search procedure.
    static unsigned exhaustiveSearchDepthNetwork();

    // The depth of the limited exhaustive search procedure.
    static unsigned exhaustiveSearchDepthComputational();

    // The depth of the k-shortest-paths algorithm.
    static unsigned kShortestPathDepth();

    // Cost of some specified path.
    static long pathCost(NetPath& path);

    // The cost of the path between some virtual machine (assigned on node) and
    // memory store to be used in the replication procedure
    static long replicationPathCost(Store* initialStore, Store* store, Network * network, NetPath& path,
                                    unsigned replicationCapacity);
    static long replicationPathCost(VirtualLink* virtualLink, Network * network, NetPath& path);

    // Automatically identify pack mode according to input parameters
    static void identifyPackMode(Requests* requests, Network* network);

    // Get pack mode.
    static PackMode getPackMode()
    {
        return packMode;
    }

private:
    // Chosen mode of perfmorming bin packing.
    //
    static PackMode packMode;
};

#endif