#include "algorithm.h"

#include <iostream>
using std::cerr;
using std::endl;

Algorithm::Algorithm(Network * n, Requests const & r)
:
    network(n),
    requests(r)
{
    cerr << "Constructed algorithm environment to schedule " 
        << requests.size() << " requests" << endl;
}

Algorithm::~Algorithm()
{
    cerr << "Assigned " << assignments.size()
        << " of " << requests.size() << " requests" << endl;
}
