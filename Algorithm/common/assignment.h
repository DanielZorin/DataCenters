#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include "publicdefs.h"

#include <string>
using std::string;

class Assignment
{
public:
    typedef std::map<Node *, Node *> NodeAssignments;
    typedef std::map<Store *, Store *> StoreAssignments;
    typedef std::map<Link *, NetPath> LinkAssignments;
public:
    Node * GetAssignment(Node *);
    Nodes GetAssigned(Node *);
    Store * GetAssignment(Store *);
    Stores GetAssigned(Store *);
    NetPath GetAssignment(Link *);
    Links GetAssigned(NetworkingElement *);
    
    Assignment()
    : request(NULL) {}
    Assignment(Request * r) { request = r; }
    string getName();

    void AddAssignment(Node * w, Node * p)
    {
        nodeAssignments.insert(NodeAssignment(w, p));
    }

    void AddAssignment(Store * s, Store * m)
    {
        storeAssignments.insert(StoreAssignment(s, m));
    }

    void AddAssignment(Link * e, NetPath & path)
    {
        linkAssignments.insert(LinkAssignment(e, path));
    }

private:
    NodeAssignments nodeAssignments;
    StoreAssignments storeAssignments;
    LinkAssignments linkAssignments;

    Request * request;
};

#endif // ASSIGNMENT_H
