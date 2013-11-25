#ifndef KSPROUTER_H
#define KSPROUTER_H

#include "dijkstrarouter.h"

class KSPRouter : public DijkstraRouter
{
public:
    KSPRouter(Link * virtualLink, Network * net, int depth = 5);
    virtual ~KSPRouter() {}

    std::vector<NetPath> getAllPathes() const { return pathSet; }

    virtual bool route();
    virtual NetPath search();
    virtual long getEdgeWeight(Link *) const { return 1; }
protected:
    long pathWeight(NetPath & path) const;
private:
    int depth;
    std::vector<NetPath> pathSet;
};

#endif // KSPROUTER_H
