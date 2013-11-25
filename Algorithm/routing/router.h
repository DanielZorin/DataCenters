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
        DIJKSTRA,
        K_SHORTEST_PATHS
    };

    struct ElementWeight
    {
        Element * element;
        long weight;

        ElementWeight(Element * e, long w)
        :
            element(e),
            weight(w)
        {}

        ElementWeight()
        :
            element(0),
            weight(0)
        {}

        bool operator==(const ElementWeight & other) const { return element == other.element; }
    };

    struct WeightCompare
    {
        bool operator() (const ElementWeight & e1, const ElementWeight & e2) const
        {
            if ( e1.element == e2.element )
                return false;
            if ( e1.weight == e2.weight )
                return e1.element < e2.element;
            return e1.weight < e2.weight;
        }
    };

public:
    Router(Link * virtualLink, Network * net)
    :
        type(NONE),
        link(virtualLink),
        network(net)
    {}

    virtual ~Router() 
    {}
    virtual bool route() = 0;
    bool validateInput() const
    {
        return link != 0 && network != 0; 
    }

    virtual NetPath getPath() const { return path; }
    SearchAlgorithm algorithm() const { return type; }
    virtual bool pathCompliesPolicies(NetPath & path) const { return true; }

protected:
    void decrease();
    void restore();

protected:
    SearchAlgorithm type;
    Link * link;
    Network * network;
    NetPath path;
    Links omittedLinks;
    Switches omittedSwitches;
};

#endif // ROUTER_H
