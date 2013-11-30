#pragma once

#include "defs.h"
#include "operation.h"

class Graph {
public:
    inline const Elements & getElements() const {
        return elements;
    }

    inline Elements getNodes() const {
        return Operation::filter(getElements(), Element::isNode);
    }
    
    inline Elements getEdges() const {
        return Operation::filter(getElements(), Element::isEdge);
    }

protected:
    Elements elements;
};
