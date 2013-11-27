#ifndef NODE_H
#define NODE_H

#include "computationalElement.h"

class Node : public ComputationalElement
{
public:
    Node(string name = "unnamed_node", unsigned long capacity = 0, unsigned long max = 0);
public:
    void setRamCapacity(unsigned long c) { ramCapacity = c; }
    void setMaxRamCapacity(unsigned long mc) { maxRamCapacity = mc; }
    unsigned long getRamCapacity() const { return ramCapacity; }
    unsigned long getMaxRamCapacity() const { return maxRamCapacity; }
    virtual bool isAssignmentPossible(Element const & other) const;
    virtual void assign(Element const & other);
    virtual void RemoveAssignment(Element const * other);
private:
    unsigned long ramCapacity;
    unsigned long maxRamCapacity;

};

#endif // NODE_H
