#ifndef DIJKSTRAROUTER_H
#define DIJKSTRAROUTER_H

#include "router.h"

class DijkstraRouter : public Router
{
public:
    DijkstraRouter(Link * tunnel, Network * network)
    :
        Router(tunnel, network)
    {
        type = DIJKSTRA;
    }

    virtual bool route();
    virtual NetPath search();
    virtual void print() const;
protected:
    virtual long getEdgeWeight(Link * link) const;
    void printPath(const NetPath & path) const; 
};

#endif // DIJKSTRAROUTER_H
