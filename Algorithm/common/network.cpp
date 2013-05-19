#include "network.h"
#include "node.h"
#include "store.h"
#include "switch.h"
#include "link.h"
#include <assert.h>

Node * Network::nodesIDLookup(const long num)
{
    for (Nodes::iterator i = nodes.begin(); i != nodes.end(); i ++)
        if ((*i)->getID() == num) return *i;

    return NULL;
}

Store * Network::storesIDLookup(const long num)
{
    for (Stores::iterator i = stores.begin(); i != stores.end(); i ++)
        if ((*i)->getID() == num) return *i;

    return NULL;
}

Switch * Network::switchesIDLookup(const long num)
{
    for (Switches::iterator i = switches.begin(); i != switches.end(); i ++)
        if ((*i)->getID() == num) return *i;

    return NULL;
}

Link * Network::linksIDLookup(const long num)
{
    for (Links::iterator i = links.begin(); i != links.end(); i ++)
        if ((*i)->getID() == num) return *i;

    return NULL;
}

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
        tmpNode->setID((*i)->getID());
        nodes.insert(tmpNode);
    }

    for (Stores::const_iterator i = n.stores.begin(); i != n.stores.end(); i ++)
    {
        Store * tmpStore = new Store((*i)->getName(), (*i)->getCapacity(), (*i)->getMaxCapacity(), (*i)->getTypeOfStore());
        tmpStore->setID((*i)->getID());
        stores.insert(tmpStore);
    }

    for (Switches::const_iterator i = n.switches.begin(); i != n.switches.end(); i ++)
    {
        Switch * tmpSwitch = new Switch((*i)->getName(), (*i)->getCapacity(), (*i)->getMaxCapacity());
        tmpSwitch->setID((*i)->getID());
        switches.insert(tmpSwitch);
    }

    long firstID = 0, secondID = 0;
    Element * bindFirst = NULL, * bindSecond = NULL;
    for (Links::const_iterator i = n.links.begin(); i != n.links.end(); i ++)
    {
        Link * tmpLink = new Link((*i)->getName(), (*i)->getCapacity(), (*i)->getMaxCapacity());
        tmpLink->setID((*i)->getID());
        // now find new elements and link them
        firstID = (*i)->getFirst()->getID();
        secondID = (*i)->getSecond()->getID();
        bindFirst = NULL;
        bindSecond = NULL;
        // first
        if ((*i)->getFirst()->isNode()) bindFirst = nodesIDLookup(firstID);
        else if ((*i)->getFirst()->isStore()) bindFirst = storesIDLookup(firstID);
        else if ((*i)->getFirst()->isSwitch()) bindFirst = switchesIDLookup(firstID);
        else assert(false);
        assert(bindFirst);
        // second
        if ((*i)->getSecond()->isNode()) bindSecond = nodesIDLookup(secondID);
        else if ((*i)->getSecond()->isStore()) bindSecond = storesIDLookup(secondID);
        else if ((*i)->getSecond()->isSwitch()) bindSecond = switchesIDLookup(secondID);
        else assert(false);
        assert(bindSecond);
        // link and add
        tmpLink->bindElements(bindFirst, bindSecond);
        links.insert(tmpLink);
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
        tmpNode->setID((*i)->getID());
        assert(tmpNode->isNode());
        nodes.insert(tmpNode);
    }

    for (Stores::const_iterator i = n.stores.begin(); i != n.stores.end(); i ++)
    {
        Store * tmpStore = new Store((*i)->getName(), (*i)->getCapacity(), (*i)->getMaxCapacity(), (*i)->getTypeOfStore());
        tmpStore->setID((*i)->getID());
        assert(tmpStore->isStore());
        stores.insert(tmpStore);
    }

    for (Switches::const_iterator i = n.switches.begin(); i != n.switches.end(); i ++)
    {
        Switch * tmpSwitch = new Switch((*i)->getName(), (*i)->getCapacity(), (*i)->getMaxCapacity());
        tmpSwitch->setID((*i)->getID());
        assert(tmpSwitch->isSwitch());
        switches.insert(tmpSwitch);
    }

    long firstID = 0, secondID = 0;
    Element * bindFirst = NULL, * bindSecond = NULL;
    for (Links::const_iterator i = n.links.begin(); i != n.links.end(); i ++)
    {
        Link * tmpLink = new Link((*i)->getName(), (*i)->getCapacity(), (*i)->getMaxCapacity());
        tmpLink->setID((*i)->getID());
        assert(tmpLink->isLink());
        // now find new elements and link them
        firstID = (*i)->getFirst()->getID();
        secondID = (*i)->getSecond()->getID();
        bindFirst = NULL;
        bindSecond = NULL;
        // first
        if ((*i)->getFirst()->isNode()) bindFirst = nodesIDLookup(firstID);
        else if ((*i)->getFirst()->isStore()) bindFirst = storesIDLookup(firstID);
        else if ((*i)->getFirst()->isSwitch()) bindFirst = switchesIDLookup(firstID);
        else assert(false);
        assert(bindFirst);
        // second
        if ((*i)->getSecond()->isNode()) bindSecond = nodesIDLookup(secondID);
        else if ((*i)->getSecond()->isStore()) bindSecond = storesIDLookup(secondID);
        else if ((*i)->getSecond()->isSwitch()) bindSecond = switchesIDLookup(secondID);
        else assert(false);
        assert(bindSecond);
        // link and add
        tmpLink->bindElements(bindFirst, bindSecond);
        links.insert(tmpLink);
    }

    return *this;
}

Network & Network::assign (const Network & n)
{
    if (&n == this) return *this;
    for (Nodes::const_iterator i = n.nodes.begin(); i != n.nodes.end(); i ++)
    {
        Node * ptr = nodesIDLookup((*i)->getID());
        assert(ptr);
        ptr->setCapacity((*i)->getCapacity());
        ptr->setMaxCapacity((*i)->getMaxCapacity());
        ptr->setRamCapacity((*i)->getRamCapacity());
        ptr->setMaxRamCapacity((*i)->getMaxRamCapacity());
    }

    for (Stores::const_iterator i = n.stores.begin(); i != n.stores.end(); i ++)
    {
        Store * ptr = storesIDLookup((*i)->getID());
        assert(ptr);
        ptr->setCapacity((*i)->getCapacity());
        ptr->setMaxCapacity((*i)->getMaxCapacity());
        ptr->setTypeOfStore((*i)->getTypeOfStore());
    }

    for (Switches::const_iterator i = n.switches.begin(); i != n.switches.end(); i ++)
    {
        Switch * ptr = switchesIDLookup((*i)->getID());
        assert(ptr);
        ptr->setCapacity((*i)->getCapacity());
        ptr->setMaxCapacity((*i)->getMaxCapacity());
    }

    for (Links::const_iterator i = n.links.begin(); i != n.links.end(); i ++)
    {
        Link * ptr = linksIDLookup((*i)->getID());
        assert(ptr);
        ptr->setCapacity((*i)->getCapacity());
        ptr->setMaxCapacity((*i)->getMaxCapacity());
    }
    return *this;
}

Node* Network::addNode(Node * node)
{
    node->setID(nodes.size());
    nodes.insert(node);
    return node;
}

Store* Network::addStore(Store * store)
{
    store->setID(stores.size());
    stores.insert(store);
    return store;
}

Switch* Network::addSwitch(Switch * sw)
{
    sw->setID(switches.size());
    switches.insert(sw);
    return sw;
}

Link* Network::addLink(Link * link)
{
    link->setID(links.size());
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
