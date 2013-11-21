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
private:
    long getEdgeWeight(Link * link) const;
};

#endif // DIJKSTRAROUTER_H
