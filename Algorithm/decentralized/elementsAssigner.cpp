#include "elementsAssigner.h"
#include "assignment.h"

ElementsAssigner::~ElementsAssigner()
{
    RequestAssignment::iterator it = requestAssignment.begin();
    RequestAssignment::iterator itEnd = requestAssignment.end();
    for ( ; it != itEnd; ++it )
        delete it->second;
}