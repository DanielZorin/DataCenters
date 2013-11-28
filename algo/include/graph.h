#pragma once

#include "defs.h"

class Graph {
public:
    virtual Nodes & getNodes() = 0;
    virtual Edges & getEdges() = 0;
    const Elements & getElements() {
        if ( elements.empty() ) {
            getNodes();
            getEdges();
            for (Nodes::iterator i = nodes.begin(); i != nodes.end(); i++)
                elements.insert(*i);
            for (Edges::iterator i = edges.begin(); i != edges.end(); i++)
                elements.insert(*i);
        }
        return elements;
    }
protected:
    Nodes nodes;
    Edges edges;
private:
    Elements elements;
};
