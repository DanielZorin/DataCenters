#include <iostream>
#include <limits.h>
#include <assert.h>
#include "ant.h"
#include "../decentralized/virtualLinkRouter.h"

AntAlgorithm::AntAlgorithm(Network * n, Requests const & r, unsigned int ants, unsigned int iter, double pd, double hd, double evap)
: Algorithm(n, r)
, vmCount(0)
, stCount(0)
, bestPath(NULL)
, bestValue(0)
, antNum(ants)
, iterNum(iter)
, pherDeg(pd)
, heurDeg(hd)
, evapRate(evap)
{
    if (!init()) success = false;
    else
    {
        success = true;
        paths.resize(antNum);
        originPaths.resize(antNum);
        for (int i = 0; i < antNum; ++ i)
            paths[i] = NULL;
        objValues.resize(antNum);
        graph->setPherDeg(pd);
        graph->setHeurDeg(hd);
    }
}

AntAlgorithm::~AntAlgorithm()
{
    delete graph;
    if (bestPath) delete bestPath;
    for (int i = 0; i < paths.size(); ++ i)
    {
        if (paths[i]) delete paths[i];
        if (originPaths[i]) delete originPaths[i];
    }
}

bool AntAlgorithm::init()
{
    try
    {
        copyNetwork = new Network(*network);

        // how many VMs and storages we have
        for (Requests::iterator i = requests.begin(); i != requests.end(); i ++)
        {
            vmCount += (*i)->getVirtualMachines().size();
            stCount += (*i)->getStorages().size();
        }
        if (antNum == 0) antNum = (vmCount+stCount)/2;
        if (antNum > 100) antNum = 100;
        const Nodes& nodes = network->getNodes();
        const Stores& stores = network->getStores();
        unsigned int cnodes = nodes.size(), cstores = stores.size();
        std::vector<unsigned long> res(cnodes+cstores);
        std::vector<unsigned long> cap(cnodes+cstores);
        std::vector<unsigned int> types(cstores);
        std::vector<Element *> physNodes(cnodes);
        std::vector<Element *> physStores(cstores);

        // get physical resources' current capacity and max capacity
        int iVec = 0;
        int iTypes = 0;
        for (Nodes::const_iterator i = nodes.begin(); i != nodes.end(); i ++, ++ iVec)
        {
            res[iVec] = (*i)->getCapacity();
            cap[iVec] = (*i)->getMaxCapacity();
            physNodes[iVec] = (*i);
        }
        for (Stores::const_iterator i = stores.begin(); i != stores.end(); i ++, ++ iVec)
        {
            res[iVec] = (*i)->getCapacity();
            cap[iVec] = (*i)->getMaxCapacity();
            types[iTypes] = (*i)->getTypeOfStore();
            physStores[iTypes] = (*i);
            ++ iTypes;
        }

        // get requests' required current capacity
        std::vector<unsigned long> reqCapacity(vmCount+stCount);
        std::vector<Element *> virtElems(vmCount+stCount);
        std::vector<unsigned int> reqTypes(stCount);
        int iReq = 0;
        int iReqTypes = 0;
        for (Requests::iterator i = requests.begin(); i != requests.end(); i ++)
        {
            const Request::VirtualMachines& vms = (*i)->getVirtualMachines();
            for (Request::VirtualMachines::const_iterator i = vms.begin(); i != vms.end(); i ++, ++ iReq)
            {
                reqCapacity[iReq] = (*i)->getCapacity();
                virtElems[iReq] = (*i);
            }
        }
        for (Requests::iterator i = requests.begin(); i != requests.end(); i ++)
        {
            const Request::Storages& sts = (*i)->getStorages();
            for (Request::Storages::const_iterator i = sts.begin(); i != sts.end(); i ++, ++ iReq)
            {
                reqCapacity[iReq] = (*i)->getCapacity();
                virtElems[iReq] = (*i);
                reqTypes[iReqTypes] = (*i)->getTypeOfStore();
                ++ iReqTypes;
            }
        }

        graph = new InternalGraph(cnodes, cstores, vmCount, stCount, res, cap, reqCapacity, types, reqTypes, physNodes, physStores, virtElems);
        if (graph->isCreated()) return true;
        return false;
    }
    catch (const char* s)
    {
        std::cerr << s;
        return false;
    }
    catch (std::bad_alloc)
    {
        std::cerr << "Error while allocating memory\n";
        return false;
    }
    catch (...)
    {
        std::cerr << "Unknown error\n";
        return false;
    }
}

