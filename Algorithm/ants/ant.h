#ifndef ANT_H
#define ANT_H

#include "../common/node.h"
#include "../common/store.h"
#include "../common/link.h"
#include "../common/switch.h"
#include "../common/network.h"
#include "../common/request.h"
#include "../common/assignment.h"
#include "../common/algorithm.h"
#include "internalgraph.h"
#include "path.h"


// Ant algorithm control class
// Constructs solution using Network and a set of requests
class AntAlgorithm: public Algorithm
{
public:
    AntAlgorithm(Network * n, Requests const & r, unsigned int ants, unsigned int iter, double pd, double hd);
    ~AntAlgorithm();

    virtual Algorithm::Result schedule();

    bool isCreated() const { return success; }
private:
    // internal graph for ant algorithm
    InternalGraph * graph;
    // number of virtual machines and storages in all the requests
    int vmCount;
    int stCount;
    // Path storage (one iteration)
    std::vector<AntPath*> paths;
    // Objective function values for each path
    std::vector<double> objValues;

    // parameters
    unsigned int antNum;
    unsigned int iterNum;
    double heurDeg;
    double pherDeg;

    // private functions
    bool init();
    bool buildPath(unsigned int ant);
    bool buildLink(unsigned int ant);
    void removeRequestElements(unsigned int vertex, AntPath* pt, std::set<unsigned int> & available, GraphComponent::RequestType t);

    // is init() successful?
    bool success;
    // No default constructor, copy constructor and operator=
    AntAlgorithm();
    AntAlgorithm(const AntAlgorithm&);
    AntAlgorithm& operator=(const AntAlgorithm&);
};

#endif
