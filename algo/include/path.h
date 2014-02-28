#pragma once

#include "defs.h"
#include "link.h"
#include "node.h"
#include "switch.h"
#include "criteria.h"

#include <vector>
#include <algorithm>

class Path {
public:
    Path() : from(0), to(0) {}
    Path(Element * begin, Element * end) {
        if ( !Criteria::isPhysical(begin) ) throw;
        if ( !Criteria::isNode(begin) ) throw;
        if ( !Criteria::isPhysical(end) ) throw;
        if ( !Criteria::isNode(end) ) throw;

        from = begin;
        end = to;
    }

    inline bool isZeroPath() const {
        return from == to;
    }

    inline bool isValid() const {
        if ( from == 0 || to == 0 ) return false;
        if ( isZeroPath() ) return true;
        if ( path.empty() ) return false;
        Element * last = path.back();
        if ( !Criteria::isLink(last) ) return false;
        Link * link = last->toLink();
        if ( !link->connects(to) ) return false;
        return true;
    }

    inline bool addElement(Element * element) {
        if ( !Criteria::isPhysical(element) ) return false;
        if ( path.empty() ) {
            if ( !Criteria::isLink(element) ) return false;
            Link * link = element->toLink();
            if ( !link->connects(from) ) return false;
        } else {
            if ( Criteria::isLink(element) ) {
                Link * link = element->toLink();
                if ( !link->connects(path.back()) ) return false; 
            } else if ( Criteria::isSwitch(element) ) {
                Switch * sw = element->toSwitch();
                if ( !sw->hasEdge(path.back()) ) return false;
            } else {
                return false;
            }
        }
        path.push_back(element);
        return true;
    }

    inline void revert() {
        if ( !isValid() )
            return;

        Element * tmp = from;
        from = to;
        to = from;
        std::reverse(path.begin(), path.end()); 
    }

    const std::vector<Element *> getPath() const { 
        return path;
    }

private:
    std::vector<Element *> path;
    Element * from;
    Element * to;
};
