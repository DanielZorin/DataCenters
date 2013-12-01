#pragma once

#include "defs.h"
#include "link.h"
#include "node.h"

#include <vector>

class Path {
public:
    Path(Element * begin, Element * end) {
        if ( !Element::isPhysical(begin) ) throw;
        if ( !Element::isNode(begin) ) throw;
        if ( !Element::isPhysical(end) ) throw;
        if ( !Element::isNode(end) ) throw;

        from = begin;
        end = to;
    }

    inline bool isZeroPath() const {
        return from == to;
    }

    inline bool isValid() const {
        if ( isZeroPath() ) return true;
        if ( path.empty() ) return false;
        Element * last = path.back();
        if ( !Element::isLink(last) ) return false;
        Link * link = last->toLink();
        if ( !link->connects(to) ) return false;
        return true;
    }

    inline bool addElement(Element * element) {
        if ( !Element::isPhysical(element) ) return false;
        if ( path.empty() ) {
            if ( !Element::isLink(element) ) return false;
            Link * link = element->toLink();
            if ( !link->connects(from) ) return false;
            path.push_back(element);
            return true;
        } else {
            if ( Element::isLink(element) ) {
                Link * link = element->toLink();
                if ( !link->connects(path.back()) ) return false; 
            } else if ( Element::isSwitch(element) ) {
                Switch * sw = element->toSwitch();
                if ( !sw->hasEdge(path.back()) ) return false;
            } else {
                return false;
            }
            path.push_back(element);
        }
        return true;
    }

    const std::vector<Element *> getPath() const { 
        return path;
    }

private:
    std::vector<Element *> path;
    Element * from;
    Element * to;
};
