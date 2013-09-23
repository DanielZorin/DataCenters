#ifndef ANT_H
#define ANT_H

#include "../common/node.h"
#include "../common/store.h"
#include "../common/link.h"
#include "../common/switch.h"
#include "../common/network.h"
#include "../common/request.h"
#include "../common/assignment.h"
#include "../common/replication.h"
#include "../common/algorithm.h"
#include "internalgraph.h"
#include "path.h"

// A struct to store information about assigned virtual channel
// Makes it possible to easy unassign it if needed
struct AssignedChannel
{
    // channel to virtual machine
    NetPath * dataChannel;
    //  consistency channel (NULL if not used or there is another object of this type with the same repChannel)
    NetPath * repChannel;
    // The store where the replica is (NULL if not used)
    Store* replica;
    // Its index (only makes sense when replica is not NULL)
    unsigned int rindex;

    AssignedChannel()
    : dataChannel(NULL)
    , repChannel(NULL)
    , replica(NULL)
    , rindex(0)
    {
    }

    AssignedChannel(NetPath * dc, NetPath * rc, Store * st, unsigned int ri)
    : dataChannel(dc)
    , repChannel(rc)
    , replica(st)
    , rindex(ri)
    {
    }
};

// Ant algorithm control class
// Constructs solution using Network and a set of requests
class AntAlgorithm: public Algorithm
{
public:
    AntAlgorithm(Network * n, Requests const & r, unsigned int ants = 20, unsigned int iter = 50, double pd = 1.5, double hd = 2, double evap = 0.15);
    ~AntAlgorithm();

    // Main function
    virtual Algorithm::Result schedule();

    bool isCreated() const { return success; }
private:
    // internal graph for ant algorithm
    InternalGraph * graph;
    // number of virtual machines and storages in all the requests
    int vmCount;
    int stCount;
    // Path storages (one iteration)
    std::vector<AntPath*> paths;
    std::vector<AntPath*> originPaths;
    // Best path and its value
    AntPath* bestPath;
    double bestValue;
    // Objective function values for each path+netpath
    std::vector<double> objValues;
    // copy network
    Network * copyNetwork;
    // A set of virtual channels for each request element that should be assigned when this request element is processed in BuildLink
    // Poiner to the set needed is copied to PathElement when it is created
    std::map< Element *, std::set<Link *> > virtChan;
    // Which global request does the request element with a certain internal number corresponds to
    // Represented as an array of pointers to requests, usage: numberToPointer[request element number] = pointer to the global request
    Request ** numberToPointer;

    // parameters
    unsigned int antNum;
    unsigned int iterNum;
    double heurDeg;
    double pherDeg;
    double evapRate;

    // private functions
    bool init();
    // Main ant algorithm functions
    bool buildPath(unsigned int ant);
    void buildLink(unsigned int ant, std::map<Link *, AssignedChannel> & channels, bool restore = true);
    unsigned int objFunctions();
    // When a request element is removed from the path, remove all other request elements corresponding to the same request
    void removeRequestElements(unsigned int vertex, AntPath* pt, std::set<unsigned int> & availableVM, std::set<unsigned int> & availableST,
                               GraphComponent::RequestType t, bool update = true);
    // Replica processing functions
    bool lastReplica(const std::map<Link *, AssignedChannel> & channels, Link * link, Element * rep);
    bool replicaExists(const std::map<Link *, AssignedChannel> & channels, Element * replicating, Element * st);
    const AssignedChannel * findReplica(const std::map<Link *, AssignedChannel> & channels, Element * replicating);

    // is init() successful?
    bool success;
    // No default constructor, copy constructor and operator=
    AntAlgorithm();
    AntAlgorithm(const AntAlgorithm&);
    AntAlgorithm& operator=(const AntAlgorithm&);
};

#endif
