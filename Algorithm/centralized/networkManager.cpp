#include "networkManager.h"

#include "network.h"
#include "node.h"
#include "store.h"
#include "link.h"
#include "assignment.h"
#include "decentralized/virtualLinkRouter.h"

#include <iostream>

using std::cerr;
using std::endl;

NetworkManager::NetworkManager(Network & n)
:
   network(n)
{

}

void NetworkManager::setSearchSpace(const Nodes & nodes)
{
    adjacentElements.clear();
    rejectedNodes.clear();
    rejectedStores.clear();

    for ( Nodes::iterator i = nodes.begin(); i != nodes.end(); i++)
    {
        Node * node = *i;
        Elements elements;
        elements.insert(node);
        adjacentElements.insert(elements);
    }

    cerr << "[NM]Constructed depth search environment to look for "
        << adjacentElements.size() << " elements connection" << endl;
}

Elements NetworkManager::getElementCandidates()
{
    Elements result;

    if ( adjacentElements.empty() )
        return result;

    result = *(adjacentElements.begin());

    for ( AdjacentElements::iterator i = adjacentElements.begin(); i != adjacentElements.end(); i++ )
    {
        result = intersect(result, *i);
    }

    return result;
}

Elements NetworkManager::intersect(const Elements & first, const Elements & second)
{
    Elements result;
    for ( Elements::iterator i = first.begin(); i != first.end(); i++ )
        if ( second.find(*i) != second.end() )
            result.insert(*i);

    return result;
}

void NetworkManager::increaseSearchSpace()
{
    AdjacentElements newAE;
    for ( AdjacentElements::iterator i = adjacentElements.begin(); i != adjacentElements.end(); i++ )
    {
        Elements const& elements = *i;
        Elements newElements;
        for ( Elements::iterator e = elements.begin(); e != elements.end(); e++ )
        {
            Elements adjacent = getAdjacentElements(*e);
            newElements.insert(adjacent.begin(), adjacent.end());
        }
        newAE.insert(newElements);
    }

    adjacentElements = newAE;
}

Elements NetworkManager::getAdjacentElements(Element * element)
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

Nodes NetworkManager::getNodeCandidates()
{
    Nodes result;

    Elements elements = getElementCandidates();

    while ( elements.empty() )
    {
        increaseSearchSpace();
        elements = getElementCandidates();
    }

    for ( Elements::iterator i = elements.begin(); i != elements.end(); i++ )
    {
        Element * element = *i;
        if ( ! element->isNode() )
            continue;

        Node * node = (Node *) element;
        if ( rejectedNodes.find(node) != rejectedNodes.end() )
            continue;

        result.insert(node);
    }

    rejectedNodes.insert(result.begin(), result.end());
    cerr << "[NM]\tPrepared " << result.size() << " candidates" << endl;
    return result;
}

Stores NetworkManager::getStoreCandidates()
{
    Stores result;

    Elements elements = getElementCandidates();
    while ( elements.empty() )
    {
        increaseSearchSpace();
        elements = getElementCandidates();
    }

    while ( !elements.empty() )
    {
        for ( Elements::iterator i = elements.begin(); i != elements.end(); i++ )
        {
            Element * element = *i;
            if ( ! element->isStore() )
                continue;

            Store * store = (Store *) element;
            
            if ( rejectedStores.find(store) != rejectedStores.end() )
                continue;
            
            result.insert(store);
        }

        if ( !result.empty()  )
            break;

        increaseSearchSpace();
        elements = getElementCandidates();
    }

    rejectedStores.insert(result.begin(), result.end());
    cerr << "[NM]\tPrepared " << result.size() << " candidates" << endl;
    return result;
}

Algorithm::Result NetworkManager::buildPath(Element * from, Element * to, Link * vlink, Assignment * assignment)
{
    if ( from == to )
        return Algorithm::SUCCESS;

    Link * dummy = new Link("dummy", vlink->getCapacity(), vlink->getMaxCapacity());
    NetPath path = VirtualLinkRouter::routeDejkstra(dummy, &network);
    delete dummy;

    if ( path.empty() )
        return Algorithm::FAILURE;

    addAssignment(vlink, path);
    assignment->AddAssignment(vlink, path);
    return Algorithm::SUCCESS;
}

void NetworkManager::cleanUpLinks(Links & links, Assignment * assignment)
{
    for ( Links::iterator l = links.begin(); l != links.end(); l++ )
    {
        NetPath path = assignment->GetAssignment(*l);
        if ( !path.empty() )
            removeAssignment(*l, path);

        assignment->RemoveAssignment(*l);
    }
}

void NetworkManager::removeAssignment(Link * vlink, NetPath & netPath)
{
    for ( NetPath::iterator i = netPath.begin(); i != netPath.end(); i++ )
        (*i)->RemoveAssignment(vlink);
}

void NetworkManager::addAssignment(Link * vlink, NetPath & netPath)
{
    for ( NetPath::iterator i = netPath.begin(); i != netPath.end(); i++ )
        (*i)->assign(*vlink);
}
