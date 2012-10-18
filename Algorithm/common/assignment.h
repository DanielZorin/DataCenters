#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include <map>

class Node;
class Store;
class Switch;
class Link;

class Assignment
{
public:
   typedef std::map<Node *, Node *> NodeAssignment;
   typedef std::map<Store *, Store *> StoreAssignment;
private:
   NodeAssignment nodeAssignment;
   StoreAssignment storeAssignment;

};

#endif // ASSIGNMENT_H
