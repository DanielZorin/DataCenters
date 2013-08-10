#include "virtualLinkRouter.h"
#include <map>
#include <algorithm>
#include <limits.h>
#include <stdio.h>
#include "link.h"
#include "switch.h"
#include "network.h"
#include "criteria.h"

// number of links per element, used when building elementLinks is dijkstra algorithm.
// actual amount of links can be greater but it will cause vector reallocation
const unsigned int NLinks = 10;

NetPath VirtualLinkRouter::route(VirtualLink * virtualLink, Network * network, SearchPathAlgorithm algorithm, std::vector<NetPath> * pathStorage)
{
    if ( algorithm == K_SHORTEST_PATHS )
        return routeKShortestPaths(virtualLink, network);

    if ( algorithm == DEJKSTRA )
        return routeDejkstra(virtualLink, network);

    // for random algorithm
    if ( algorithm == K_SHORTEST_PATHS_ALL )
        return routeKShortestPathsALL(virtualLink, network, pathStorage);

    return NetPath();
}

// choose the next element to parse during the dejkstra algorithm
Element* chooseNext(std::map<Element * , long>& elementWeight, std::set<Element *>& elementsToParse) {
    Element* nextElement = NULL;
    long minWeight = LONG_MAX;
    std::set<Element *>::iterator it = elementsToParse.begin();
    std::set<Element *>::iterator itEnd = elementsToParse.end();
    for ( ; it != itEnd; ++it )
    {
        if ( elementWeight[*it] < minWeight )
        {
            minWeight = (*it)->getCapacity();
            nextElement = *it;
        }
    }
    return nextElement;
}

NetPath VirtualLinkRouter::searchPathDejkstra(VirtualLink * virtualLink, Network * network, SearchPathAlgorithm algorithm)
{
    if ( virtualLink->getFirst() == virtualLink->getSecond() )
        return NetPath();
    
    // local variables
    std::map<Element * , long> elementWeight;
    std::map<Element * , Link*> incomingEdge;
    std::map<Element * , std::vector<Link *> > elementLinks;
    std::set<Element *> elementsToParse;

    // initializing parameters
    Links::iterator it = network->getLinks().begin();
    Links::iterator itEnd = network->getLinks().end();
    std::vector<Link *> * vecLinks = NULL;
    for ( ; it != itEnd; ++it )
    {
        elementWeight[(*it)->getFirst()] = LONG_MAX; // equal to inf
        elementWeight[(*it)->getSecond()] = LONG_MAX;
        vecLinks = &elementLinks[(*it)->getFirst()];
        if (vecLinks->capacity() < NLinks) vecLinks->reserve(NLinks);
        vecLinks->push_back(*it);
        vecLinks = &elementLinks[(*it)->getSecond()];
        if (vecLinks->capacity() < NLinks) vecLinks->reserve(NLinks);
        vecLinks->push_back(*it);

        // can go only to the switch, not to node or store
        if ( (*it)->getFirst()->isSwitch() )
            elementsToParse.insert((*it)->getFirst());
        if ( (*it)->getSecond()->isSwitch() )
            elementsToParse.insert((*it)->getSecond());
    }

    elementsToParse.insert(virtualLink->getFirst());
    elementsToParse.insert(virtualLink->getSecond());

    elementWeight[virtualLink->getFirst()] = 0l;
    elementWeight[virtualLink->getSecond()] = LONG_MAX;
    Element * currentElement = virtualLink->getFirst();

    Link edge("dijkstra edge", 0);
    // algorithm itself
    while ( currentElement != NULL && currentElement != virtualLink->getSecond() )
    {
        elementsToParse.erase(currentElement);
        
        // going through all neighbors of current element,
        // parsing their weight and choosing the element with the
        // lowest weight
        if ( elementLinks.find(currentElement) == elementLinks.end() )
        {
            return NetPath(); // No links assosiated with element
        }

//        it = elementLinks[currentElement].begin();
//        itEnd = elementLinks[currentElement].end();
        std::vector<Link *>& curLinks = elementLinks[currentElement];
        unsigned int sz = curLinks.size();
//        for ( ; it != itEnd; ++it )
        for(unsigned int index = 0; index < sz; ++ index)
        {
            Link * cur = curLinks[index];
            Element * other = cur->getFirst() == currentElement ? 
                cur->getSecond() : cur->getFirst();
            if ( elementsToParse.find(other) != elementsToParse.end() )
            {
                edge.setCapacity(cur->getCapacity());
                edge.bindElements(currentElement, other);

                // weight of reaching the next element from current element
                long weight = getEdgeWeigth(edge, network, algorithm) + elementWeight[currentElement];
                if ( weight < elementWeight[other] )
                {
                    elementWeight[other] = weight;
                    incomingEdge[other] = cur;
                }
            }
        }
        
        currentElement = chooseNext(elementWeight, elementsToParse);
    }

    if ( currentElement != virtualLink->getSecond() )
        return NetPath(); // no way from one element to another

    // retrieving the way
    NetPath answer;

    Element * other = currentElement; // for parsing results with just one edge
    while ( incomingEdge[currentElement]->getFirst() != virtualLink->getFirst() 
        && incomingEdge[currentElement]->getSecond() != virtualLink->getFirst() )
    {
        answer.push_back(incomingEdge[currentElement]);
        other = incomingEdge[currentElement]->getFirst() == currentElement ?
            incomingEdge[currentElement]->getSecond() : incomingEdge[currentElement]->getFirst();
        answer.push_back(static_cast<NetworkingElement *>(other));
        currentElement = other;
    }
    answer.push_back(incomingEdge[other]);
    
    // this step may be skiped, but doing for sure
    std::reverse(answer.begin(), answer.end());
    return answer;
}

