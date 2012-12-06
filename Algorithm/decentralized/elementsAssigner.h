#ifndef ELEMENTS_ASSIGNMER_H_
#define ELEMENTS_ASSIGNMER_H_

#include "publicdefs.h"
#include<map>

// to add NULL constant independently
#include <cstddef>

// The class to assign any of virtual elements
// to physical resources.

class ElementsAssigner
{
public:
    // Map between request and it's assignment.
    // If request is not assigned, it is not present.
    //
    typedef std::map<Request *, Assignment *> RequestAssignment;
public:
    ElementsAssigner(Network* network)
    {
        this->network = network;
    }

    virtual ~ElementsAssigner();

public:
    // Perform the assignment.
    // Returns the requests, for which assignments are succeded, and
    // forms the assigmnets map, which may be get by the appropriate getter.
    //
    virtual Requests PerformAssignment(Requests& requests) = 0;

    // Get assignments after performing the assignment process
    //
    Assignment* GetRequestAssignment(Request* req)
    {
        if ( requestAssignment.find(req) != requestAssignment.end() )
            return requestAssignment[req];

        return NULL;
    }

    RequestAssignment * GetRequestAssignment()
    {
        return &requestAssignment;
    }

protected:
    // Limited exhaustive search procedure for any element
    // (virtual machine, storage or virtual link), which can't be assigned.
    // Returns true if succeded.
    //
    virtual bool limitedExhaustiveSearch(Element * element, Assignment* assignment, Request* req) = 0;

protected:

    // Resource Network.
    //
    Network* network;

    // Assignments of request elements.
    //
    RequestAssignment requestAssignment;
};

#endif
