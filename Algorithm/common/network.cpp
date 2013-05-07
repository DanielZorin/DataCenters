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
    for (Nodes::const_iterator i = n.nodes.begin(); i != n.nodes.end(); i ++)
    {
        Node * tmpNode = new Node((*i)->getName(), (*i)->getCapacity(), (*i)->getMaxCapacity());
        tmpNode->setRamCapacity((*i)->getRamCapacity());
        tmpNode->setMaxRamCapacity((*i)->getMaxRamCapacity());
        nodes.insert(tmpNode);
    }

    for (Stores::const_iterator i = n.stores.begin(); i != n.stores.end(); i ++)
        stores.insert(new Store((*i)->getName(), (*i)->getCapacity(), (*i)->getMaxCapacity(), (*i)->getTypeOfStore()));

    for (Switches::const_iterator i = n.switches.begin(); i != n.switches.end(); i ++)
        switches.insert(new Switch((*i)->getName(), (*i)->getCapacity(), (*i)->getMaxCapacity()));

    for (Links::const_iterator i = n.links.begin(); i != n.links.end(); i ++)
    {
        Link * tmp = new Link((*i)->getName(), (*i)->getCapacity(), (*i)->getMaxCapacity());
        tmp->bindElements((*i)->getFirst(), (*i)->getSecond()); // FIXME bind new elements, not old ones
        links.insert(tmp);
    }
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
    for (Nodes::const_iterator i = n.nodes.begin(); i != n.nodes.end(); i ++)
    {
        Node * tmpNode = new Node((*i)->getName(), (*i)->getCapacity(), (*i)->getMaxCapacity());
        tmpNode->setRamCapacity((*i)->getRamCapacity());
        tmpNode->setMaxRamCapacity((*i)->getMaxRamCapacity());
        nodes.insert(tmpNode);
    }

    for (Stores::const_iterator i = n.stores.begin(); i != n.stores.end(); i ++)
        stores.insert(new Store((*i)->getName(), (*i)->getCapacity(), (*i)->getMaxCapacity(), (*i)->getTypeOfStore()));

    for (Switches::const_iterator i = n.switches.begin(); i != n.switches.end(); i ++)
        switches.insert(new Switch((*i)->getName(), (*i)->getCapacity(), (*i)->getMaxCapacity()));

    for (Links::const_iterator i = n.links.begin(); i != n.links.end(); i ++)
    {
        Link * tmp = new Link((*i)->getName(), (*i)->getCapacity(), (*i)->getMaxCapacity());
        tmp->bindElements((*i)->getFirst(), (*i)->getSecond()); // FIXME bind new elements, not old ones
        links.insert(tmp);
    }

    return *this;
}

Network & Network::assign (const Network & n)
{
    if (&n == this) return *this;
    Nodes::const_iterator i1 = n.nodes.begin();
    Nodes::const_iterator i2 = nodes.begin();
    for (; i1 != n.nodes.end() && i2 != nodes.end(); i1 ++, i2 ++)
    {
        (*i2)->setCapacity((*i1)->getCapacity());
        (*i2)->setMaxCapacity((*i1)->getMaxCapacity());
        (*i2)->setRamCapacity((*i1)->getRamCapacity());
        (*i2)->setMaxRamCapacity((*i1)->getMaxRamCapacity());
    }

    Stores::const_iterator j1 = n.stores.begin();
    Stores::const_iterator j2 = stores.begin();
    for (; j1 != n.stores.end() && j2 != stores.end(); j1 ++, j2 ++)
    {
        (*j2)->setCapacity((*j1)->getCapacity());
        (*j2)->setMaxCapacity((*j1)->getMaxCapacity());
        (*j2)->setTypeOfStore((*j1)->getTypeOfStore());
    }

    Switches::const_iterator k1 = n.switches.begin();
    Switches::const_iterator k2 = switches.begin();
    for (; k1 != n.switches.end() && k2 != switches.end(); k1 ++, k2 ++)
    {
        (*k2)->setCapacity((*k1)->getCapacity());
        (*k2)->setMaxCapacity((*k1)->getMaxCapacity());
    }

    Links::const_iterator p1 = n.links.begin();
    Links::const_iterator p2 = links.begin();
    for (; p1 != n.links.end() && p2 != links.end(); p1 ++, p2 ++)
    {
        (*p2)->setCapacity((*p1)->getCapacity());
        (*p2)->setMaxCapacity((*p1)->getMaxCapacity());
    }
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
