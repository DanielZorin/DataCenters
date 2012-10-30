#ifndef REQUEST_H
#define REQUEST_H

#include "publicdefs.h"

#include <string>
using std::string;

// Class representing a resource request to the data center
class Request
{
public:
    // types used for defining the request
    typedef Nodes VirtualMachines;
    typedef Stores Storages;
    typedef Links VirtualLinks;
public:
    Request() {}
    // construct empty request
    Request(string name);
    // copy constructor
    Request(const Request & r);
    // destructor
    ~Request();

    // operator=
    Request& operator=(const Request & r);

    // Getters/Setters
    const VirtualMachines& getVirtualMachines() const;
    const Storages& getStorages() const;
    const VirtualLinks& getVirtualLinks() const;
    VirtualMachines& getVirtualMachines();
    Storages& getStorages();
    VirtualLinks& getVirtualLinks();

    inline string getName() { return name; }

    Node* addVirtualMachine(Node * node);
    Store* addStorage(Store * store);
    Link* addLink(Link * link);
private:
    // Members
    VirtualMachines virtualMachines;
    Storages storages;
    VirtualLinks virtualLinks;

    string name;
};

#endif // REQUEST_H