unsigned int AntAlgorithm::objFunctions()
{
    unsigned int iMax = antNum+1;
    double vMax = 0;
    for (int i = 0; i < antNum; ++ i)
    {
        objValues[i] = (paths[i]->getLength()-2)/((double)(vmCount+stCount));
        if (objValues[i] > vMax) { vMax = objValues[i]; iMax = i; }
    }
    return iMax;
}

bool AntAlgorithm::lastReplica(const std::map<Link *, AssignedChannel> & channels, Link * link, Element * rep)
{
    Element * st = (link->getFirst()->isStore()) ? link->getFirst() : link->getSecond();
    bool found;
    int num = 0;
    for (std::map<Link *, AssignedChannel>::const_iterator i = channels.begin(); i != channels.end(); i ++)
    {
        if ((i->first->getFirst() == st) || (i->first->getSecond() == st))
            if (i->second.replica == rep) num ++;
        if (num > 1) return false;
    }
    assert(num == 1);
    return true;
}

bool AntAlgorithm::replicaExists(const std::map<Link *, AssignedChannel> & channels, Element * replicating, Element * st)
{
    for (std::map<Link *, AssignedChannel>::const_iterator i = channels.begin(); i != channels.end(); i ++)
    {
        if ((i->first->getFirst() == replicating) || (i->first->getSecond() == replicating))
            if (i->second.replica == st) return true;
    }
    return false;
}

const AssignedChannel * AntAlgorithm::findReplica(const std::map<Link *, AssignedChannel> & channels, Element * replicating)
{
    for (std::map<Link *, AssignedChannel>::const_iterator i = channels.begin(); i != channels.end(); i ++)
        if ((i->first->getFirst() == replicating) || (i->first->getSecond() == replicating))
            if (i->second.replica && i->second.repChannel) return &(i->second);
    return NULL;
}

