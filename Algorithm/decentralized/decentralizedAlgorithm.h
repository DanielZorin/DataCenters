#ifndef DECENTRALIZED_ALGORITHM_H
#define DECENTRALIZED_ALGORITHM_H

#include "common/algorithm.h"
#include "common/request.h"

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
class VirtualLinksAssigner;

class DecentralizedAlgorithm: public Algorithm
{
public:

    // constructor
    DecentralizedAlgorithm(Network* network, Requests const& requests)
    :
        Algorithm(network, requests)
    {
    }

    ~DecentralizedAlgorithm();

    // Schedule requests:
    virtual Result schedule();

private:
    // Restore the network by removing the assignments of
    // requests, which were accepted on some step and rejected on the next step.
    void restoreNetwork(Requests& initialRequests, Requests& assignedRequests,
        VirtualLinksAssigner& virtualLinksAssigner);
};

#endif
