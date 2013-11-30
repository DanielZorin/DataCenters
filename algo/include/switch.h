#pragma once

#include "node.h"
#include "link.h"

class Switch : public Node {
    friend class XMLFactory;
private:
    Switch() : Node() {
        type = SWITCH;
    }

    virtual bool typeCheck(const Element * other) const {
        return Element::isLink(other);
    }

    virtual bool attributeCheck(const Element * other) const {
        Link * link = other->toLink();
        return (attributes & link->attributes) == link->attributes;
    } 

    virtual bool physicalCheck(const Element * other) const {
        Link * link = other->toLink();
        if ( throughput < link->throughput ) return false;
        return true;
    }

    virtual void decreaseResources(const Element * other) {
        Link * link = other->toLink();
        throughput -= link->throughput;
    }

    virtual void restoreResources(const Element * other) {
        Link * link = other->toLink();
        throughput += link->throughput;
    }

private:
    Link::Attributes attributes;
    long throughput;
};
