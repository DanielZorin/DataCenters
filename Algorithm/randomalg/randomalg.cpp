#include "randomalg.h"
#include "decentralized/virtualLinkRouter.h"
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <iostream>
#include <map>


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
    if (requests.size() == 0)
    {
        std::cerr << "There are no requests.\n";
        return Algorithm::FAILURE;
    }
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

    std::vector<CritValue> crit;
    crit.reserve(vmCount);
    unsigned int roll = 0;
    unsigned int critMax = 0, iMax = 0;
    long tempCrit = LONG_MAX;
    bool fail = false, found = false;
    for (int iter = 0; iter < tries; ++ iter)
    {
        std::cerr << "Try #" << iter << '\n';
        for (Requests::iterator ri = requests.begin(); ri != requests.end(); ri ++)
        {
            unsigned int curSize = curSeq.size();
            // assign virtual machines
            const Request::VirtualMachines& vms = (*ri)->getVirtualMachines();
            for (Request::VirtualMachines::const_iterator vmi = vms.begin(); vmi != vms.end(); vmi ++)
            {
                // calculate criterion values
                for (Nodes::iterator ni = physNodes.begin(); ni != physNodes.end(); ni ++)
                {
                    //crit.push_back(CritValue((*ni)->getCapacity(), *ni));
                    crit.push_back(CritValue(tempCrit-(*ni)->getID(), *ni));
                }
                // choose N best nodes
                for (unsigned int N = 0; N < NRes && N < physNodes.size(); ++ N)
                {
                    found = false;
                    critMax = 0;
                    for (unsigned int index = 0; index < crit.size(); ++ index)
                        if (crit[index].value > critMax && dynamic_cast<Node *>(crit[index].resource)->isAssignmentPossible(**vmi))
                        {
                            critMax = crit[index].value;
                            iMax = index;
                            found = true;
                        }
                    if (found) { chooseNode.push_back(crit[iMax].resource); crit.erase(crit.begin()+iMax); }
                    else break;
                }
                if (chooseNode.size() == 0) { fail = true; break; }
                // randomly choose from those nodes
                roll = (int)(((double)rand())/(RAND_MAX+1.0)*chooseNode.size());
                curSeq.push_back(new SequenceElement(*vmi, chooseNode[roll]));
                chooseNode[roll]->assign(**vmi);
                chooseNode.clear();
                crit.clear();
                tempCrit = LONG_MAX;
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
            tempCrit = LONG_MAX;
            const Request::Storages& sts = (*ri)->getStorages();
            for (Request::Storages::const_iterator sti = sts.begin(); sti != sts.end(); sti ++)
            {
                // calculate criterion values
                for (Stores::iterator si = physStores.begin(); si != physStores.end(); si ++)
                {
                    //crit.push_back(CritValue((*si)->getCapacity(), *si));
                    crit.push_back(CritValue(tempCrit - (*si)->getID(), *si));
                }
                // choose N best stores
                for (unsigned int N = 0; N < NRes && N < physStores.size(); ++ N)
                {
                    found = false;
                    critMax = 0;
                    for (unsigned int index = 0; index < crit.size(); ++ index)
                        if (crit[index].value > critMax && dynamic_cast<Store *>(crit[index].resource)->isAssignmentPossible(**sti) &&
                            dynamic_cast<Store *>(crit[index].resource)->getTypeOfStore() == (*sti)->getTypeOfStore())
                        {
                            critMax = crit[index].value;
                            iMax = index;
                            found = true;
                        }
                    if (found) { chooseStore.push_back(crit[iMax].resource); crit.erase(crit.begin()+iMax); }
                    else break;
                }
                if (chooseStore.size() == 0) { fail = true; break; }
                // randomly choose from those stores
                roll = (int)(((double)rand())/(RAND_MAX+1.0)*chooseStore.size());
                curSeq.push_back(new SequenceElement(*sti, chooseStore[roll]));
                chooseStore[roll]->assign(**sti);
                chooseStore.clear();
                crit.clear();
                tempCrit = LONG_MAX;
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
//                    std::cerr << "removed " << curSeq[curSeq.size()-1]->request->getCapacity() << " of " << curSeq[curSeq.size()-1]->request->getName().c_str() << ", now capacity = " << curSeq[curSeq.size()-1]->resource->getCapacity() << " on " << curSeq[curSeq.size()-1]->resource->getName() << '\n';
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
        if (iter == tries - 1) break;
        curSeq.clear();
        curReq.clear();
        channels.clear();
        if (bestReq.size() == requests.size()) break;
        network->assign(*copyNetwork);
    }
    // create assignments
    std::cerr << "Generating answer...\n";
    Assignment * asg;
    unsigned int index = 0;
    unsigned int oldIndex = index;
    std::map<Link *, NetPath>::iterator place;
    Request * curPtr = NULL;
    assignments.clear();
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
