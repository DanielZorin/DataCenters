#pragma once

#include "node.h"

class Computer : public Node {
    friend class XMLFactory;
public:
    enum Attributes {
        NONE = 0
    };

private:
    Computer() : Node() {
        type = COMPUTER;
    }

    virtual bool typeCheck(const Element * other) const {
        return other->isComputer();
    }

    virtual bool attributeCheck(const Element * other) const {
        Computer * vm = other->toComputer();
        return (attributes & vm->attributes) == vm->attributes; 
    }

    virtual bool physicalCheck(const Element * other) const {
        Computer * vm = other->toComputer();
        if ( cores < vm->cores ) return false;
        if ( ram < vm->ram ) return false;
        return true;
    }

    virtual void decreaseResources(const Element * other) {
        Computer * vm = other->toComputer();
        cores -= vm->cores;
        ram -= vm->ram;
    }

    virtual void restoreResources(const Element * other) {
        Computer * vm = other->toComputer();
        cores += vm->cores;
        ram += vm->ram;
    }

private:
    int cores;
    long ram;
    Attributes attributes;
};
