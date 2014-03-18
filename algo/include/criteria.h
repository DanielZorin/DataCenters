#pragma once

#include "element.h"

class Criteria {
public:
    inline static bool isComputer(const Element * e) { 
        return e->isComputer();
    }

    inline static bool isStore(const Element * e) {
        return e->isStore();
    }

    inline static bool isSwitch(const Element * e) { 
        return e->isSwitch();
    }

    inline static bool isLink(const Element * e) { 
        return e->isLink();
    }

    inline static bool isComputational(const Element * e) {
        return e->isComputational();
    }

    inline static bool isNetwork(const Element * e) {
        return e->isNetwork();
    }

    inline static bool isNode(const Element * e) {
        return e->isNode();
    }

    inline static bool isEdge(const Element * e) {
        return e->isEdge();
    }

    inline static bool isPhysical(const Element * e) {
        return e->isPhysical();
    }

    inline static bool isVirtual(const Element * e) {
        return e->isVirtual();
    }

    inline static bool isAvailable(const Element * e) {
        return e->isAvailable();
    }

    inline static bool isAssigned(const Element * e) {
        return e->isAssigned();
    }

    inline static bool isAdjacent(const Element * t, const Element * e) {
        return e->isAdjacent(t); 
    }
};
