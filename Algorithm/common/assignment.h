#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include <map>
#include <vector>
#include <set>

#include <string>

class Node;
class Store;
class Switch;
class Link;
class NetworkingElement;

using std::string;

class Assignment
{
public:
    typedef std::map<Node *, Node *> NodeAssignment;
    typedef std::set<Node *> Nodes;
    typedef std::map<Store *, Store *> StoreAssignment;
    typedef std::set<Store *> Stores;
    typedef std::vector<NetworkingElement *> NetPath;
    typedef std::set<Link *> Links;
    typedef std::map<Link *, NetPath> LinkAssignment;
public:
    Node * GetAssignment(Node *);
    Nodes GetAssigned(Node *);
    Store * GetAssignment(Store *);
    Stores GetAssigned(Store *);
    NetPath GetAssignment(Link *);
    Links GetAssigned(NetworkingElement *);
    
    void AddAssignment(Node * w, Node * p)
    {
        nodeAssignment.insert(std::pair<Node *, Node *>(w, p));
    }

    void AddAssignment(Store * s, Store * m)
    {
        storeAssignment.insert(std::pair<Store *, Store *>(s, m));
    }

    void AddAssignment(Link * e, NetPath & path)
    {
        linkAssignment.insert(std::pair<Link *, NetPath>(e, path));
    }

private:
    NodeAssignment nodeAssignment;
    StoreAssignment storeAssignment;
    LinkAssignment linkAssignment;

};

#endif // ASSIGNMENT_H
