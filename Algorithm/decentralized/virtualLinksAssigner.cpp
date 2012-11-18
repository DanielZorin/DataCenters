#include "virtualLinksAssigner.h"

#include "algorithm.h"
#include "assignment.h"
#include "criteria.h"
#include "network.h"
#include "node.h"
#include "store.h"
#include "virtualLinkRouter.h"

#include <vector>
#include <algorithm>

VirtualLinksAssigner::~VirtualLinksAssigner()
{
}

// Functions used by sort algorithm to campare resource's weights.
bool requestVirtualLinksCompare(Request::VirtualLinks* vl1, Request::VirtualLinks* vl2)
{
    return Criteria::requestVirtualLinksWeight(vl1) < Criteria::requestVirtualLinksWeight(vl2);
}
bool virtualLinksCompare(Link * vl1, Link * vl2)
{
    return Criteria::virtualLinkWeight(vl1) < Criteria::virtualLinkWeight(vl2);
}

Requests VirtualLinksAssigner::PerformAssignment(Requests& requests)
{
    Requests assignedRequests; // the result set

    Requests::iterator it = requests.begin();
    Requests::iterator itEnd = requests.end();

    // generate the assosiative set of links
    RequestsVirtualLinks requestsVirtualLinks;

    // form the vector from the set to have an ability to sort it
    std::vector<Request::VirtualLinks * > virtualLinks;
    virtualLinks.reserve(requests.size());
    for ( ; it != itEnd; ++it )
    {
        requestsVirtualLinks[&(*it)->getVirtualLinks()] = (*it);
        virtualLinks.push_back(&(*it)->getVirtualLinks());
    }

    std::sort(virtualLinks.begin(), virtualLinks.end(), requestVirtualLinksCompare);

    for ( unsigned requestIndex = 0; requestIndex < virtualLinks.size(); ++requestIndex )
    {
        Request::VirtualLinks * vl = virtualLinks[requestIndex];
        // generate new assignment for the chosen virtual machine
        Assignment* reqAssignment = new Assignment();
        requestAssignment[requestsVirtualLinks[vl]] = reqAssignment;

        bool result = assignOneRequest(vl, reqAssignment, requestsVirtualLinks[vl]);
        if ( result ) // virtual machine assigned successfully 
            assignedRequests.insert(requestsVirtualLinks[vl]);
        else
        {
            requestAssignment.erase(requestsVirtualLinks[vl]);
            delete reqAssignment;
        }
    }

    return assignedRequests;
}

bool VirtualLinksAssigner::assignOneRequest(Request::VirtualLinks * virtualLinks,
                                            Assignment* reqAssignment, Request* req)
{
    // form the vector from the set to have an ability to sort it
    std::vector<Link * > virtualLinksVec(virtualLinks->begin(), virtualLinks->end());
    std::sort(virtualLinksVec.begin(), virtualLinksVec.end(), virtualLinksCompare);
    for ( unsigned index = 0; index < virtualLinksVec.size(); ++index )
    {
        // forming the link in physical resources from the virtual link
        Link link = getPhysicalLink(virtualLinksVec[index], req);

        bool result = assignOneVirtualLink(virtualLinksVec[index], &link, reqAssignment);
        if ( !result )
        {
            // trying limited exhaustive search
            result = limitedExhaustiveSearch(virtualLinksVec[index], reqAssignment, req);
        }
        
        if ( !result )
        {
            // trying replication, not implemented yet
            // result = replicate(virtualLinksVec[index], reqAssignment);
        }

        if ( !result )
        {
            printf("    Request assignment failed, removing virtual links\n");
            // remove assignments
            for ( unsigned i = 0; i < index; ++i )
            {
                printf("Virtual link %s is removed\n", virtualLinksVec[i]->getName().c_str());
                RemoveVirtualLink(virtualLinksVec[i], reqAssignment);
            }

            // tell the upper layer to delete assignment
            return false;
        }
    }

    return true;
}

bool VirtualLinksAssigner::assignOneVirtualLink(Link * virtualLink, Link * physicalLink, Assignment* reqAssignment)
{
    NetPath path = VirtualLinkRouter::route(physicalLink, network, VirtualLinkRouter::K_SHORTEST_PATHS);
    if ( path.size() > 0 )
    {
        reqAssignment->AddAssignment(virtualLink, path);
        AddVirtualLink(virtualLink, &path, reqAssignment);
        return true;
    }
    return false;
}

