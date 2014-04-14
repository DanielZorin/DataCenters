#ifndef RANDOMALG_H
#define RANDOMALG_H

#include "common/algorithm.h"
#include "common/element.h"
#include "common/network.h"
#include "common/request.h"
#include "common/publicdefs.h"
#include "common/node.h"
#include "common/store.h"
#include "common/assignment.h"
#include "common/link.h"
#include <vector>
#include <time.h>

// utility structs

struct SequenceElement
{
    Element * request;
    Element * resource;

    SequenceElement(Element * req, Element * res)
    : request(req)
    , resource(res)
    {
    }

    SequenceElement()
    : request(NULL)
    , resource(NULL)
    {
    }
};

struct CritValue
{
    double value;
    Element * resource;

    CritValue(double v, Element * r)
    : value(v)
    , resource(r)
    {
    }

    CritValue()
    : value(0)
    , resource(NULL)
    {
    }
};

// algorithm that assign requests into a node randomly chosen from a set of fitting nodes
// netPath for a link is chosen randomly from a few first paths found by k shortest paths algorithm
class RandomAlgorithm: public Algorithm
{
    public:
    RandomAlgorithm(Network * n, Requests const & r, unsigned long tr = 1, unsigned int N = 0) : Algorithm(n, r), copyNetwork(NULL), tries(tr), NRes(N)
	{
		vmCount = 0, stCount = 0;
		copyNetwork = new Network();
		*copyNetwork = *network;
		for (Requests::iterator i = requests.begin(); i != requests.end(); i ++)
		{
			vmCount += (*i)->getVirtualMachines().size();
			stCount += (*i)->getStorages().size();
		}
		if (NRes == 0) NRes = (network->getStores().size() > network->getNodes().size()) ? network->getStores().size() : network->getNodes().size();

//		srand(time(NULL));
	//    srand(1);

		bestSequence.reserve(vmCount+stCount);
		bestReq.reserve(requests.size());
	}
    ~RandomAlgorithm();

    virtual Algorithm::Result schedule();

    private:
    Element * findResource(Element * request, std::vector<SequenceElement *> seq, unsigned int start);
    // copy network to restore it between iterations
    Network * copyNetwork;
    // number of tries
    unsigned long tries;
    // current best sequence of elements, a list of assigned requests and a map with assigned channels
    std::vector<SequenceElement *> bestSequence;
    std::vector<Request *> bestReq;
    std::map<Link *, NetPath> bestChan;
    // amount of vms and storages
    unsigned int vmCount, stCount;
    // amount of fitting physical resources to consider when choosing the place for the request
    unsigned int NRes;
};

#endif
