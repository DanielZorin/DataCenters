#pragma once

#include "defs.h"
#include "element.h"

namespace Operation {

    Elements filter(const Elements & elements, bool (*criterion)(const Element *)) {
        Elements result;
        for ( Elements::iterator e = elements.begin(); e != elements.end(); e++) {
            Element * element = *e;
            if (criterion(element)) {
                result.insert(element);
            } 
        }
        return result;
    } 

    Elements intersect(const Elements & first, const Elements & second) {
        Elements result;
        if ( first.empty() ) return result;
        if ( second.empty() ) return result;
        const Elements & less = first.size() < second.size() ? first : second;
        const Elements & more = first.size() < second.size() ? second : first;
        for ( Elements::const_iterator e = less.begin(); e != less.end(); e++ )
            if ( more.find(*e) != more.end() )
                result.insert(*e);
        return result;
    }

    Elements join(const Elements & first, const Elements & second) {
        if ( first.empty() ) return second;
        if ( second.empty() ) return first;
        Elements result(first);
        result.insert(second.begin(), second.end());
        return result;
    }

    Elements minus(const Elements & first, const Elements & second) {
        if ( first.empty() ) return first;
        if ( second.empty() ) return first;
        Elements result(first);
        for ( Elements::iterator e = second.begin(); e != second.end(); e++ )
            result.erase(*e);
        return result;
    }
};
