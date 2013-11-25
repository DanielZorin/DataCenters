#include "dijkstrarouter.h"

#include "common/link.h"
#include "common/network.h"

#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <algorithm>

bool DijkstraRouter::route()
{
    decrease();
    path = search();
    restore();
    return !path.empty() && pathCompliesPolicies(path);
}

NetPath DijkstraRouter::search()
{
    if ( !validateInput() )
    {
        printf("[RD]Wrong input\n");
        return NetPath();
    }

    if ( link->getFirst() == link->getSecond() )
        return NetPath();

    std::set<ElementWeight, WeightCompare> elementsToParse;
    std::map<Element * , Link *> incomingEdge;
    std::map<Element * , std::vector<Link *> > elementLinks;

    Links & links = network->getLinks();
    for ( Links::iterator i = links.begin(); i != links.end(); i++ )
    {
        Link * l = *i;
        if ( l->getFirst() != link->getFirst() )
            elementsToParse.insert(ElementWeight(l->getFirst(), LONG_MAX)); 
        if ( l->getSecond() != link->getFirst() )
            elementsToParse.insert(ElementWeight(l->getSecond(), LONG_MAX));

        elementLinks[l->getFirst()].push_back(l);
        elementLinks[l->getSecond()].push_back(l);
    }
    elementsToParse.insert(ElementWeight(link->getFirst(), 0));
    elementsToParse.insert(ElementWeight(link->getSecond(), LONG_MAX));
    Element * currentElement = link->getFirst();

    Link edge("dijkstraedge", 0);
    ElementWeight temp(0, -LONG_MAX);
    std::set<ElementWeight, WeightCompare>::iterator tempIter;
    long curWeight = 0;
    while ( currentElement != 0 && currentElement != link->getSecond() )
    {
        temp.element = currentElement;
        tempIter = elementsToParse.find(temp);
        assert(tempIter != elementsToParse.end());
        curWeight = tempIter->weight;
        elementsToParse.erase(tempIter);

        if ( elementLinks.find(currentElement) == elementLinks.end() )
            return NetPath();

        std::vector<Link *>& curLinks = elementLinks[currentElement];
        unsigned int size = curLinks.size();
        for(unsigned int index = 0; index < size; index++)
        {
            Link * cur = curLinks[index];
            Element * other = cur->getFirst() == currentElement ?
                cur->getSecond() : cur->getFirst();
            temp.element = other;
            temp.weight = LONG_MAX;
            tempIter = elementsToParse.find(temp);
            if ( tempIter != elementsToParse.end() )
            {
                edge.setCapacity(cur->getCapacity());
                edge.bindElements(currentElement, other);

                long weight = getEdgeWeight(&edge) + curWeight;
                if ( weight < tempIter->weight )
                {
                    temp.element = other;
                    temp.weight = weight;
                    elementsToParse.erase(tempIter);
                    elementsToParse.insert(temp);
                    incomingEdge[other] = cur;
                }
            }
            temp.weight = -LONG_MAX;
        }

        if (elementsToParse.begin()->weight != LONG_MAX) 
            currentElement = elementsToParse.begin()->element;
        else
            return NetPath();
    }

    if ( currentElement != link->getSecond() )
        return NetPath();

    Element * other = currentElement;
    NetPath result;
    while ( incomingEdge[currentElement]->getFirst() != link->getFirst() 
            && incomingEdge[currentElement]->getSecond() != link->getFirst() )
    {
        result.push_back(incomingEdge[currentElement]);
        other = incomingEdge[currentElement]->getFirst() == currentElement ?
            incomingEdge[currentElement]->getSecond() : incomingEdge[currentElement]->getFirst();
        result.push_back((NetworkingElement *)other);
        currentElement = other;
    }
    result.push_back(incomingEdge[other]);
    
    std::reverse(result.begin(), result.end());
    return result;
}

void DijkstraRouter::print() const
{
    printf("Constructed dijkstra path of length %d for link %s between %s and %s as follows:\n",
            path.size(),
            link->getName().c_str(),
            link->getFirst()->getName().c_str(),
            link->getSecond()->getName().c_str());
    printPath(path);
}

long DijkstraRouter::getEdgeWeight(Link * link) const
{
    if ( link->getSecond()->isSwitch() )
        return link->getMaxCapacity() - link->getCapacity() + 
            link->getSecond()->getMaxCapacity() - link->getSecond()->getCapacity();
    return link->getMaxCapacity() - link->getCapacity();
}

void DijkstraRouter::printPath(const NetPath & path) const
{
    if ( path.empty() )
        return;

    for ( NetPath::const_iterator i = path.begin(); i != path.end(); i++)
    {
        Element * element = *i;
        printf("%s:%lu", element->getName().c_str(), element->getID());
        if ( i + 1 != path.end() )
            printf(" -> ");
    }
    printf("\n");
}
