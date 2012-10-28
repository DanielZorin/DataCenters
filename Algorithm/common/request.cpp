#include "request.h"
#include "node.h"
#include "link.h"
#include "store.h"

Request::Request()
{
}

Request::Request(const Request & r)
{
    for (VirtualMachines::iterator i = r.virtualMachines.begin(); i != r.virtualMachines.end(); i ++)
        virtualMachines.insert(new Node((*i)->getName(), (*i)->getCapacity()));

    for (Stores::iterator i = r.storages.begin(); i != r.storages.end(); i ++)
        storages.insert(new Store((*i)->getName(), (*i)->getCapacity()));

    for (VirtualLinks::iterator i = r.virtualLinks.begin(); i != r.virtualLinks.end(); i ++)
        virtualLinks.insert(new Link((*i)->getName(), (*i)->getCapacity()));
}

Request::~Request()
{
    for (VirtualMachines::iterator i = virtualMachines.begin(); i != virtualMachines.end(); i ++)
        delete *i;

    for (Stores::iterator i = storages.begin(); i != storages.end(); i ++)
        delete *i;

    for (VirtualLinks::iterator i = virtualLinks.begin(); i != virtualLinks.end(); i ++)
        delete *i;
}

Request& Request::operator=(const Request & r)
{
    if (&r == this) return *this;
    // clear
    for (VirtualMachines::iterator i = virtualMachines.begin(); i != virtualMachines.end(); i ++)
        delete *i;

    for (Stores::iterator i = storages.begin(); i != storages.end(); i ++)
        delete *i;

    for (VirtualLinks::iterator i = virtualLinks.begin(); i != virtualLinks.end(); i ++)
        delete *i;

    virtualMachines.clear();
    storages.clear();
    virtualLinks.clear();

    // add new items
    for (VirtualMachines::iterator i = r.virtualMachines.begin(); i != r.virtualMachines.end(); i ++)
        virtualMachines.insert(new Node((*i)->getName(), (*i)->getCapacity()));

    for (Stores::iterator i = r.storages.begin(); i != r.storages.end(); i ++)
        storages.insert(new Store((*i)->getName(), (*i)->getCapacity()));

    for (VirtualLinks::iterator i = r.virtualLinks.begin(); i != r.virtualLinks.end(); i ++)
        virtualLinks.insert(new Link((*i)->getName(), (*i)->getCapacity()));

    return *this;
}

Node* Request::addVirtualMachine(Node * node)
{
    virtualMachines.insert(node);
    return node;
}

Store* Request::addStorage(Store * store)
{
    storages.insert(store);
    return store;
}

Link* Request::addLink(Link * link)
{
    virtualLinks.insert(link);
    return link;
}

Request::VirtualMachines& Request::getVirtualMachines()
{
    return virtualMachines;
}

Request::Storages& Request::getStorages()
{
    return storages;
}

Request::VirtualLinks& Request::getVirtualLinks()
{
    return virtualLinks;
}

const Request::VirtualMachines& Request::getVirtualMachines() const
{
    return virtualMachines;
}

const Request::Storages& Request::getStorages() const
{
    return storages;
}

const Request::VirtualLinks& Request::getVirtualLinks() const
{
    return virtualLinks;
}
