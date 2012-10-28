#include "network.h"
#include "node.h"
#include "store.h"
#include "switch.h"
#include "link.h"

Network::Network()
{
}

Network::Network(const Network & n)
{
    for (Nodes::iterator i = n.nodes.begin(); i != n.nodes.end(); i ++)
        nodes.insert(new Node((*i)->getName(), (*i)->getCapacity()));

    for (Stores::iterator i = n.stores.begin(); i != n.stores.end(); i ++)
        stores.insert(new Store((*i)->getName(), (*i)->getCapacity()));

    for (Switches::iterator i = n.switches.begin(); i != n.switches.end(); i ++)
        switches.insert(new Switch((*i)->getName(), (*i)->getCapacity()));

    for (Links::iterator i = n.links.begin(); i != n.links.end(); i ++)
        links.insert(new Link((*i)->getName(), (*i)->getCapacity()));
}

Network::~Network()
{
    for (Nodes::iterator i = nodes.begin(); i != nodes.end(); i++)
        delete *i;

    for (Switches::iterator i = switches.begin(); i != switches.end(); i++)
        delete *i;

    for (Stores::iterator i = stores.begin(); i != stores.end(); i++)
        delete *i;

    for (Links::iterator i = links.begin(); i != links.end(); i++)
        delete *i;
}

Network& Network::operator=(const Network & n)
{
    if (&n == this) return *this;
    // clean
    for (Nodes::iterator i = nodes.begin(); i != nodes.end(); i++)
        delete *i;

    for (Switches::iterator i = switches.begin(); i != switches.end(); i++)
        delete *i;

    for (Stores::iterator i = stores.begin(); i != stores.end(); i++)
        delete *i;

    for (Links::iterator i = links.begin(); i != links.end(); i++)
        delete *i;

    nodes.clear();
    stores.clear();
    switches.clear();
    links.clear();

    // add new items
    for (Nodes::iterator i = n.nodes.begin(); i != n.nodes.end(); i ++)
        nodes.insert(new Node((*i)->getName(), (*i)->getCapacity()));

    for (Stores::iterator i = n.stores.begin(); i != n.stores.end(); i ++)
        stores.insert(new Store((*i)->getName(), (*i)->getCapacity()));

    for (Switches::iterator i = n.switches.begin(); i != n.switches.end(); i ++)
        switches.insert(new Switch((*i)->getName(), (*i)->getCapacity()));

    for (Links::iterator i = n.links.begin(); i != n.links.end(); i ++)
        links.insert(new Link((*i)->getName(), (*i)->getCapacity()));

    return *this;
}

Node* Network::addNode(Node * node)
{
    nodes.insert(node);
    return node;
}

Store* Network::addStore(Store * store)
{
    stores.insert(store);
    return store;
}

Switch* Network::addSwitch(Switch* sw)
{
    switches.insert(sw);
    return sw;
}

Link* Network::addLink(Link * link)
{
    links.insert(link);
    return link;
}

Nodes& Network::getNodes()
{
    return nodes;
}

Stores& Network::getStores()
{
    return stores;
}

Switches& Network::getSwitches()
{
    return switches;
}

Links& Network::getLinks()
{
    return links;
}

const Nodes& Network::getNodes() const
{
    return nodes;
}

const Stores& Network::getStores() const
{
    return stores;
}

const Switches& Network::getSwitches() const
{
    return switches;
}

const Links& Network::getLinks() const
{
    return links;
}
