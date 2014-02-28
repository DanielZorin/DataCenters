#pragma once

#include "element.h"
#include "criteria.h"

class Edge : public Element {
protected:
    Edge() : Element(), first(0), second(0) {}
public:
    Element * getFirst() const {
        return first;
    }

    Element * getSecond() const {
        return second;
    }

    bool connect(Element * first, Element * second) {
        if ( Criteria::isPhysical(this) != Criteria::isPhysical(first)) return false;
        if ( Criteria::isPhysical(this) != Criteria::isPhysical(second)) return false;
        if ( !Criteria::isNode(first) ) return false;
        if ( !Criteria::isNode(second) ) return false;

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

    virtual Elements adjacent() const {
        Elements result;
        result.insert(first);
        result.insert(second);
        return result; 
    }
protected:
    Element * first;
    Element * second;
};
