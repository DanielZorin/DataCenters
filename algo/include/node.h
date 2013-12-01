#pragma once

#include "element.h"

class Node : public Element {
protected:
    Node() : Element() {}
public:
    Elements & getEdges() {
        return edges;
    }

    bool addEdge(Element * element) {
        if ( Element::isEdge(element) )
            return false;
        edges.insert(element);
        return true;
    }

    bool hasEdge(Element * element) {
        if ( !Element::isEdge(element) ) return false;
        return edges.find(element) != edges.end(); 
    }

private:
    Elements edges;
};
