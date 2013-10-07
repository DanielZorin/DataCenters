#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "publicdefs.h"
#include "algorithm.h"
#include "depthSearcher.h"

#include <set>

using std::set;

class Assignment;
class Element;

class NetworkManager
{
public:
    NetworkManager(Network & network);

    void setSearchSpace(const Nodes & nodes);
    Nodes getNodeCandidates();
    Stores getStoreCandidates();

    void cleanUpLinks(Links & links, Assignment * assignment);
    Algorithm::Result buildPath(Element * from, Element * to, Link * vLink, Assignment * assignment);
private:
    void removeAssignment(Link * vlink, NetPath & path);
    void addAssignment(Link * vlink, NetPath & path);
private:
    Network & network;
    DepthSearcher * depthSearcher;

    Nodes rejectedNodes; 
    Stores rejectedStores;
};

#endif // NETWORKMANAGER_H
