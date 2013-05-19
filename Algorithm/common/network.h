#ifndef NETWORK_H
#define NETWORK_H

#include "publicdefs.h"

// Class representing a physical resources model of the data center
class Network
{
public:
   // constructor
   Network();
   // copy constructor
   Network(const Network & n);
   // destructor
   ~Network();

   // operator=
   Network& operator=(const Network & n);

   // quick assign
   // assigns only the capacity and typeOfStore values from the given network to the existing network
   // doesn't cause reallocation, doesn't assign resource names
   // if there are different number of resources in the networks, only first possible capacities will be assigned
   Network & assign(const Network & n);

   // Getters/Setters
   const Nodes& getNodes() const;
   const Stores& getStores() const;
   const Switches& getSwitches() const;
   const Links& getLinks() const;
   Nodes& getNodes();
   Stores& getStores();
   Switches& getSwitches();
   Links& getLinks();

   Node* addNode(Node * node);
   Store* addStore(Store * store);
   Switch* addSwitch(Switch* sw);
   Link* addLink(Link * link);
protected:
   // look for element with the given ID and return it's pointer
   Node * nodesIDLookup(const long num);
   Store * storesIDLookup(const long num);
   Switch * switchesIDLookup(const long num);
   Link * linksIDLookup(const long num);
private:
   // members
   Nodes nodes;
   Stores stores;
   Switches switches;
   Links links;
};

#endif // NETWORK_H
