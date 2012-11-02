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

    GraphComponent(unsigned long req, int phys, RequestType t);
    ~GraphComponent();

    GraphComponent(const GraphComponent & gc);
    GraphComponent& operator=(const GraphComponent & gc);

    RequestType getType() const { return type; }
    bool isCreated() const { return success; }

    void updateHeuristic(std::vector<unsigned long> & res, std::vector<unsigned long> & cap);
private:
    // initialize
    bool init(int num);

    // Arcs to physical resiurces
    std::vector<Arc*> physArcs;
    // was init() successful?
    bool success;
    // request type
    RequestType type;
    // requested resources
    unsigned long required;

    // No default constructor
    GraphComponent();
};

// Ant algorithm internal graph
// Manages pheromone and heuristic values, builds paths
class InternalGraph
{
public:
    InternalGraph(unsigned int nodes, unsigned int stores, unsigned int vm, unsigned int st,
                  std::vector<unsigned long> & res, std::vector<unsigned long> & cap, std::vector<unsigned long> & req);
    ~InternalGraph();

    bool isCreated() const { return success; }
private:
    // initialize
    bool init(std::vector<unsigned long> & res, std::vector<unsigned long> & cap, std::vector<unsigned long> & req);
    void clean(int i, int j, int k);
    // calculate heuristic for arcs
    void calcHeuristic(std::vector<unsigned long> & req);
    // update heuristic for every graph component
    void updateInternalHeuristic();

    // Vertices that correspond to requests
    std::vector<GraphComponent*> vertices;
    // Arcs between these vertices
    std::vector< std::vector<Arc*> > arcs;
    // Current available physical resources for computational nodes
    std::vector<unsigned long> nodesRes;
    // Current available physical resources for storages
    std::vector<unsigned long> storesRes;
    // Maximum available physical resources for computational nodes
    std::vector<unsigned long> nodesCap;
    // Maximum available physical resources for storages
    std::vector<unsigned long> storesCap;

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
