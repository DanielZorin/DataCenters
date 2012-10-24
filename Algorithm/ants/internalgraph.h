#ifndef INTERNALGRAPH_H
#define INTERNALGRAPH_H

#include <vector>

struct Arc
{
    double pher;
    double heur;

    Arc()
    : pher(0)
    , heur(0)
    {}

    Arc(double p, double h)
    : pher(p)
    , heur(h)
    {}

    // default destructor
};

class GraphComponent
{
public:
    typedef enum {NOTYPE = 0, VMACHINE = 1, STORAGE = 2} RequestType;

    GraphComponent(int phys, RequestType t);
    ~GraphComponent();

    RequestType getType() { return type; }
    bool isCreated() { return success; }

private:
    bool init(int num);

    std::vector<Arc*> physArcs;

    bool success;
    RequestType type;

    // No default constructor
    GraphComponent();
};

class InternalGraph
{
public:
    InternalGraph(unsigned int nodes, unsigned int stores, unsigned int vm, unsigned int st);
    ~InternalGraph();

    bool isCreated() { return success; }
private:
    bool init();

    std::vector<GraphComponent*> vertices;
    std::vector< std::vector<Arc*> > arcs;
    std::vector<double> resources;

    int nodesNum;
    int storesNum;
    int vmNum;
    int stNum;

    bool success;

    // No default constructor
    InternalGraph();
};

#endif