Algorithm::Result AntAlgorithm::schedule()
{
    std::cerr << "Algorithm parameters: antNum = " << antNum << ", iter = " << iterNum << ", pd = " << pherDeg << ", hd = " << heurDeg << ", evap = " << evapRate << '\n';
    std::cout << '\n';
    unsigned int iMax = 0;
    std::map<Link *, AssignedChannel> channels;
    for (int i = 0; i < iterNum; ++ i)
    {
        std::cerr << "Iteration " << i << '\n';
        for (int ant = 0; ant < antNum; ++ ant)
        {
            std::cerr << "Ant " << ant << '/' << antNum << '\r';
//            std::cerr << "Ant " << ant << '/' << antNum;
            channels.clear();
            graph->nextPath();
            buildPath(ant);
//            std::cerr << ", path length before = " << paths[ant]->getLength()-2;
            buildLink(ant, channels, true);
//            std::cerr << ", path length after = " << paths[ant]->getLength()-2 << '\n';
            for (std::map<Link *, AssignedChannel>::iterator iter = channels.begin(); iter != channels.end(); iter ++)
            {
                if (iter->second.dataChannel) delete iter->second.dataChannel;
                if (iter->second.repChannel) delete iter->second.repChannel;
            }
        }

        // remember the best solution
        iMax = objFunctions();
        if (iMax < antNum && objValues[iMax] > bestValue)
        {
            delete bestPath;
            bestPath = new AntPath(*originPaths[iMax]);
            bestValue = objValues[iMax];
        }
        std::cerr << "bestValue = " << bestValue;
        if (iMax < antNum)
        {
            std::cerr << ", current best value = " << objValues[iMax] << '\n';
            std::cout << objValues[iMax] << '\n';
        }
        else std::cerr << '\n';

        graph->updatePheromone(paths, objValues, evapRate, objValues[iMax]);

        // clean
        for (int j = 0; j < paths.size(); ++ j)
        {
            if (paths[j]) { delete paths[j]; paths[j] = NULL; }
            if (originPaths[j]) { delete originPaths[j]; originPaths[j] = NULL; }
        }
        if (ZERO(bestValue-1)) break;
    }

    // generate assignments
    if (bestPath == NULL) return Algorithm::FAILURE;
    std::cerr << "generating answer...";
    channels.clear();
    graph->nextPath();
    graph->assignPath(bestPath);
    paths[0] = bestPath;
    buildLink(0, channels, true);
    Assignment* asg;
    Replication* rpl;
    std::map<Link *, AssignedChannel>::iterator place;
    Element * tmpElem;
    int tmp;
    bool added = false;
    for (Requests::iterator iter = requests.begin(); iter != requests.end(); iter ++)
    {
        asg = new Assignment(*iter);
        const Request::VirtualMachines & vm = (*iter)->getVirtualMachines();
        const Request::Storages & st = (*iter)->getStorages();
        const Request::VirtualLinks & lk = (*iter)->getVirtualLinks();
        // assign virtual machines
        for (Request::VirtualMachines::const_iterator iVM = vm.begin(); iVM != vm.end(); iVM ++)
        {
            if ((tmpElem = bestPath->findPointer(*iVM, tmp)) != NULL)
                asg->AddAssignment(*iVM, static_cast<Node *>(tmpElem));
            else break;
        }
        if (tmpElem) added = true;
        // assign storages
        for (Request::Storages::const_iterator iST = st.begin(); iST != st.end(); iST ++)
        {
            if ((tmpElem = bestPath->findPointer(*iST, tmp)) != NULL)
                asg->AddAssignment(*iST, static_cast<Store *>(tmpElem));
            else break;
        }
        if (tmpElem) added = true;
        // assign links
        for (Request::VirtualLinks::const_iterator iLK = lk.begin(); iLK != lk.end(); iLK ++)
        {
            place = channels.find(*iLK);
            if (place != channels.end())
            {
                asg->AddAssignment(*iLK, *(place->second.dataChannel));
//                std::cerr << "channel between " << (*iLK)->getFirst() << " and " << (*iLK)->getSecond() << ":\n";
//                std::cerr << static_cast<Link *>((*(place->second.dataChannel))[0])->getLinkedComputationalElement() << "-";
//                for (int index = 0; index < place->second.dataChannel->size(); ++ index)
//                {
//                   std::cerr << (*(place->second.dataChannel))[index]->getName() << '(' << (*(place->second.dataChannel))[index]->getCapacity() << ")-";
//                }
//                std::cerr << static_cast<Link *>((*(place->second.dataChannel))[place->second.dataChannel->size()-1])->getLinkedComputationalElement() << '\n';
            }
        }
        // assign replications
        for (Request::VirtualLinks::const_iterator iLK = lk.begin(); iLK != lk.end(); iLK ++)
        {
            place = channels.find(*iLK);
            if (place != channels.end() && place->second.replica)
            {
                rpl = new Replication;
                tmpElem = ((*iLK)->getFirst()->isStore()) ? (*iLK)->getFirst() : (*iLK)->getSecond();
                rpl->setStorage(static_cast<Storage *>(tmpElem));
                rpl->bind(static_cast<Store *>(bestPath->findPointer(tmpElem, tmp)), place->second.replica);
                if (place->second.repChannel) rpl->setLink(*(place->second.repChannel));
                else
                {
                    const AssignedChannel * exist = findReplica(channels, tmpElem);
                    rpl->setLink(*(exist->repChannel));
                }
                if (!asg->checkReplicaOnStore(static_cast<Storage *>(tmpElem), place->second.replica)) asg->AddReplication(rpl);
            }
        }
        if (added) assignments.insert(asg);
//        std::cerr << "added = " << added << '\n';
        added = false;
    }
    std::cerr << "ok\n";
    std::cout << assignments.size() << ' ';
    int repSum = 0;
    for (Assignments::iterator i = assignments.begin(); i != assignments.end(); i ++) repSum += (*i)->getReplications().size();
    std::cout << repSum << '\n';

    paths[0] = NULL;
    if (ZERO(bestValue-1)) return Algorithm::SUCCESS;
    return Algorithm::PARTIAL;
}

