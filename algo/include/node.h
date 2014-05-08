#pragma once

#include "element.h"
#include "criteria.h"
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
        for (Elements::iterator i = edges.begin(); i != edges.end(); i++) {
            Element * e = *i;
            Elements nodes = e->adjacent();
            result.insert(nodes.begin(), nodes.end());
        }
        return result;
    }

    virtual Elements adjacentEdges() const {
        return adjacent();
    }

private:
    Elements edges;
    Ports ports;
};
