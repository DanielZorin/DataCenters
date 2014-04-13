#pragma once

#include "node.h"

class Computer : public Node {
    friend class ElementFactory;
public:
    enum Attributes {
        NONE = 0
    };

    Computer(bool vnf = false) : Node(), vnf(vnf) {
        type = COMPUTER;
    }

    bool isVnf() const {
    	return vnf;
    }

private:
    virtual bool typeCheck(const Element * other) const {
        return other->isComputer();
    }

    bool vnf;

};
