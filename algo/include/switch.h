#pragma once

#include "node.h"
#include "link.h"
#include "criteria.h"

class Switch : public Node {
    friend class ElementFactory;
public:
    Switch(bool isVirtualRouter = false) : Node(), isVirtualRouter(isVirtualRouter) {
        type = SWITCH;
    }
private:

    virtual bool typeCheck(const Element * other) const {
        return Criteria::isLink(other);
    }

    virtual bool physicalCheck(const Element * other) const {
        Link * link = other->toLink();
        // if ( throughput < link->throughput ) return false;
        return true;
    }

    virtual void decreaseResources(const Element * other) {
        Link * link = other->toLink();
        //throughput -= link->throughput;
    }

    virtual void restoreResources(const Element * other) {
        Link * link = other->toLink();
        //throughput += link->throughput;
    }

private:
    // TODO: switch doesn't have any parameters, maybe latency calculation should be added on switch (buffer latency)
    //unsigned throughput;

    // TODO: maybe this is the wrong place
    bool isVirtualRouter;
};