NetPath VirtualLinkRouter::routeKShortestPaths(VirtualLink * virtualLink, Network * network)
{   
    // links and switches that would be removed from the network,
    // they should be restored after algorithm's finish
    Links removedLinks;
    Switches removedSwitches;

    // first, create the graph with decreased capacities
    decreaseCapacities(virtualLink, network, &removedLinks, &removedSwitches);

    // Yen's algorithm
    NetPath shortest = searchPathDejkstra(virtualLink, network, K_SHORTEST_PATHS);
    if ( shortest.size() == 0 )
    {
        restoreCapacities(virtualLink, network, &removedLinks, &removedSwitches);
        return NetPath(); // no path found!
    }

    Links& links = network->getLinks();

    unsigned pathsFound = 1;
    long pathWeight = calculateKShortestPathWeight(shortest);
    bool isNewPathFound = true;
    while ( isNewPathFound && pathsFound < Criteria::kShortestPathDepth() )
    {
        NetPath candidate = shortest;
        NetPath::iterator it = shortest.begin();
        NetPath::iterator itEnd = shortest.end();
        isNewPathFound = false;
        bool isNewCandidateFound = false;

        Link * linkToRemove = NULL;
        for ( ; it != itEnd; ++it )
        {
            if ( (*it)->isLink() && links.find(static_cast<Link*>(*it)) != links.end() )
            {
                // removing link and trying dejkstra
                links.erase(links.find(static_cast<Link*>(*it)));
                NetPath newPath = searchPathDejkstra(virtualLink, network, K_SHORTEST_PATHS);
                if ( newPath.size() == shortest.size() )
                {
                    isNewPathFound = true;
                    ++pathsFound;
                    long weight = calculateKShortestPathWeight(newPath);
                    if ( weight > pathWeight )
                    {
                        isNewCandidateFound = true;
                        linkToRemove = static_cast<Link*>(*it);
                        pathWeight = weight;
                        candidate = newPath;
                        if ( pathsFound == Criteria::kShortestPathDepth() )
                            break;
                    }
                    if ( linkToRemove == NULL )
                        linkToRemove = static_cast<Link*>(*it); // to avoid the situation with removing NULL-link
                }
                links.insert(static_cast<Link*>(*it)); // inserting link again
            }
        }

        if ( isNewCandidateFound )
            shortest = candidate;

        if ( isNewPathFound && pathsFound != Criteria::kShortestPathDepth() )
        {
            // restoring the capacity of link being removed
            linkToRemove->RemoveAssignment(virtualLink);
            links.erase(links.find(linkToRemove));
            removedLinks.insert(linkToRemove);
        }
    }

    // restoring removed capacities
    restoreCapacities(virtualLink, network, &removedLinks, &removedSwitches);

    return shortest;
}

