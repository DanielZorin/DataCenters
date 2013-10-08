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
    currentElement = adjacentElements.begin();
    hasBeenModified = false;

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

    if ( currentElement == adjacentElements.end() )
        currentElement = adjacentElements.begin();

    Elements * elements = currentElement->second;
    int size = elements->size();
    Elements copy = *elements;
    for ( Elements::iterator i = copy.begin(); i != copy.end(); i++)
    {
        Elements adjacent = getAdjacentElements(*i);
        elements->insert(adjacent.begin(), adjacent.end());
    }
    int newSize = elements->size();

    if ( newSize > size )
    {
        hasBeenModified = true;
        cerr << "[DS]\tElements has been modified" << endl;
    }

    currentElement++;
}

bool DepthSearcher::isExhausted()
{
    return !hasBeenModified && currentElement == adjacentElements.end();
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

