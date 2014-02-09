#pragma once

#include "node.h"

class Computer : public Node {
    friend class ElementFactory;
public:
    enum Attributes {
        NONE = 0
    };

    Computer() : Node() {
        type = COMPUTER;
    }

private:
    virtual bool typeCheck(const Element * other) const {
        return other->isComputer();
    }

};
