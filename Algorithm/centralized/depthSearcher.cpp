#include "depthSearcher.h"

#include "node.h"
#include "network.h"
#include "store.h"
#include "link.h"

#include "iostream"
using std::cerr;
using std::endl;

DepthSearcher::DepthSearcher(Network & n, const Elements & elements)
:
    network(n)
{
    for (Elements::iterator i = elements.begin(); i != elements.end(); i++)
    {
        Elements * currentElement = new Elements();
        currentElement->insert(*i);
        adjacentElements[*i] = currentElement;
    }

    lastIncreased = adjacentElements.end();
    hasBeenModified = true;

    cerr << "[DS] constructed search environment to look for " 
        << adjacentElements.size() << " elements connection" << endl;
}

DepthSearcher::~DepthSearcher()
{
    for(AdjacentElements::iterator i = adjacentElements.begin();
            i != adjacentElements.end(); i++)
    {
        delete (i->second);
    }
}

void DepthSearcher::increaseSearchSpace()
{
    if ( isExhausted() )
        return;

    if ( lastIncreased != adjacentElements.end() )
        lastIncreased++;
    else
    {
        lastIncreased = adjacentElements.begin();
        hasBeenModified = false;
    }

    Elements * elements = lastIncreased->second;
    Elements * newElements = new Elements();
    int size = elements->size();
    for ( Elements::iterator i = elements->begin(); i != elements->end(); i++)
    {
        Elements adjacent = getAdjacentElements(*i);
        newElements->insert(adjacent.begin(), adjacent.end());
    }
    adjacentElements[lastIncreased->first] = newElements;
    delete elements;
    int newSize = newElements->size();
    if ( newSize > size )
        hasBeenModified = true;
}

bool DepthSearcher::isExhausted()
{
    return !hasBeenModified && lastIncreased == adjacentElements.end();
}

Elements DepthSearcher::getAdjacentElements(Element * element)
{
    Elements result;
    result.insert(element);

    Links & links = network.getLinks();
    for ( Links::iterator l = links.begin(); l != links.end(); l++ )
    {
        Link * link = *l;
        Element * e = link->getAdjacentElement(element);
        if ( e != 0 )
            result.insert(e);
    }
    return result;
}

Elements DepthSearcher::intersect(const Elements & core, const Elements & projection)
{
    Elements result;
    for ( Elements::iterator i = core.begin(); i != core.end(); i++ )
        if ( projection.find(*i) != projection.end() )
            result.insert(*i);

    return result;
}

Elements DepthSearcher::getElementCandidates()
{
    Elements result;

    if ( isExhausted() )
        return result;

    result = *(adjacentElements.begin()->second);

    for ( AdjacentElements::iterator i = adjacentElements.begin(); i != adjacentElements.end(); i++ )
    {
        result = intersect(result, *(i->second));
    }

    return result;
}

