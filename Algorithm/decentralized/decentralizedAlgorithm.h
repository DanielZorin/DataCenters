#ifndef DECENTRALIZED_ALGORITHM_H
#define DECENTRALIZED_ALGORITHM_H

#include "algorithm.h"
#include "request.h"
#include <map>

// The algorithm of scheduling in data-center with different
// resource pools schedulers. Three main stages are performed:
//  - assigning of all possible computational nodes
//  - assigning of all possible memory stores (from requests with 
//     computational nodes already assigned)
//  - assigning of all virtrual links (from requests with
//     computational nodes and memory stores already assigned).
// For details, see the algorithm specification.

class Node;
class Store;
class Link;
class ComputationalElement;

class DecentralizedAlgorithm: public Algorithm
{
public:
    typedef std::set<Request::VirtualMachines * > RequestsVirtualMachines;
    typedef std::set<Request::Storages * > RequestsStorages;
    typedef std::set<Request::VirtualLinks * > RequestsVirtualLinks;
public:

    // constructor
    DecentralizedAlgorithm(Network* network, Requests const& requests)
    :
        Algorithm(network, requests)
    {
    }

    ~DecentralizedAlgorithm();

    // Schedule requests:
    virtual ResultEnum::Result schedule();

private:
    // internal methods, performing the steps of the algorithm

    // Perform the resource packing.
    // Return the set of requests packed.
    Requests assignComputationalElements(RequestsVirtualMachines& requestsVirtualMachines);
    Requests assignComputationalElements(RequestsStorages& requestsStorages);

    // Perform the mapping of virtual links.
    // The assognments are formed after execution of these method.
    ResultEnum::Result assignVirtualLinks(RequestsVirtualLinks& requestsVirtualLinks);

    // Assign one request resources: virtual machines or storages.
    // Used in the process of assigning computational elements.
    // Return true if assignment succeded.
    bool assignOneRequestResourses(Request::VirtualMachines * virtualMachines);
    bool assignOneRequestResourses(Request::Storages * storages);

    // Assign one computational element.
    // Used in the process of assigning computational elements.
    // Return true if assignment succeded.
    bool assignOneComputationalElement(ComputationalElement* element, Assignment& localAssignment);

public:
    // methods, that identifies different criterias to be used in the algorithm

    // The weight of the request virtual machines set to be assigned first.
    static long requestVirtualMachinesWeight(Request::VirtualMachines* virtualMachines);

    // The weight of one virtual machine to be assigned first.
    static long virtualMachineWeight(Node * virtualMachine);
};

#endif
