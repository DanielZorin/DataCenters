#ifndef NETWORK_H
#define NETWORK_H

#include <set>

class Node;
class Store;
class Switch;
class Link;

class Network
{
public:
   typedef std::set<Node *> Nodes;
   typedef std::set<Store *> Stores;
   typedef std::set<Switch *> Switches;
   typedef std::set<Link *> Links;

public:
   Network() {}
   ~Network();
private:
   Nodes nodes;
   Stores stores;
   Switches switches;
   Links links;
};

#endif // NETWORK_H