void VirtualLinksAssigner::AddVirtualLink(VirtualLink * virtualLink, NetPath* path, Assignment * assignment)
{
    NetPath::iterator it = path->begin();
    NetPath::iterator itEnd = path->end();
    for ( ; it != itEnd; ++it )
    {
        (*it)->assign(*virtualLink);
    }
}

void VirtualLinksAssigner::RemoveVirtualLink(VirtualLink * virtualLink, Assignment * assignment)
{
    NetPath path = assignment->GetAssignment(virtualLink);
    NetPath::iterator it = path.begin();
    NetPath::iterator itEnd = path.end();
    for ( ; it != itEnd; ++it )
    {
        (*it)->RemoveAssignment(virtualLink);
    }
}

bool decreaseOrder(Link * vl1, Link * vl2)
{
    return Criteria::virtualLinkWeight(vl1) > Criteria::virtualLinkWeight(vl2);
}

bool VirtualLinksAssigner::limitedExhaustiveSearch(Element * element, Assignment* assignment, Request* req)
{
    printf("  Request assignment failed, trying limited exhaustive search\n");
    
    // first, forming all assigned virtual links
    // and assosiated assignments and requests
    std::map<VirtualLink*, Assignment* > vlAssignment;
    std::map<VirtualLink*, Request* > vlRequest;
    vlRequest[static_cast<VirtualLink*>(element)] = req;
    getAllVirtualLinksAssignments(element, vlAssignment, vlRequest, assignment, req);
    for ( unsigned depth = 1; depth <= Criteria::exhaustiveSearchDepth(); ++depth )
        if ( recursiveExhaustiveSearch(static_cast<VirtualLink*>(element), assignment, vlAssignment,
                vlRequest, vlAssignment.begin(), Links(), depth) )
            return true;
    return false;
}

void VirtualLinksAssigner::getAllVirtualLinksAssignments(Element* element, 
                                                     std::map<VirtualLink*, Assignment* >& vlAssignment,
                                                     std::map<VirtualLink*, Request* >& vlRequest,
                                                     Assignment* assignment, Request* req)
{
    Links::iterator linksIt = network->getLinks().begin();
    Links::iterator linksItEnd = network->getLinks().end();
    for ( ; linksIt != linksItEnd; ++linksIt )
    {
        // get list of assignments of current vl first
        Links vls = assignment->GetAssigned(*linksIt);

        Links::iterator vlIt = vls.begin();
        Links::iterator vlItEnd = vls.end();

        for ( ; vlIt != vlItEnd; ++vlIt )
            if ( vlAssignment.find(*vlIt) == vlAssignment.end() )
            {
                vlAssignment[*vlIt] = assignment;
                vlRequest[*vlIt] = req;
            }

        // going through all other assigned requests
        RequestAssignment::iterator it = requestAssignment.begin();
        RequestAssignment::iterator itEnd = requestAssignment.end();
        for ( ; it != itEnd; ++it )
        {
            Links vls = it->second->GetAssigned(*linksIt);
            vlIt = vls.begin();
            vlItEnd = vls.end();
            for ( ; vlIt != vlItEnd; ++vlIt )
                if ( vlAssignment.find(*vlIt) == vlAssignment.end() )
                {
                    vlAssignment[*vlIt] = it->second;
                    vlRequest[*vlIt] = it->first;
                }
        }
    }
}