void AntAlgorithm::removeRequestElements(unsigned int vertex, AntPath* pt, std::set<unsigned int> & availableVM, std::set<unsigned int> & availableST,
                                         GraphComponent::RequestType t)
{
    int erased;
    if (t == GraphComponent::VMACHINE)
    {
        unsigned int sum = 0, oldSum = 0;
        unsigned int storesSum = vmCount, oldStoresSum = vmCount;
        for (Requests::iterator i = requests.begin(); i != requests.end(); i ++)
        {
            sum += (*i)->getVirtualMachines().size();
            storesSum += (*i)->getStorages().size();
            if (sum >= vertex)
            {
                for (int req = oldSum+1; req <= sum; ++ req)
                {
                    if ((erased = pt->eraseRequest(req)) != -1) graph->requestErased(erased, req, GraphComponent::VMACHINE);
                    availableVM.erase(req);
                }
                for (int req = oldStoresSum+1; req <= storesSum; ++ req)
                {
                    if ((erased = pt->eraseRequest(req)) != -1) graph->requestErased(erased, req, GraphComponent::STORAGE);
                    availableST.erase(req);
                }
                break;
            }
            oldSum = sum;
            oldStoresSum = storesSum;
        }
    }
    else if (t == GraphComponent::STORAGE)
    {
        unsigned int sum = vmCount, oldSum = vmCount;
        unsigned int nodesSum = 0, oldNodesSum = 0;
        for (Requests::iterator i = requests.begin(); i != requests.end(); i ++)
        {
            sum += (*i)->getStorages().size();
            nodesSum += (*i)->getVirtualMachines().size();
            if (sum >= vertex)
            {
                for (int req = oldSum+1; req <= sum; ++ req)
                {
                    if ((erased = pt->eraseRequest(req)) != -1) graph->requestErased(erased, req, GraphComponent::STORAGE);
                    availableST.erase(req);
                }
                for (int req = oldNodesSum+1; req <= nodesSum; ++ req)
                {
                    if ((erased = pt->eraseRequest(req)) != -1) graph->requestErased(erased, req, GraphComponent::VMACHINE);
                    availableVM.erase(req);
                }
                break;
            }
            oldSum = sum;
            oldNodesSum = nodesSum;
        }
    }
}

