#ifndef REQUEST_H
#define REQUEST_H

#include "publicdefs.h"

class Request
{
public:
    // types used for defining the request
    typedef Nodes VirtualMachines;
    typedef Stores Storages;
    typedef Links VirtualLinks;
public:
    // construct empty request
    Request()
    {}

    // TODO: construct non-empty request and/or add elements to request
    // TODO: destructor
public:
    // Getters/Setters

    VirtualMachines& getVirtualMachines()
    {
        return virtualMachines;
    }

    Storages& getStorages()
    {
        return storages;
    }

    VirtualLinks& getVirtualLinks()
    {
        return virtualLinks;
    }

private:
    // Members:

    VirtualMachines virtualMachines;
    Storages storages;
    VirtualLinks virtualLinks;
};

#endif // REQUEST_H