NetPath VirtualLinkRouter::routeKShortestPathsALL(VirtualLink * virtualLink, Network * network, std::vector<NetPath> * pathStorage)
{   
    // links and switches that would be removed from the network,
    // they should be restored after algorithm's finish
    Links removedLinks;
    Switches removedSwitches;

    // first, create the graph with decreased capacities
    decreaseCapacities(virtualLink, network, &removedLinks, &removedSwitches);

    // Yen's algorithm
    NetPath shortest = searchPathDejkstra(virtualLink, network, K_SHORTEST_PATHS);
    if ( shortest.size() == 0 )
    {
        restoreCapacities(virtualLink, network, &removedLinks, &removedSwitches);
        return NetPath(); // no path found!
    }
    pathStorage->push_back(shortest);

    Links& links = network->getLinks();

    unsigned pathsFound = 1;
    long pathWeight = calculateKShortestPathWeight(shortest);
    bool isNewPathFound = true;
    while ( isNewPathFound && pathsFound < Criteria::kShortestPathDepth() )
    {
        NetPath candidate = shortest;
        NetPath::iterator it = shortest.begin();
        NetPath::iterator itEnd = shortest.end();
        isNewPathFound = false;
        bool isNewCandidateFound = false;

        Link * linkToRemove = NULL;
        for ( ; it != itEnd; ++it )
        {
            if ( (*it)->isLink() && links.find(static_cast<Link*>(*it)) != links.end() )
            {
                // removing link and trying dejkstra
                links.erase(links.find(static_cast<Link*>(*it)));
                NetPath newPath = searchPathDejkstra(virtualLink, network, K_SHORTEST_PATHS);
                if ( newPath.size() == shortest.size() )
                {
                    isNewPathFound = true;
                    pathStorage->push_back(newPath);
                    ++pathsFound;
                    long weight = calculateKShortestPathWeight(newPath);
                    if ( weight > pathWeight )
                    {
                        isNewCandidateFound = true;
                        linkToRemove = static_cast<Link*>(*it);
                        pathWeight = weight;
                        candidate = newPath;
                        if ( pathsFound == Criteria::kShortestPathDepth() )
                            break;
                    }
                    if ( linkToRemove == NULL )
                        linkToRemove = static_cast<Link*>(*it); // to avoid the situation with removing NULL-link
                }
                links.insert(static_cast<Link*>(*it)); // inserting link again
            }
        }

        if ( isNewCandidateFound )
            shortest = candidate;

        if ( isNewPathFound && pathsFound != Criteria::kShortestPathDepth() )
        {
            // restoring the capacity of link being removed
            linkToRemove->RemoveAssignment(virtualLink);
            links.erase(links.find(linkToRemove));
            removedLinks.insert(linkToRemove);
        }
    }

    // restoring removed capacities
    restoreCapacities(virtualLink, network, &removedLinks, &removedSwitches);

    return shortest;
}

NetPath VirtualLinkRouter::routeDejkstra(VirtualLink * virtualLink, Network * network)
{
    // links and switches that would be removed from the network,
    // they should be restored after algorithm's finish
    Links removedLinks;
    Switches removedSwitches;

    // first, create the graph with decreased capacities
    decreaseCapacities(virtualLink, network, &removedLinks, &removedSwitches);

    NetPath path = searchPathDejkstra(virtualLink, network, DEJKSTRA);

    restoreCapacities(virtualLink, network, &removedLinks, &removedSwitches);

    return path;
}