bool AntAlgorithm::buildPath(unsigned int ant)
{
    std::set<unsigned int> availableNodes;
    std::set<unsigned int> availableStores;
    AntPath* pt = new AntPath(vmCount+stCount);

    int i = 1;
    for (; i <= vmCount; ++ i) availableNodes.insert(i);
    for (; i <= vmCount+stCount; ++ i) availableStores.insert(i);

    unsigned int vertex, oldVertex;
    bool s;
    // build through virtual machines' vertices
    pt->addElement(new PathElement(0, NULL, 0, NULL));
    while (!availableNodes.empty())
    {
        vertex = graph->selectVertex(pt, 0, availableNodes, s);
        if (!s) removeRequestElements(vertex, pt, availableNodes, availableStores, GraphComponent::VMACHINE);
        else break;
    }
    oldVertex = vertex;
    while(!availableNodes.empty())
    {
        vertex = graph->selectVertex(pt, oldVertex, availableNodes, s);
        if (!s)
        {
            removeRequestElements(vertex, pt, availableNodes, availableStores, GraphComponent::VMACHINE);
            vertex = oldVertex;
        }
        else oldVertex = vertex;
    }

//    std::cerr << "================\n";
    //build through storages' vertices
    pt->addElement(new PathElement(0, NULL, 0, NULL));
    while (!availableStores.empty())
    {
        vertex = graph->selectVertex(pt, 0, availableStores, s);
        if (!s) removeRequestElements(vertex, pt, availableNodes, availableStores, GraphComponent::STORAGE);
        else break;
    }
    oldVertex = vertex;
    while (!availableStores.empty())
    {
        vertex = graph->selectVertex(pt, vertex, availableStores, s);
        if (!s)
        {
            removeRequestElements(vertex, pt, availableNodes, availableStores, GraphComponent::STORAGE);
            vertex = oldVertex;
        }
        else oldVertex = vertex;
    }

    paths[ant] = pt;
    originPaths[ant] = new AntPath(*pt);
    return true;
}

