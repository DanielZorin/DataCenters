#pragma once

#include "element.h"

class Node : public Element {
protected:
    Node() : Element() {}
public:
    Elements & getEdges() {
        return edges;
    }

    bool addEdge(Element * edge) {
        if ( !edge->isEdge() )
            return false;
        edges.insert(edge);
        return true;
    }

private:
    Elements edges;
};
