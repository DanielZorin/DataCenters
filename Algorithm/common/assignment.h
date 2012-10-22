#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include <map>
#include <vector>

class Node;
class Store;
class Switch;
class Link;

class Assignment
{
public:
    typedef std::map<Node *, Node *> NodeAssignment;
    typedef std::map<Store *, Store *> StoreAssignment;
public:
    Node * GetAssignment(Node *);
    std::vector<Node *> GetAssigned(Node *);
    Store * GetAssignment(Store *);
    std::vector<Store *> GetAssigned(Store *);

private:
    NodeAssignment nodeAssignment;
    StoreAssignment storeAssignment;

};

#endif // ASSIGNMENT_H
