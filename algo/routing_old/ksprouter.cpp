#include "ksprouter.h"

#include "common/network.h"
#include "common/link.h"

#include <stdio.h>

KSPRouter::KSPRouter(Link * vl, Network * net, int d)
:
    DijkstraRouter(vl, net),
    depth(d)
{
    type = K_SHORTEST_PATHS;
}

bool KSPRouter::route()
{
    if ( !validateInput() )
        return false;

    decrease();
    path = search();
    restore();

    std::vector<NetPath> compliedPathSet;
    for ( std::vector<NetPath>::iterator i = pathSet.begin(); i != pathSet.end(); i++)
        if ( pathCompliesPolicies(*i) )
            compliedPathSet.push_back(*i);

    pathSet = compliedPathSet;

    return !path.empty() && pathCompliesPolicies(path);
}

NetPath KSPRouter::search()
{
    NetPath shortest = DijkstraRouter::search();
    if ( shortest.empty() )
    {
        return NetPath();
    }

    pathSet.push_back(shortest);

    Links & links = network->getLinks();

    unsigned pathsFound = 1;
    long currentWeight = pathWeight(shortest);
    bool isNewPathFound = true;
    while ( isNewPathFound && pathsFound < depth)
    {
        NetPath candidate = shortest;
        isNewPathFound = false;
        bool isNewCandidateFound = false;

        Link * linkToRemove = 0;
        for ( NetPath::iterator i = shortest.begin(); i != shortest.end(); i++ )
        {
            Element * e = *i;
            if ( e->isLink() && links.find((Link *)(e)) != links.end() )
            {
                Link * l = (Link *)e;
                links.erase(l);
                NetPath newPath = DijkstraRouter::search();
                if ( newPath.size() == shortest.size() )
                {
                    isNewPathFound = true;
                    pathSet.push_back(newPath);
                    pathsFound++;
                    long weight = pathWeight(newPath);
                    if ( weight > currentWeight )
                    {
                        isNewCandidateFound = true;
                        currentWeight = weight;
                        candidate = newPath;
                    }
                    if ( linkToRemove == 0 )
                        linkToRemove = l;
                }
                links.insert(l);

                if ( pathsFound == depth )
                    break;
            }
        }

        if ( isNewCandidateFound )
            shortest = candidate;

        if ( isNewPathFound && pathsFound != depth )
        {
            linkToRemove->RemoveAssignment(link);
            links.erase(linkToRemove);
            omittedLinks.insert(linkToRemove);
        }
    }

    return shortest;
}

void KSPRouter::print() const
{
    printf("Constructed shortest path of length %d for link %s between %s and %s as follows:\n",
            path.size(),
            link->getName().c_str(),
            link->getFirst()->getName().c_str(),
            link->getSecond()->getName().c_str());
    printPath(path);

    if ( pathSet.size() == 1 )
        return;

    printf("All %d pathes of same length:\n", pathSet.size());
    for(std::vector<NetPath>::const_iterator i = pathSet.begin(); i != pathSet.end(); i++)
        printPath(*i);
}

long KSPRouter::pathWeight(NetPath & path) const
{
    long result = 0;
    for ( NetPath::iterator i = path.begin(); i != path.end(); i++ )
        result += (*i)->getCapacity();
    return result;
}
