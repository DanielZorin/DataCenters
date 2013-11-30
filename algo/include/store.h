#pragma once

#include "node.h"

class Store : public Node {
    friend class XMLFactory;
public:
    enum Attributes {
        NONE = 0
    };

private:
    Store() : Node() {
        type = STORE;
    }

    virtual bool typeCheck(const Element * other) const {
        return Element::isStore(other);
    }

    virtual bool attributeCheck(const Element * other) const {
        Store * storage = other->toStore();
        return (attributes & storage->attributes) == storage->attributes;
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
    Attributes attributes;
    long capacity;
    long readrate;
    long writerate;
};
