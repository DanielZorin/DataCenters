#ifndef INTERNALGRAPH_H
#define INTERNALGRAPH_H

#include <vector>

// Class representing a single arc in the graph
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

    Arc(const Arc & a);
    Arc& operator=(const Arc & a);
    // default destructor
};

// Class representing a vertex in the graph that corresponds to one of the requests
// Also manages arcs to the vertices representing physical resources
class GraphComponent
{
public:
    typedef enum {NOTYPE = 0, VMACHINE = 1, STORAGE = 2} RequestType;

    GraphComponent(int phys, RequestType t);
    ~GraphComponent();

    GraphComponent(const GraphComponent & gc);
    GraphComponent& operator=(const GraphComponent & gc);

    RequestType getType() { return type; }
    bool isCreated() { return success; }
private:
    // initialize
    bool init(int num);

    // Arcs to physical resiurces
    std::vector<Arc*> physArcs;

    // was init() successful?
    bool success;
    // request type
    RequestType type;

    // No default constructor
    GraphComponent();
};

// Ant algorithm internal graph
// Manages pheromone and heuristic values, builds paths
class InternalGraph
{
public:
    InternalGraph(unsigned int nodes, unsigned int stores, unsigned int vm, unsigned int st);
    ~InternalGraph();

    bool isCreated() { return success; }
private:
    // initialize
    bool init();

    // Vertices that correspond to requests
    std::vector<GraphComponent*> vertices;
    // Arcs between these vertices
    std::vector< std::vector<Arc*> > arcs;
    // Current available physical resources for storages and computational nodes
    std::vector<double> resources;

    // graph parameters
    int nodesNum;
    int storesNum;
    int vmNum;
    int stNum;

    // was init() successful?
    bool success;

    // No default constructor, copy constructor and operator=
    InternalGraph();
    InternalGraph(const InternalGraph&);
    InternalGraph& operator=(const InternalGraph&);
};

#endif
