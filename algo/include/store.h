#pragma once

#include "node.h"
#include "criteria.h"

class Store : public Node {
    friend class ElementFactory;
public:
    enum Attributes {
        NONE = 0,
        REPLICABLE = 1
    };

    Store() : Node() {
        type = STORE;
    }

private:
    virtual bool typeCheck(const Element * other) const {
        return Criteria::isStore(other);
    }

    virtual bool physicalCheck(const Element * other) const {
        Store * storage = other->toStore();
        if ( capacity < storage->capacity ) return false;
        if ( readrate < storage->readrate ) return false;
        if ( writerate < storage->writerate ) return false;
        return true;
    }

    virtual void decreaseResources(const Element * other) {
        Store * storage = other->toStore();
        capacity -= storage->capacity;
        readrate -= storage->readrate;
        writerate -= storage->writerate;
    }

    virtual void restoreResources(const Element * other) {
        Store * storage = other->toStore();
        capacity += storage->capacity;
        readrate += storage->readrate;
        writerate += writerate;
    }
private:
    unsigned capacity;
    unsigned readrate;
    unsigned writerate;
};
