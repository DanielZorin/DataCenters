#pragma once

#include "element.h"
#include "criteria.h"

class Node : public Element {
protected:
    Node() : Element() {}
public:
    Elements & getEdges() {
        return edges;
    }

    bool addEdge(Element * element) {
        if ( Criteria::isEdge(element) )
            return false;
        edges.insert(element);
        return true;
    }

    bool hasEdge(Element * element) {
        if ( !Criteria::isEdge(element) ) return false;
        return edges.find(element) != edges.end(); 
    }

    bool addPort(Port * port) {
    	ports.insert(port);
    }

    Ports getPorts() const {
		return ports;
	}

    virtual Elements adjacent() const {
        return edges;
    }

private:
    Elements edges;
    Ports ports;
};
