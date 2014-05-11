#pragma once

#include "element.h"
#include "criteria.h"
#include "edge.h"
#include "port.h"

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

    const Ports& getPorts() const {
        return ports;
    }

    Port* getPortByName(std::string name) const {
        for (Ports::const_iterator it = ports.begin(); it != ports.end(); ++it )
            if ( (*it)->getName().compare(name) == 0 )
                return *it;
        return 0;
    }

    virtual Elements adjacent() const {
        return edges;
    }

    virtual Elements adjacentNodes() const {
        Elements result;
        Elements edges = adjacentEdges();
        for (Elements::iterator i = edges.begin(); i != edges.end(); i++) {
            Edge * edge = (*i)->toEdge();
            result.insert(edge->getAdjacent(this));
        }
        return result;
    }

    virtual Elements adjacentEdges() const {
        Elements result;
        for(Ports::iterator i = ports.begin(); i != ports.end(); i++ ) {
            Port * p = *i;
            Edge * e = p->getConnectedLink();
            if ( e != 0 )
                result.insert(e);
        }
        return result;
    }

private:
    Elements edges;
    Ports ports;
};
