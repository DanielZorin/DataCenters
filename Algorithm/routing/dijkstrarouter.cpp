#include "dijkstrarouter.h"

#include "common/link.h"
#include "common/network.h"

#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <algorithm>

bool DijkstraRouter::route()
{
    if ( !Router::route() )
    {
        printf("[RD]Wrong input\n");
        return false;
    }

    if ( link->getFirst() == link->getSecond() )
        return false;

    std::set<ElementWeight, WeightCompare> elementsToParse;
    std::map<Element * , Link*> incomingEdge;
    std::map<Element * , std::vector<Link *> > elementLinks;

    Links::iterator it = network->getLinks().begin();
    Links::iterator itEnd = network->getLinks().end();
    std::vector<Link *> * vecLinks = NULL;
    for ( ; it != itEnd; ++it )
    {
        if ((*it)->getFirst() != link->getFirst())
            elementsToParse.insert(ElementWeight((*it)->getFirst(), LONG_MAX)); 
        if ((*it)->getSecond() != link->getFirst())
            elementsToParse.insert(ElementWeight((*it)->getSecond(), LONG_MAX));

        vecLinks = &elementLinks[(*it)->getFirst()];
        vecLinks->push_back(*it);
        vecLinks = &elementLinks[(*it)->getSecond()];
        vecLinks->push_back(*it);
    }
    elementsToParse.insert(ElementWeight(link->getFirst(), 0));
    elementsToParse.insert(ElementWeight(link->getSecond(), LONG_MAX));
    Element * currentElement = link->getFirst();

    Link edge("dijkstra edge", 0);
    ElementWeight temp(NULL, -LONG_MAX);
    std::set<ElementWeight, WeightCompare>::iterator tempIter;
    long curWeight = 0;
    while ( currentElement != NULL && currentElement != link->getSecond() )
    {
        temp.element = currentElement;
        tempIter = elementsToParse.find(temp);
        assert(tempIter != elementsToParse.end());
        curWeight = tempIter->weight;
        elementsToParse.erase(tempIter);

        if ( elementLinks.find(currentElement) == elementLinks.end() )
            return false;

        std::vector<Link *>& curLinks = elementLinks[currentElement];
        unsigned int sz = curLinks.size();
        for(unsigned int index = 0; index < sz; ++ index)
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
            return false;
    }

    if ( currentElement != link->getSecond() )
        return false;

    Element * other = currentElement;
    while ( incomingEdge[currentElement]->getFirst() != link->getFirst() 
        && incomingEdge[currentElement]->getSecond() != link->getFirst() )
    {
        path.push_back(incomingEdge[currentElement]);
        other = incomingEdge[currentElement]->getFirst() == currentElement ?
            incomingEdge[currentElement]->getSecond() : incomingEdge[currentElement]->getFirst();
        path.push_back(static_cast<NetworkingElement *>(other));
        currentElement = other;
    }
    path.push_back(incomingEdge[other]);
    
    std::reverse(path.begin(), path.end());
    return true;
}

long DijkstraRouter::getEdgeWeight(Link * link) const
{
    if ( link->getSecond()->isSwitch() )
        return link->getMaxCapacity() - link->getCapacity() + 
            link->getSecond()->getMaxCapacity() - link->getSecond()->getCapacity();
    return link->getMaxCapacity() - link->getCapacity();
}
