#include "virtualLinkRouter.h"
#include <map>
#include <algorithm>
#include "link.h"
#include "switch.h"
#include "network.h"
#include "criteria.h"

NetPath VirtualLinkRouter::route(VirtualLink * virtualLink, Network * network, SearchPathAlgorithm algorithm)
{
    if ( algorithm == K_SHORTEST_PATHS )
        return routeKShortestPaths(virtualLink, network);

    if ( algorithm == DEJKSTRA )
        return routeDejkstra(virtualLink, network);

    return NetPath();
}

NetPath VirtualLinkRouter::searchPathDejkstra(VirtualLink * virtualLink, Network * network, SearchPathAlgorithm algorithm)
{
    if ( virtualLink->getFirst() == virtualLink->getSecond() )
        return NetPath(1, static_cast<NetworkingElement * >(virtualLink->getFirst()));
    
    // local variables
    std::map<Element * , long> elementWeight;
    std::map<Element * , Link*> incomingEdge;
    std::map<Element * , Links> elementLinks;
    std::map<Element * , bool> isParsed;

    // initializing parameters
    Links::iterator it = network->getLinks().begin();
    Links::iterator itEnd = network->getLinks().end();
    for ( ; it != itEnd; ++it )
    {
        elementWeight[(*it)->getFirst()] = LONG_MAX; // equal to inf
        elementWeight[(*it)->getSecond()] = LONG_MAX;
        elementLinks[(*it)->getFirst()].insert(*it);
        elementLinks[(*it)->getSecond()].insert(*it);
        isParsed[(*it)->getFirst()] = false;
        isParsed[(*it)->getSecond()] = false;
    }

    elementWeight[virtualLink->getFirst()] = 0l;
    Element * currentElement = virtualLink->getFirst();

    // algorithm itself
    while ( currentElement != virtualLink->getSecond() )
    {
        long minimalWeight = LONG_MAX;
        Element * minimalWeightNeighbor = NULL;
        
        // going through all neighbors of current element,
        // parsing their weight and choosing the element with the
        // lowest weight
        if ( elementLinks.find(currentElement) == elementLinks.end() )
        {
            printf("No links assosiated with element\n");
            return NetPath(); // No links assosiated with element
        }

        it = elementLinks[currentElement].begin();
        itEnd = elementLinks[currentElement].end();
        for ( ; it != itEnd; ++it )
        {
            Element * other = (*it)->getFirst() == currentElement ? 
                (*it)->getSecond() : (*it)->getFirst();
            if ( !isParsed[other] )
            {
                Link edge("dijkstra_edge", (*it)->getCapacity());
                edge.bindElements(currentElement, other);

                // weight of reaching the next element from current element
                long weight = getEdgeWeigth(edge, network, algorithm) + elementWeight[currentElement];
                if ( weight < elementWeight[other] || elementWeight[other] == -1 )
                {
                    elementWeight[other] = weight;
                    incomingEdge[other] = (*it);
                }
                
                // checking the vertex to parse next
                if ( elementWeight[other] < minimalWeight || minimalWeight == -1 )
                {
                    minimalWeight = elementWeight[other];
                    minimalWeightNeighbor = other;
                }
            }
        }

        isParsed[currentElement] = true;
        currentElement = minimalWeightNeighbor;
        if ( minimalWeightNeighbor == NULL )
            break;
    }

    if ( currentElement != virtualLink->getSecond() )
        return NetPath(); // no way from one element to another

    // retrieving the way
    NetPath answer;
    printf("Found path for link %s:\n", virtualLink->getName().c_str());

    Element * other;
    while ( incomingEdge[currentElement]->getFirst() != virtualLink->getFirst() 
        && incomingEdge[currentElement]->getSecond() != virtualLink->getFirst() )
    {
        answer.push_back(incomingEdge[currentElement]);
        other = incomingEdge[currentElement]->getFirst() == currentElement ?
            incomingEdge[currentElement]->getSecond() : incomingEdge[currentElement]->getFirst();
        answer.push_back(static_cast<NetworkingElement *>(other));
        //printf("    %s -> %s -> %s\n", currentElement->getName().c_str(),
        //    incomingEdge[currentElement]->getName().c_str(), other->getName().c_str());
        currentElement = other;
    }
    answer.push_back(incomingEdge[other]);
    //printf("    %s -> %s -> %s\n", currentElement->getName().c_str(),
    //        incomingEdge[currentElement]->getName().c_str(), virtualLink->getFirst()->getName().c_str());

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
            return - (long)(link.getCapacity() + link.getSecond()->getCapacity());
        return - (long)link.getCapacity();
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