void AntAlgorithm::buildLink(unsigned int ant, std::map<Link *, AssignedChannel> & channels, bool restore)
{
    std::set<unsigned int> emptySet; // for removeRequestElements
    Element* firstRes, *secondRes;
    int firstVertex, secondVertex;
    GraphComponent::RequestType firstType;
    const Stores& storesPtr = network->getStores();
    for(Requests::iterator i = requests.begin(); i != requests.end(); i ++)
    {
        Request::VirtualLinks& links = (*i)->getVirtualLinks();
        for (Request::VirtualLinks::iterator lk = links.begin(); lk != links.end(); lk ++)
        {
            if ((*lk)->getFirst() == (*lk)->getSecond()) continue;
            firstRes = paths[ant]->findPointer((*lk)->getFirst(), firstVertex);
            secondRes = paths[ant]->findPointer((*lk)->getSecond(), secondVertex);
            if (firstRes && secondRes)
            {
                Link linkToBuild("", (*lk)->getCapacity());
                linkToBuild.bindElements(firstRes, secondRes);
//                std::cerr << "Routing link from " << firstVertex << '(' << (*lk)->getFirst() << ") to " << secondVertex << '(' << (*lk)->getSecond() <<
//                             "), capacity = " << linkToBuild.getCapacity() << ", assigned to " << linkToBuild.getFirst() << " and " << linkToBuild.getSecond() << '\n';
                firstType = (firstRes->isNode()) ? GraphComponent::VMACHINE : GraphComponent::STORAGE;
                // try to route
                NetPath * channel = new NetPath(VirtualLinkRouter::route(&linkToBuild, network, VirtualLinkRouter::DEJKSTRA));
                if (channel->empty())
                {
                    // replication
                    Element * source = (firstType == GraphComponent::STORAGE) ? firstRes : secondRes;
                    Element * destination = (firstType == GraphComponent::STORAGE) ? secondRes : firstRes;
                    Element * replicating = ((*lk)->getFirst()->isNode() == true) ? (*lk)->getSecond() : (*lk)->getFirst();
                    Element * repDest = NULL;
                    AssignedChannel * exist = NULL;
//                    std::cerr << "Trying replication. Source = " << source << ", dest = " << destination << ", replicating = " << replicating << '\n';
                    NetPath * repChannel, * dataChannel;
                    Link repLink("", static_cast<Store *>(replicating)->getReplicationCapacity());
                    Link dataLink("", (*lk)->getCapacity());
                    bool repSuccess = false;
                    std::vector<unsigned long> curStoresRes = graph->getCurStoresRes();
                    unsigned int minRes = curStoresRes[0], minIndex = 0;
                    if (static_cast<Store *>(replicating)->getTypeOfStore() != 0 && replicating->isStore())
                    {
                        // try existing replication
                        const AssignedChannel * exist = findReplica(channels, replicating);
                        if (exist != NULL)
                        {
                            dataLink.bindElements(exist->replica, destination);
                            dataChannel = new NetPath(VirtualLinkRouter::route(&dataLink, network, VirtualLinkRouter::DEJKSTRA));
                            if (!dataChannel->empty())
                            {
                                repDest = exist->replica;
                                minIndex = exist->rindex;
                                repChannel = NULL;
                                repSuccess = true;
                            }
                            else repSuccess = false;
                        }
                        // create a replication
                        else
                        {
                            for (int p = 0; p < curStoresRes.size(); ++ p)
                            {
                                if (curStoresRes[p] < replicating->getCapacity()) curStoresRes[p] = LONG_MAX;
                                else curStoresRes[p] -= replicating->getCapacity();
                            }
                            while (1)
                            {
                                // sort
                                for (unsigned int q = 0; q < curStoresRes.size(); ++ q)
                                {
                                    if (curStoresRes[q] < LONG_MAX && curStoresRes[q] < minRes) { minRes = curStoresRes[q]; minIndex = q; }
                                }
                                if (minIndex >= curStoresRes.size()) { repSuccess = false; break; }
                                curStoresRes[minIndex] = LONG_MAX;
                                // SET DON'T HAVE += FOR ITERATORS, ****!
                                Stores::const_iterator iter = storesPtr.begin();
                                for (unsigned int q = 0; q < minIndex; ++ q) iter ++;
                                repDest = *iter;
//                                std::cerr << "Trying " << minIndex << '(' << repDest << ")\n";
                                // is replication possible?
                                if (repDest == source || static_cast<Store *>(repDest)->getTypeOfStore() != static_cast<Store *>(replicating)->getTypeOfStore())
                                {
//                                    std::cerr << "Failed because repDest == " << repDest << ", source == " << source << ", repDest.type == " <<
//                                                 static_cast<Store *>(repDest)->getTypeOfStore() << "replicating.type == " <<
//                                                 static_cast<Store *>(replicating)->getTypeOfStore() << '\n';
                                    minIndex = curStoresRes.size();
                                    minRes = LONG_MAX;
                                    continue;
                                }
                                repLink.bindElements(repDest, source);
                                dataLink.bindElements(repDest, destination);

                                // route channels
//                                std::cerr << "Routing link from " << repDest << " to " << source << ", capacity = " << repLink.getCapacity();
                                repChannel = new NetPath(VirtualLinkRouter::route(&repLink, network, VirtualLinkRouter::DEJKSTRA));
//                                if (repChannel->empty()) std::cerr << "... no\n";
//                                else std::cerr << "... yes\n";

//                                std::cerr << "Routing link from " << repDest << " to " << destination << ", capacity = " << dataLink.getCapacity();
                                dataChannel = new NetPath(VirtualLinkRouter::route(&dataLink, network, VirtualLinkRouter::DEJKSTRA));
//                                if (dataChannel->empty()) std::cerr << "... no\n";
//                                else std::cerr << "... yes\n";

                                if (!repChannel->empty() && !dataChannel->empty()) { repSuccess = true; break; }
                                minIndex = curStoresRes.size();
                                minRes = LONG_MAX;
                            }
                        }
                    }
                    if (!repSuccess)
                    {
//                        std::cerr << "There is no virtual channel\n";
                        // replication failed, removing first is enough
                        removeRequestElements(firstVertex, paths[ant], emptySet, emptySet, firstType);
                        // remove assigned links and replications
                        for (Request::VirtualLinks::iterator iter = links.begin(); iter != links.end(); iter ++)
                        {
                            std::map<Link *, AssignedChannel>::iterator place = channels.find(*iter);
                            if (place != channels.end())
                            {
                                AssignedChannel ach = place->second;
                                // find index and restore capacity on the replica
                                if (ach.replica && lastReplica(channels, *iter, ach.replica))
                                {
                                    Element * repElem = (place->first->getFirst()->isStore()) ? place->first->getFirst() : place->first->getSecond();
//                                    std::cerr << "Restored " << repElem << " capacity on the store " << ach.rindex << '(' << ach.replica << ")\n";
                                    graph->increaseCurStoresRes(ach.rindex, ach.replica->getCapacity());
                                }
                                // restore capacity on the consistency channel
                                if (ach.repChannel)
                                {
//                                    std::cerr << "Restored capacity on the consistency channel: ";
                                    Element * repElem = (place->first->getFirst()->isStore()) ? place->first->getFirst() : place->first->getSecond();
                                    Link tmp("", static_cast<Store *>(repElem)->getReplicationCapacity());
                                    for (int index = 0; index < ach.repChannel->size(); ++ index)
                                    {
                                        (*(ach.repChannel))[index]->RemoveAssignment(&tmp);
//                                        std::cerr << (*(ach.repChannel))[index]->getName() << '(' << (*(ach.repChannel))[index]->getCapacity() << ")-";
                                    }
//                                    std::cerr << '\n';
                                }
                                // restore capacity on the data channel
//                                std::cerr << "Restored capacity on the data channel: ";
                                for (int index = 0; index < ach.dataChannel->size(); ++ index)
                                {
                                    (*(ach.dataChannel))[index]->RemoveAssignment(place->first);
//                                    std::cerr << (*(ach.dataChannel))[index]->getName() << '(' << (*(ach.dataChannel))[index]->getCapacity() << ")-";
                                }
//                                std::cerr << '\n';
                                channels.erase(place);
                            }
                        }
                    }
                    else
                    {
                        // replication successful
//                        std::cerr << "Replication successful, minIndex = " << minIndex << ", paths: " << '\n';
                        if (repChannel)
                        {
                            for (int index = 0; index < repChannel->size(); ++ index)
                            {
                            (*repChannel)[index]->assign(repLink);
//                               std::cerr << (*repChannel)[index]->getName() << '(' << (*repChannel)[index]->getCapacity() << ")-";
                            }
                        }
//                        std::cerr << '\n';
                        for (int index = 0; index < dataChannel->size(); ++ index)
                        {
                           (*dataChannel)[index]->assign(dataLink);
//                           std::cerr << (*dataChannel)[index]->getName() << '(' << (*dataChannel)[index]->getCapacity() << ")-";
                        }
//                        std::cerr << '\n';
                        if (!replicaExists(channels, replicating, repDest))
                        {
                            graph->decreaseCurStoresRes(minIndex, replicating->getCapacity());
//                            std::cerr << "Created " << replicating << " replica on " << minIndex << '\n';
                        }
                        std::map<Link *, AssignedChannel>::iterator iter = channels.end();
                        AssignedChannel ach(dataChannel, repChannel, static_cast<Store *>(repDest), minIndex);
                        channels.insert(iter, std::pair<Link *, AssignedChannel>(*lk, ach));
                    }
                }
                else // !channel.empty()
                {
//                    std::cerr << "Found channel: ";
                    for (int index = 0; index < channel->size(); ++ index)
                    {
                        (*channel)[index]->assign(linkToBuild);
//                        std::cerr << (*channel)[index]->getName() << '(' << (*channel)[index]->getCapacity() << ")-";
                    }
//                    std::cerr << '\n';
                    std::map<Link *, AssignedChannel>::iterator iter = channels.end();
                    AssignedChannel ach(channel, NULL, NULL, 0);
                    channels.insert(iter, std::pair<Link *, AssignedChannel>(*lk, ach));
                }
            }
        }
    }
    // restore network
    if (restore) network->assign(*copyNetwork);
}
