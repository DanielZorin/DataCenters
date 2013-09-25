#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "publicdefs.h"
#include "algorithm.h"

#include <set>

using std::set;

class Assignment;
class Element;

class NetworkManager
{
public:
    typedef set<set<Element *> > AdjacentElements;
    NetworkManager(Network & network);

    void setSearchSpace(const Nodes & nodes);
    Nodes getNodeCandidates();
    Stores getStoreCandidates();

    void cleanUpLinks(Links & links, Assignment * assignment);
    Algorithm::Result buildPath(Element * from, Element * to, Link * vLink, Assignment * assignment);
private:
    Elements intersect(const Elements & first, const Elements & second);
    Elements getElementCandidates();
    Elements getAdjacentElements(Element * element);
    void removeAssignment(Link * vlink, NetPath & path);
    void addAssignment(Link * vlink, NetPath & path);
    void increaseSearchSpace();
private:
    Network & network;
    AdjacentElements adjacentElements;
    Nodes rejectedNodes; 
    Stores rejectedStores;
};

#endif // NETWORKMANAGER_H
