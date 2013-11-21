#ifndef ROUTER_H
#define ROUTER_H

#include "common/publicdefs.h"

class Router
{
public:
    enum SearchAlgorithm
    {
        NONE = 0,
        TEST,
        DEJKSTRA,
        K_SHORTEST_PATHS
    };
public:
    Router(Link * virtualLink, Network * net)
    :
        type(NONE),
        link(virtualLink),
        network(net)
    {}

    virtual ~Router() {}
    virtual bool route() = 0;
    SearchAlgorithm algorithm() const { return type; }
protected:
    SearchAlgorithm type;
    Link * link;
    Network * network;
};

#endif // ROUTER_H
