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

};
