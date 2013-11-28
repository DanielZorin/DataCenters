#pragma once

#include "graph.h"

class Network : public Graph {
public:
    const Computers & getComputers() const {
        return computers;
    }

    const Stores & getStores() const {
        return stores;
    }

    const Switches & getSwitches() const {
        return switches;
    }

    const Links & getLinks() const {
        return links;
    }

    virtual Nodes & getNodes() {
        if ( nodes.empty() ) {
            for (Computers::iterator i = computers.begin(); i != computers.end(); i++)
                nodes.insert(*i);
            for (Stores::iterator i = stores.begin(); i != stores.end(); i++)
                nodes.insert(*i);
            for (Switches::iterator i = switches.begin(); i != switches.end(); i++)
                nodes.insert(*i);
        }
        return nodes;
    }

    virtual Edges & getEdges() {
        if ( edges.empty() )
            for( Links::iterator i = links.begin(); i != links.end(); i++)
                edges.insert(*i);
        return edges;
    }
private:
    Computers computers;
    Stores stores;
    Switches switches;
    Links links;
};
