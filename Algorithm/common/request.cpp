#include "request.h"
#include "node.h"
#include "link.h"
#include "store.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

Request::Request(string n)
{
    name = n;
}

Request::Request(const Request & r)
{
    for (VirtualMachines::const_iterator i = r.virtualMachines.begin(); i != r.virtualMachines.end(); i ++)
        virtualMachines.insert(new Node((*i)->getName(), (*i)->getCapacity(), (*i)->getMaxCapacity()));

    for (Stores::const_iterator i = r.storages.begin(); i != r.storages.end(); i ++)
        storages.insert(new Store((*i)->getName(), (*i)->getCapacity(), (*i)->getMaxCapacity(), (*i)->getTypeOfStore()));

    for (VirtualLinks::const_iterator i = r.virtualLinks.begin(); i != r.virtualLinks.end(); i ++)
    {
        Link* tmp = new Link((*i)->getName(), (*i)->getCapacity(), (*i)->getMaxCapacity());
        tmp->bindElements((*i)->getFirst(), (*i)->getSecond());
        virtualLinks.insert(tmp);
    }
}

Request::~Request()
{
    for (VirtualMachines::const_iterator i = virtualMachines.begin(); i != virtualMachines.end(); i ++)
        delete *i;

    for (Stores::const_iterator i = storages.begin(); i != storages.end(); i ++)
        delete *i;

    for (VirtualLinks::const_iterator i = virtualLinks.begin(); i != virtualLinks.end(); i ++)
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
    for (VirtualMachines::const_iterator i = r.virtualMachines.begin(); i != r.virtualMachines.end(); i ++)
        virtualMachines.insert(new Node((*i)->getName(), (*i)->getCapacity(), (*i)->getMaxCapacity()));

    for (Stores::const_iterator i = r.storages.begin(); i != r.storages.end(); i ++)
        storages.insert(new Store((*i)->getName(), (*i)->getCapacity(), (*i)->getMaxCapacity(), (*i)->getTypeOfStore()));

    for (VirtualLinks::const_iterator i = r.virtualLinks.begin(); i != r.virtualLinks.end(); i ++)
    {
        Link* tmp = new Link((*i)->getName(), (*i)->getCapacity(), (*i)->getMaxCapacity());
        tmp->bindElements((*i)->getFirst(), (*i)->getSecond());
        virtualLinks.insert(tmp);
    }

    return *this;
}

void Request::printRequest()
{
	cout << "======================PrintRequest===================" << endl;
	cout << "Name: " << name << endl;
	if (virtualMachines.begin() != virtualMachines.end()) {
		cout << "virtualMachines:" << endl;
	}
	int vms_counter = 0;
	for (VirtualMachines::const_iterator i = virtualMachines.begin(); i != virtualMachines.end(); ++i) {
		++vms_counter;
		printf("%ld %ld %ld\n", (*i)->getID(), (*i)->getCapacity(), (*i)->getMaxCapacity());
	}
	printf("Total of %d virtual machines\n", vms_counter);
	if (storages.begin() != storages.end()) {
		cout << "storages: " << endl;
	}
	int storages_counter = 0;
	for (Storages::const_iterator i = storages.begin(); i != storages.end(); ++i) {
		++storages_counter;
		printf("%ld %ld %ld\n", (*i)->getID(), (*i)->getCapacity(), (*i)->getMaxCapacity());
	}
	printf("Total of %d storages\n", storages_counter);
}

Node* Request::addVirtualMachine(Node * node)
{
	//node->setID(virtualMachines.size());
    virtualMachines.insert(node);
    return node;
}

Store* Request::addStorage(Store * store)
{
	//store->setID(storages.size());
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
