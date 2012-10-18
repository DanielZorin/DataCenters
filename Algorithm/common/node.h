#ifndef NODE_H
#define NODE_H

#include "computationalElement.h"

class Node : public ComputationalElement
{
public:
   Node(string name = "unnamed_node", unsigned long capacity = 0)
   : ComputationalElement(name, capacity)
   {}

};

#endif // NODE_H
