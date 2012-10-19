#ifndef DECENTRALIZED_ALGORITHM_H
#define DECENTRALIZED_ALGORITHM_H

#include "algorithm.h"
#include "request.h"

// The algorithm of scheduling in data-center with different
// resource pools schedulers. Three main stages are performed:
//  - assigning of all possible computational nodes
//  - assigning of all possible memory stores (from requests with 
//     computational nodes already assigned)
//  - assigning of all virtrual links (from requests with
//     computational nodes and memory stores already assigned).
// For details, see the algorithm specification.

class ComputationalElement;
class NetworkingElement;

class DecentralizedAlgorithm: public Algorithm
{
public:
    typedef std::set<ComputationalElement *> RequestComputationalElements;
    typedef std::set<RequestComputationalElements * > ComputationalElements;

    typedef std::set<ComputationalElement *> RequestNetworkingElements;
    typedef std::set<RequestNetworkingElements * > NetworkingElements;
public:

    // constructor
    DecentralizedAlgorithm(Network* network, Requests const& requests)
    :
        Algorithm(network, requests)
    {
    }

    // Schedule requests:
    virtual ResultEnum::Result schedule();

private:
    // internal methods, performing the steps of the algorithm

    // Perform the resource packing.
    // Return the set of requests packed.
    Requests assignComputationalElements(ComputationalElements& computationalElements);

    // Perform the mapping of virtual links.
    // The assognments are formed after execution of these method.
    ResultEnum::Result assignVirtualLinks(NetworkingElements& networkingElements);
};

#endif
