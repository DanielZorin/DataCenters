#pragma once

#include "element.h"

class Edge : public Element {
protected:
    Edge() : Element(), first(0), second(0) {}
public:
    Node * getFirst() const {
        return first->toNode();
    }

    Node * getSecond() const {
        return second->toNode();
    }

    bool connect(Element * first, Element * second) {
        if ( physical != first->isPhysical() || physical != second->isPhysical())
            return false;
        if ( !first->isNode() ) return false;
        if ( !first->isNode() ) return false;
        this->first = first;
        this->second = second;
        return true;
    }

    bool connects(const Element * node) const {
        return getAdjacent(node) == 0;
    }

    Element * getAdjacent(const Element * node) const {
        if ( first == node ) return second;
        if ( second == node ) return first;
        return 0;
    }

protected:
    Element * first;
    Element * second;
};
