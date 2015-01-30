#pragma once

#include "leafnode.h"

class Store : public LeafNode {
    friend class ElementFactory;
public:
    enum Attributes {
        NONE = 0,
        REPLICABLE = 1
    };

    Store() : LeafNode() {
        type = STORE;
    }

private:
    virtual bool typeCheck(const Element * other) const {
        return other->isStore();
    }

};
