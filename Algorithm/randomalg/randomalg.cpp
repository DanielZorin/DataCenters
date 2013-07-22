#include "randomalg.h"
#include "../decentralized/virtualLinkRouter.h"
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <iostream>
#include <map>

RandomAlgorithm::RandomAlgorithm(Network * n, Requests const & r, unsigned long tr)
: Algorithm(n, r)
, copyNetwork(NULL)
, tries(tr)
{
    copyNetwork = new Network();
    *copyNetwork = *network;
    for (Requests::iterator i = requests.begin(); i != requests.end(); i ++)
    {
        vmCount += (*i)->getVirtualMachines().size();
        stCount += (*i)->getStorages().size();
    }

//    srand(time(NULL));
    srand(1);

    bestSequence.reserve(vmCount+stCount);
    bestReq.reserve(requests.size());
}

RandomAlgorithm::~RandomAlgorithm()
{
    delete copyNetwork;
}

Element * RandomAlgorithm::findResource(Element * request, std::vector<SequenceElement *> seq, unsigned int start)
{
    for(unsigned int i = start; i < seq.size(); ++ i)
        if (seq[i]->request == request) return seq[i]->resource;
    return NULL;
}

Algorithm::Result RandomAlgorithm::schedule()
{
    // current sequence of elements, a list of assigned requests and a map with assigned channels
    std::map<Link *, NetPath> channels;
    std::vector<SequenceElement *> curSeq;
    std::vector<Request *> curReq;
    curSeq.reserve(vmCount+stCount);
    curReq.reserve(requests.size());

    std::vector<Element *> chooseNode;
    chooseNode.reserve(vmCount);
    std::vector<Element *> chooseStore;
    chooseStore.reserve(stCount);
    std::vector<NetPath> choosePath;
    choosePath.reserve((*requests.begin())->getVirtualLinks().size());
    Nodes& physNodes = network->getNodes();
    Stores& physStores = network->getStores();
    unsigned int roll = 0;
    bool fail = false;
    for (int iter = 0; iter < tries; ++ iter)
    {
        std::cerr << "Try #" << iter << '\n';
        for (Requests::iterator ri = requests.begin(); ri != requests.end(); ri ++)
        {
            //std::cerr << "request " << *ri << '\n';
            unsigned int curSize = curSeq.size();
            // assign virtual machines
            const Request::VirtualMachines& vms = (*ri)->getVirtualMachines();
            for (Request::VirtualMachines::const_iterator vmi = vms.begin(); vmi != vms.end(); vmi ++)
            {
                // choose nodes which the VM can be assigned to
                for (Nodes::iterator ni = physNodes.begin(); ni != physNodes.end(); ni ++)
                    if ((*ni)->isAssignmentPossible(**vmi)) chooseNode.push_back(*ni);
                //std::cerr << "# of available nodes: " << chooseNode.size() << '\n';
                if (chooseNode.size() == 0) { fail = true; break; }
                // randomly choose from those nodes
                roll = (int)(((double)rand())/(RAND_MAX+1.0)*chooseNode.size());
                curSeq.push_back(new SequenceElement(*vmi, chooseNode[roll]));
                chooseNode[roll]->assign(**vmi);
                chooseNode.clear();
            }
            if (fail)
            {
                // remove all elements from the sequence starting from curSize
                while (curSeq.size() > curSize)
                {
                    curSeq[curSeq.size()-1]->resource->RemoveAssignment(curSeq[curSeq.size()-1]->request);
                    curSeq.pop_back();
                }
                fail = false;
                continue;
            }
            // assign storages
            const Request::Storages& sts = (*ri)->getStorages();
            for (Request::Storages::const_iterator sti = sts.begin(); sti != sts.end(); sti ++)
            {
                // choose stores which the storage can be assigned to
                for (Stores::iterator phsi = physStores.begin(); phsi != physStores.end(); phsi ++)
                    if ((*phsi)->isAssignmentPossible(**sti) && (*phsi)->getTypeOfStore() == (*sti)->getTypeOfStore()) chooseStore.push_back(*phsi);
                //std::cerr << "# of available stores: " << chooseStore.size() << '\n';
                if (chooseStore.size() == 0) { fail = true; break; }
                // randomly choose from those stores
                roll = (int)(((double)rand())/(RAND_MAX+1.0)*chooseStore.size());
                curSeq.push_back(new SequenceElement(*sti, chooseStore[roll]));
                chooseStore[roll]->assign(**sti);
                chooseStore.clear();
            }
            if (fail)
            {
                // remove all elements from the sequence starting from curSize
                while (curSeq.size() > curSize)
                {
                    curSeq[curSeq.size()-1]->resource->RemoveAssignment(curSeq[curSeq.size()-1]->request);
                    curSeq.pop_back();
                }
                fail = false;
                continue;
            }
            // route links
            const Request::VirtualLinks& links = (*ri)->getVirtualLinks();
            Element * firstRes, * secondRes;
            for (Request::VirtualLinks::const_iterator lki = links.begin(); lki != links.end(); lki ++)
            {
                if ((*lki)->getFirst() == (*lki)->getSecond()) continue;
                firstRes = findResource((*lki)->getFirst(), curSeq, curSize);
                secondRes = findResource((*lki)->getSecond(), curSeq, curSize);
                assert(firstRes && secondRes);
                Link toRoute("", (*lki)->getCapacity());
                toRoute.bindElements(firstRes, secondRes);
                //std::cerr << "link = " << *lki << ", capacity = " << (*lki)->getCapacity() << ", getFirst = " << (*lki)->getFirst() << ", getSecond = " << (*lki)->getSecond() << ", firstRes = " << firstRes << ", secondRes = " << secondRes << ", toRoute.getFirst = " << toRoute.getFirst() << ", toRoute.getSecond = " << toRoute.getSecond() << ", toRoute.capacity = " << toRoute.getCapacity() << '\n';
                // build some paths
                VirtualLinkRouter::route(&toRoute, network, VirtualLinkRouter::K_SHORTEST_PATHS_ALL, &choosePath);
                if (choosePath.size() == 0) { fail = true; break; }
                // randomly choose from those paths
                roll = (int)(((double)rand())/(RAND_MAX+1.0)*choosePath.size());
                channels.insert(channels.end(), std::pair<Link *, NetPath>(*lki, choosePath[roll]));
                // assign path
                for (NetPath::iterator inet = choosePath[roll].begin(); inet != choosePath[roll].end(); inet ++)
                    (*inet)->assign(toRoute);
                choosePath.clear();
            }
            if (fail)
            {
                // remove all elements from the sequence starting from curSize
                while (curSeq.size() > curSize)
                {
                    curSeq[curSeq.size()-1]->resource->RemoveAssignment(curSeq[curSeq.size()-1]->request);
                    curSeq.pop_back();
                }
                fail = false;
                // remove all assigned channels from this request
                std::map<Link *, NetPath>::iterator place;
                for (Request::VirtualLinks::const_iterator lki = links.begin(); lki != links.end(); lki ++)
                {
                    if ((place = channels.find(*lki)) != channels.end())
                    {
                        for (NetPath::iterator inet = place->second.begin(); inet != place->second.end(); inet ++)
                            (*inet)->RemoveAssignment(*lki);
                        channels.erase(place);
                    }
                }
                continue;
            }
            // if we are here, everything is ok
            curReq.push_back(*ri);
        }
        std::cerr << "Assigned " << curReq.size() << '/' << requests.size() << " requests\n";
        if (curReq.size() > bestReq.size())
        {
            bestReq.swap(curReq);
            bestSequence.swap(curSeq);
            bestChan.swap(channels);
        }
        curSeq.clear();
        curReq.clear();
        channels.clear();
        network->assign(*copyNetwork);
    }
    // create assignments
    std::cerr << "Generating answer...\n";
    Assignment * asg;
    unsigned int index = 0;
    unsigned int oldIndex = index;
    std::map<Link *, NetPath>::iterator place;
    Request * curPtr = NULL;
    for (unsigned int ireq = 0; ireq < bestReq.size(); ++ ireq)
    {
        curPtr = bestReq[ireq];
        asg = new Assignment(curPtr);
        const Request::VirtualLinks& bestLinks = curPtr->getVirtualLinks();
        for (Request::VirtualLinks::const_iterator lk = bestLinks.begin(); lk != bestLinks.end(); lk ++)
        {
            if ((*lk)->getFirst() == (*lk)->getSecond()) continue;
            place = bestChan.find(*lk);
            assert(place != bestChan.end());
            asg->AddAssignment(*lk, place->second);
        }

        oldIndex = index;
        for (;index < oldIndex+curPtr->getVirtualMachines().size(); ++ index)
        {
            asg->AddAssignment(dynamic_cast<Node*>(bestSequence[index]->request), dynamic_cast<Node*>(bestSequence[index]->resource));
            assert(curPtr->getVirtualMachines().find(dynamic_cast<Node*>(bestSequence[index]->request)) != curPtr->getVirtualMachines().end());
        }

        oldIndex = index;
        for (;index < oldIndex+curPtr->getStorages().size(); ++ index)
        {
            asg->AddAssignment(dynamic_cast<Store*>(bestSequence[index]->request), dynamic_cast<Store*>(bestSequence[index]->resource));
            assert(curPtr->getStorages().find(dynamic_cast<Store*>(bestSequence[index]->request)) != curPtr->getStorages().end());
        }
        assignments.insert(asg);
    }
    std::cerr << "Assigned " << assignments.size() << '/' << requests.size() << " requests\n";
    return SUCCESS;
}
