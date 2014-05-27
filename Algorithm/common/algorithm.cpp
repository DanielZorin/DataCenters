#include "algorithm.h"
#include "assignment.h"

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

    Assignments::iterator it = assignments.begin();
    Assignments::iterator itEnd = assignments.end();
    for ( ; it != itEnd; ++it )
        delete (*it);
    assignments.clear();
}
