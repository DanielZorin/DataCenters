#pragma once

#include "edge.h"
#include "criteria.h"

class Link : public Edge {
    friend class ElementFactory;
    friend class Switch;
public:
    enum Attributes {
        NONE = 0
    };

    Link() : Edge() {
        type = LINK;
    } 

private:
    virtual bool typeCheck(const Element * other) const {
        return Criteria::isLink(other);
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
    unsigned throughput;
};
