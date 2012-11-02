#ifndef NODE_H
#define NODE_H

#include "computationalElement.h"

class Node : public ComputationalElement
{
public:
    Node(string name = "unnamed_node", unsigned long capacity = 0, unsigned long max = 0)
        : ComputationalElement(name, capacity, max)
    {
        setType(Element::NODE);
    }

};

#endif // NODE_H