bool VirtualLinksAssigner::recursiveExhaustiveSearch(VirtualLink * virtualLink, Assignment* assignment,
                                                     std::map<VirtualLink*, Assignment* >& vlAssignment,
                                                     std::map<VirtualLink*, Request* >& vlRequest,
                                                     std::map<VirtualLink*, Assignment* >::iterator curIt,
                                                     Links& removedVirtualLinks, int level)
{
    if ( level > 0 )
    {
        if ( curIt == vlAssignment.end() )
            return false; // the variant with the depth less then current is not regarded.
        for ( ; curIt != vlAssignment.end(); ++curIt )
        {
            // removing virtual link and going in the next level of recursive search
            NetPath path = curIt->second->GetAssignment(curIt->first);
            RemoveVirtualLink(curIt->first, curIt->second);
            curIt->second->RemoveAssignment(curIt->first);
            

            removedVirtualLinks.insert(curIt->first);
            std::map<VirtualLink*, Assignment* >::iterator curItNext = curIt;
            ++curItNext;
            if ( recursiveExhaustiveSearch(virtualLink, assignment, vlAssignment, vlRequest, curItNext,
                removedVirtualLinks, level-1) )
                return true;

            removedVirtualLinks.erase(curIt->first);
            curIt->second->AddAssignment(curIt->first, path);
            AddVirtualLink(curIt->first, &path, curIt->second);
        }
        return false; // all variants are parsed, no successful variant was found
    } else {
        // checking the existance of the path
        Link physicalLink = getPhysicalLink(virtualLink, vlRequest[virtualLink]);
        NetPath path = VirtualLinkRouter::routeDejkstra(&physicalLink, network);
        if ( path.size() > 0 )
        {
            AddVirtualLink(virtualLink, &path, assignment);

            // trying to reassign removed virtual links
            Links::iterator it = removedVirtualLinks.begin();
            Links::iterator itEnd = removedVirtualLinks.end();
            for ( ; it != itEnd; ++it )
            {
                physicalLink = getPhysicalLink(*it, vlRequest[*it]);
                NetPath newPath = VirtualLinkRouter::routeDejkstra(&physicalLink, network);
                if ( newPath.size() > 0 )
                {
                    AddVirtualLink(*it, &newPath, vlAssignment[*it]);
                    vlAssignment[*it]->AddAssignment(*it, newPath);
                } else {
                    // attempt failed
                    for ( Links::iterator itRemove = removedVirtualLinks.begin(); itRemove != it;
                        ++itRemove)
                    {
                        RemoveVirtualLink(*itRemove, vlAssignment[*itRemove]);
                        vlAssignment[*itRemove]->RemoveAssignment(*itRemove);
                    }
                    RemoveVirtualLink(virtualLink, assignment);
                    return false;
                }
            }
            assignment->AddAssignment(virtualLink, path);
            return true;
        }
        return false;
    }
}

Link VirtualLinksAssigner::getPhysicalLink(VirtualLink* virtualLink, Request* req)
{
    Link link("dummy_virtual_link", virtualLink->getCapacity());

    Element * first = getAssigned(virtualLink->getFirst(), req);
    Element * second = getAssigned(virtualLink->getSecond(), req);
    link.bindElements(first, second);

    return link;
}

Element * VirtualLinksAssigner::getAssigned(Element * virtualResource, Request* req)
{
    if ( virtualResource->isNode() )
        return (*virtualMachinesAssignments)[req]->GetAssignment(static_cast<Node*>(virtualResource));
    if ( virtualResource->isStore() )
        return (*storagesAssignments)[req]->GetAssignment(static_cast<Store*>(virtualResource));
    return NULL;
}

void VirtualLinksAssigner::removeAssignment(Request * req)
{
    // searching for the request
    Assignment * assignment;
    if ( virtualMachinesAssignments->find(req) != virtualMachinesAssignments->end() )
        assignment = (*virtualMachinesAssignments)[req];
    else if ( storagesAssignments->find(req) != storagesAssignments->end() )
        assignment = (*storagesAssignments)[req];
    else if ( requestAssignment.find(req) != requestAssignment.end() )
    {
        printf("\nshouldn't delete this request!!!\n\n");
        assignment = requestAssignment[req];
    } else
        return; // no assignment found   

    // Removing virtual machines
    Request::VirtualMachines::iterator vmIt = req->getVirtualMachines().begin();
    Request::VirtualMachines::iterator vmItEnd = req->getVirtualMachines().end();
    for ( ; vmIt != vmItEnd; ++vmIt )
        assignment->GetAssignment(*vmIt)->RemoveAssignment(*vmIt);

    // Removing storages
    Request::Storages::iterator stIt = req->getStorages().begin();
    Request::Storages::iterator stItEnd = req->getStorages().end();
    for ( ; stIt != stItEnd; ++stIt )
        assignment->GetAssignment(*stIt)->RemoveAssignment(*stIt);

    // It is expected that removing of virtualLinks is not necessary
    // because it virtual links are assigned on the last step
}