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
    AntAlgorithm(Network * n, Requests const & r, unsigned int ants, unsigned int iter, double pd, double hd, double evap);
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
    // Best path and its value
    AntPath* bestPath;
    double bestValue;
    // Objective function values for each path+netpath
    std::vector<double> objValues;
    // copy network
    Network * copyNetwork;

    // parameters
    unsigned int antNum;
    unsigned int iterNum;
    double heurDeg;
    double pherDeg;
    double evapRate;

    // private functions
    bool init();
    bool buildPath(unsigned int ant);
    std::vector<NetPath>* buildLink(unsigned int ant, bool resultNeeded);
    unsigned int objFunctions();
    void removeRequestElements(unsigned int vertex, AntPath* pt, std::set<unsigned int> & availableVM, std::set<unsigned int> & availableST, GraphComponent::RequestType t);

    // is init() successful?
    bool success;
    // No default constructor, copy constructor and operator=
    AntAlgorithm();
    AntAlgorithm(const AntAlgorithm&);
    AntAlgorithm& operator=(const AntAlgorithm&);
};

#endif
