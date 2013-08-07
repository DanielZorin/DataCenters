#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "publicdefs.h"

// base class for heuristic calculation objects
class Heuristic
{
public:
    virtual double calculate(unsigned int param1 = 0, unsigned int param2 = 0, unsigned int param3 = 0) = 0;
    inline double getMax() const { return maxValue; }

    Heuristic(double max = 0)
    : maxValue(max)
    {
    }
    ~Heuristic() {}
protected:
    // maximum possible value for this heuristic
    double maxValue;
};

// the more the physical resources remains after the element is assigned, the more the value
// heuristic is used when choosing a resource for a request element
// param1 = current physical resources
// param2 = required resources
// param3 = maximum physical resources
// maxValue = maximum heuristic value ever calculated
class MoreResourceFirst: public Heuristic
{
public:
    virtual double calculate(unsigned int param1 = 0, unsigned int param2 = 0, unsigned int param3 = 0);

    MoreResourceFirst(double max = 0)
    : Heuristic(max)
    {
    }
    ~MoreResourceFirst() {}
};

// the more the requested capacity is, the more the value. The value is normalized.
// heuristic is used when choosing the next request element
// param1 = current element requested capacity
// param2 = maximum requested capacity from all the elements
// param3 not used
// maxValue not used
class LargeRequestFirst: public Heuristic
{
public:
    virtual double calculate(unsigned int param1 = 0, unsigned int param2 = 0, unsigned int param3 = 0);

    LargeRequestFirst(double max = 0)
    : Heuristic(max)
    {
    }
    ~LargeRequestFirst() {}
};

#endif