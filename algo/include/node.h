#pragma once

#include "element.h"

class Node : public Element {
protected:
    Node() : Element() {}
public:
    Elements & getEdges() {
        return edges;
    }

    void addEdge(Element * edge) {
        if ( !edge->isEdge() )
            return;
        edges.insert(edge);
    }

    void removeEdge(Element * edge) {
        if ( !edge->isEdge() )
            return;
        edges.erase(edge);
    }

private:
    Elements edges;
};
