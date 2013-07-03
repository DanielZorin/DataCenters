#ifndef INTERNALGRAPH_H
#define INTERNALGRAPH_H

#include <vector>
#include <set>
#include "../common/element.h"
#include "path.h"

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

    GraphComponent(unsigned long req, int phys, Element * pr, RequestType t, unsigned int sType = 0, unsigned int ramReq = 0);
    ~GraphComponent();

    GraphComponent(const GraphComponent & gc);
    GraphComponent& operator=(const GraphComponent & gc);

    unsigned int getResNum() const { return physArcs.size(); }
    RequestType getType() const { return type; }
    unsigned long getRequired() const { return required; }
    unsigned long getRequiredRam() const { return ramRequired; }
    bool isCreated() const { return success; }

    Element * getPointer () { return request; }

    // Perform some actions when a new path is about to start building
    void nextPath();
    // Init pheromone and heuristic values
    void initValues(std::vector<unsigned long> & res, std::vector<unsigned long> & cap, std::vector<unsigned int> & types);
    // Update heuristic on arcs
    void updateHeuristic(unsigned int resNum, unsigned int resCur, unsigned int resCap);
    // Update pheromone on the arc
    void updatePheromone(unsigned int res, double value);
    // Choose resource for the request
    unsigned int chooseResource(std::vector<unsigned long> & ram, double pherDeg, double heurDeg);
private:
    // initialize
    bool init(int num);

    // Arcs to physical resources
    std::vector<Arc*> physArcs;
    // Initial heuristic values
    std::vector<double> physHeurs;
    // Non-normalized heuristic values
    std::vector<double> justHeurs;
    // Non-normalized initial heuristic values
    std::vector<double> initJustHeurs;
    // was init() successful?
    bool success;
    // request type
    RequestType type;
    // requested resources
    unsigned long required;
    // requested memory (for virtual machine graph component)
    unsigned long ramRequired;
    // storage type (for storage graph component)
    unsigned int storageType;
    // pointer to corresponding request element
    Element * request;

    // No default constructor
    GraphComponent();
};

// Ant algorithm internal graph
// Manages pheromone and heuristic values, builds paths
class InternalGraph
{
public:
    InternalGraph(unsigned int nodes, unsigned int stores, unsigned int vm, unsigned int st, // request set and network parameters
                  std::vector<unsigned long> & res, // current physical resources capacity
                  std::vector<unsigned long> & cap, // maximum physical resources capacity
                  std::vector<unsigned long> & ramRes, // current virtual machine memory capacity
                  std::vector<unsigned long> & ramCap, // maximum virtual machine memory capacity
                  std::vector<unsigned long> & ramReq, // amount of memory requested by virtual machines
                  std::vector<unsigned long> & req, // amount of resources requested
                  std::vector<unsigned int> & types, // physical store types
                  std::vector<unsigned int> & reqTypes, // virtual storage types
                  std::vector<Element *> & pn, // pointers to physical nodes
                  std::vector<Element *> & ps, // pointers to physical stores
                  std::vector<Element *> & virtElems // pointer to virtual requests
                  );
    ~InternalGraph();

    bool isCreated() const { return success; }
    // Perform some actions when a new path is about to start building
    void nextPath();
    // When building the best path, call this function to assign path's vertices to graph's resources so it is possible to build the same links later
    void assignPath(AntPath * pt);
    // Update pheromone
    void updatePheromone(std::vector<AntPath*> & paths, std::vector<double> & objValues, double evapRate, double max);
    // Update heuristic for every graph component
    void updateInternalHeuristic(unsigned int resNum, GraphComponent::RequestType t);
    // From vertex cur, select a new vertex (one of the set members)
    unsigned int selectVertex(AntPath* pt, unsigned int cur, std::set<unsigned int> & available, bool& s, std::map< Element *, std::set<Link *> >& chan);
    // Set heuristic degree and pheromone degree
    void setHeurDeg(double deg) { heurDeg = deg; }
    void setPherDeg(double deg) { pherDeg = deg; }
    // Request erased from path, add the resources back
    void requestErased(int resource, unsigned int request, GraphComponent::RequestType t);
    std::vector<unsigned long> getCurStoresRes() { return curStoresRes; }
    void decreaseCurStoresRes(int index, unsigned long cap) { curStoresRes[index] -= cap; }
    void increaseCurStoresRes(int index, unsigned long cap) { curStoresRes[index] += cap; }
private:
    // initialize
    bool init(std::vector<unsigned long> & res, std::vector<unsigned long> & cap, std::vector<unsigned long> & ramRes, std::vector<unsigned long> & ramCap,
              std::vector<unsigned long> & req, std::vector<unsigned long> & ramReq, std::vector<unsigned int> & reqTypes, std::vector<Element *> & pn,
              std::vector<Element *> & ps, std::vector<Element *> & virtElems);
    void clean(int i, int j, int k);
    // calculate heuristic for arcs
    void initValues(std::vector<unsigned long> & req, std::vector<unsigned int> & types);

    // Vertices that correspond to requests
    std::vector<GraphComponent*> vertices;
    // Arcs between these vertices
    std::vector< std::vector<Arc*> > arcs;
    // Current available physical resources for computational nodes (at the start)
    std::vector<unsigned long> nodesRes;
    // Current available memory for computational nodes (at the start)
    std::vector<unsigned long> nodesRam;
    // Current available physical resources for storages (at the start)
    std::vector<unsigned long> storesRes;
    // Current available physical resources for computational nodes (when the path is being built)
    std::vector<unsigned long> curNodesRes;
    // Current available mempry for computational nodes (when the path is being built)
    std::vector<unsigned long> curNodesRam;
    // Current available physical resources for storages (when the path is being built)
    std::vector<unsigned long> curStoresRes;

    // Maximum available physical resources for computational nodes
    std::vector<unsigned long> nodesCap;
    // Maximum available mempry for computational nodes
    std::vector<unsigned long> nodesCapRam;
    // Maximum available physical resources for storages
    std::vector<unsigned long> storesCap;

    // Pointers to physical nodes
    std::vector<Element *> physNodes;
    // Pointers to physical stores
    std::vector<Element *> physStores;

    // select parameters
    double pherDeg;
    double heurDeg;

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
