#ifndef REPLICATION_H
#define REPLICATION_H

#include <map>
#include "publicdefs.h"

// The class, which represents the replication for some storage specified.
// The replication is the copy of the database in another memory store, for which
// the channel for maintaining consistency exists.

class Replication
{
public:
    // Constructor, destructor.
    Replication() {}
    ~Replication() {}

public:
    // Getters/setters.

    // The storage, for which replication is maintained.
    Storage * getStorage()
    {
        return storage;
    }
    void setStorage(Storage * storage)
    {
        this->storage = storage;
    }

    // Memory stores, between which replication is maintained.
    Store * getFirstStore()
    {
        return first;
    }
    Store * getSecondStore()
    {
        return second;
    }
    void bind(Store * firstStore, Store * secondStore)
    {
        first = firstStore;
        second = secondStore;
    }

    // Network path between stores.
    NetPath& getLink()
    {
        return link;
    }
    void setLink(NetPath& link)
    {
        this->link = NetPath(link.begin(), link.end());
    }

public:
    // Remove the replication.
    // This also removes the assignments of the replication.
    //
    void Remove();
    
private:
    // The storage, for which replication is maintained.
    Storage * storage;

    // Memory stores, between which replication is maintained.
    Store * first;
    Store * second;

    // Network path between stores.
    NetPath link;
};

#endif
