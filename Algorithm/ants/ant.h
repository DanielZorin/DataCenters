#ifndef ANT_H
#define ANT_H

#include "../common/node.h"
#include "../common/store.h"
#include "../common/link.h"
#include "../common/switch.h"
#include "../common/network.h"
#include "../common/request.h"
#include "../common/assignment.h"
#include "../common/algorithm.h"
#include "internalgraph.h"


// Ant algorithm control class
// Constructs solution using Network and a set of requests
class AntAlgorithm: public Algorithm
{
public:
    AntAlgorithm(Network * n, Requests const & r)
    : Algorithm(n, r)
    {}
    virtual Algorithm::ResultEnum::Result schedule() { return Algorithm::ResultEnum::Result::SUCCESS; }

    // start function
    void start();
private:
    InternalGraph * graph;

    bool buildPath();
    bool buildLink();

    // No default constructor, copy constructor and operator=
    AntAlgorithm();
    AntAlgorithm(const AntAlgorithm&);
    AntAlgorithm& operator=(const AntAlgorithm&);
};

#endif