long VirtualLinkRouter::getEdgeWeigth(Link& link, Network * network, SearchPathAlgorithm algorithm)
{
    if ( algorithm == K_SHORTEST_PATHS )
        return 1;

    if ( algorithm == DEJKSTRA )
    {
        if ( link.getSecond()->isSwitch() )
            // the less capacity - the less weight - the more chance to choose it
            return (long)(link.getMaxCapacity() - link.getCapacity() + link.getSecond()->getMaxCapacity() - link.getSecond()->getCapacity());
        return (long)link.getMaxCapacity() - (long)link.getCapacity();
    }

    return link.getCapacity();
}

long VirtualLinkRouter::calculateKShortestPathWeight(NetPath& path)
{
    long result = 0l;
    NetPath::iterator it = path.begin();
    NetPath::iterator itEnd = path.end();
    for ( ; it != itEnd; ++it )
    {
        result += (*it)->getCapacity();
    }

    return result;
}

void VirtualLinkRouter::decreaseCapacities(VirtualLink * virtualLink, Network * network, Links * removedLinks, 
        Switches * removedSwitches)
{
    // forming the set od switches to remove
    Switches::iterator swIt = network->getSwitches().begin();
    Switches::iterator swItEnd = network->getSwitches().end();
    for ( ; swIt != swItEnd; ++swIt )
    {
        if ( (*swIt)->getCapacity() >= virtualLink->getCapacity() )
            (*swIt)->assign(*virtualLink); // this just decrease capacity
        else
            removedSwitches->insert(*swIt);
    }

    // forming the set od links to remove
    // Links with switches already removed (as their vertex), are removed
    Links::iterator lIt = network->getLinks().begin();
    Links::iterator lItEnd = network->getLinks().end();
    for ( ; lIt != lItEnd; ++lIt )
    {
        if ( removedSwitches->find(static_cast<Switch*>((*lIt)->getFirst())) != removedSwitches->end() ||
             removedSwitches->find(static_cast<Switch*>((*lIt)->getSecond())) != removedSwitches->end() )
             removedLinks->insert(*lIt);
        else
        {
            if ( (*lIt)->getCapacity() >= virtualLink->getCapacity() )
                (*lIt)->assign(*virtualLink); // this just decrease capacity
            else
                removedLinks->insert(*lIt);
        }
    }

    // removing links and switches
    swIt = removedSwitches->begin();
    swItEnd = removedSwitches->end();
    Switches& switches = network->getSwitches();
    for ( ; swIt != swItEnd; ++swIt )
        switches.erase(*swIt);

    lIt = removedLinks->begin();
    lItEnd = removedLinks->end();
    Links& links = network->getLinks();
    for ( ; lIt != lItEnd; ++lIt )
        links.erase(*lIt);
}

void VirtualLinkRouter::restoreCapacities(VirtualLink * virtualLink, Network * network,
                                          Links * removedLinks, Switches * removedSwitches)
{
    // first, restoring the capacities values
    Switches::iterator swIt = network->getSwitches().begin();
    Switches::iterator swItEnd = network->getSwitches().end();
    for ( ; swIt != swItEnd; ++swIt )
        (*swIt)->RemoveAssignment(virtualLink); // this just insrease capacity

    // forming the set od links to remove
    // Links with switches already removed (as their vertex), are removed
    Links::iterator lIt = network->getLinks().begin();
    Links::iterator lItEnd = network->getLinks().end();
    for ( ; lIt != lItEnd; ++lIt )
        (*lIt)->RemoveAssignment(virtualLink); // this just increase capacity

    // restoring removed links and switches
    swIt = removedSwitches->begin();
    swItEnd = removedSwitches->end();
    Switches& switches = network->getSwitches();
    for ( ; swIt != swItEnd; ++swIt )
        switches.insert(*swIt);

    lIt = removedLinks->begin();
    lItEnd = removedLinks->end();
    Links& links = network->getLinks();
    for ( ; lIt != lItEnd; ++lIt )
        links.insert(*lIt);
}