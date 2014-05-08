#pragma once

#include "node.h"

class Computer : public Node {
    friend class ElementFactory;
public:
    enum Attributes {
        NONE = 0,
        VNF = 1
    };

    Computer(bool vnf = false) : Node() {
        type = COMPUTER;
        if (vnf)
        	attributes |= VNF;
    }

    bool isVnf() const {
    	return attributes & VNF == VNF;
    }

private:
    virtual bool typeCheck(const Element * other) const {
        return other->isComputer();
    }

};
