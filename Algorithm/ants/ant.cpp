#include <iostream>
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
        paths.resize(ants);
        for (int i = 0; i < ants; ++ i)
            paths[i] = NULL;
        objValues.resize(ants);
        graph->setPherDeg(pd);
        graph->setHeurDeg(hd);
    }
}

AntAlgorithm::~AntAlgorithm()
{
    delete graph;
    for (int i = 0; i < paths.size(); ++ i)
        if (paths[i]) delete paths[i];
}

bool AntAlgorithm::init()
{
    try
    {
        // how many VMs and storages we have
        for (Requests::iterator i = requests.begin(); i != requests.end(); i ++)
        {
            vmCount += (*i)->getVirtualMachines().size();
            stCount += (*i)->getStorages().size();
        }
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

Algorithm::Result AntAlgorithm::schedule()
{
    unsigned int iMax = 0;
    for (int i = 0; i < iterNum; ++ i)
    {
        for (int ant = 0; ant < antNum; ++ ant)
        {
            graph->nextPath();
            buildPath(ant);
            buildLink(ant, false);
            std::cerr << "---\n";
        }

        // remember the best solution
        iMax = objFunctions();
        if (iMax < antNum && objValues[iMax] > bestValue)
        {
            delete bestPath;
            bestPath = new AntPath(*paths[iMax]);
            bestValue = objValues[iMax];
//            if (ZERO(bestValue-1)) break;
            std::cerr << "bestValue = " << bestValue << '\n';
        }

        graph->updatePheromone(paths, objValues, evapRate);

        for (int j = 0; j < paths.size(); ++ j)
            if (paths[i]) { delete paths[i]; paths[i] = NULL; }
    }
    return Algorithm::SUCCESS;
}

void AntAlgorithm::removeRequestElements(unsigned int vertex, AntPath* pt, std::set<unsigned int> & available, GraphComponent::RequestType t)
{
    int erased;
    if (t == GraphComponent::VMACHINE)
    {
        unsigned int sum = 0, oldSum = 0;
        for (Requests::iterator i = requests.begin(); i != requests.end(); i ++)
        {
            sum += (*i)->getVirtualMachines().size();
            if (sum >= vertex)
            {
                for (int req = oldSum+1; req <= sum; ++ req)
                {
                    if ((erased = pt->eraseRequest(req)) != -1) graph->requestErased(erased, req, GraphComponent::VMACHINE);
                    available.erase(req);
                }
                break;
            }
            oldSum = sum;
        }
    }
    else if (t == GraphComponent::STORAGE)
    {
        unsigned int sum = vmCount, oldSum = vmCount;
        for (Requests::iterator i = requests.begin(); i != requests.end(); i ++)
        {
            sum += (*i)->getStorages().size();
            if (sum >= vertex)
            {
                for (int req = oldSum+1; req <= sum; ++ req)
                {
                    if ((erased = pt->eraseRequest(req)) != -1) graph->requestErased(erased, req, GraphComponent::STORAGE);
                    available.erase(req);
                }
                break;
            }
            oldSum = sum;
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
        if (!s) removeRequestElements(vertex, pt, availableNodes, GraphComponent::VMACHINE);
        else break;
    }
    oldVertex = vertex;
    while(!availableNodes.empty())
    {
        vertex = graph->selectVertex(pt, oldVertex, availableNodes, s);
        if (!s)
        {
            removeRequestElements(vertex, pt, availableNodes, GraphComponent::VMACHINE);
            vertex = oldVertex;
        }
        else oldVertex = vertex;
    }

    //build through storages' vertices
    pt->addElement(new PathElement(0, NULL, 0, NULL));
    while (!availableStores.empty())
    {
        vertex = graph->selectVertex(pt, 0, availableStores, s);
        if (!s) removeRequestElements(vertex, pt, availableStores, GraphComponent::STORAGE);
        else break;
    }
    oldVertex = vertex;
    while (!availableStores.empty())
    {
        vertex = graph->selectVertex(pt, vertex, availableStores, s);
        if (!s)
        {
            removeRequestElements(vertex, pt, availableStores, GraphComponent::STORAGE);
            vertex = oldVertex;
        }
        else oldVertex = vertex;
    }

    paths[ant] = pt;
    return true;
}

std::vector<NetPath>* AntAlgorithm::buildLink(unsigned int ant, bool resultNeeded)
{
    std::set<unsigned int> emptySet; // for removeRequestElements
    Element* firstRes, *secondRes;
    int firstVertex, secondVertex;
    GraphComponent::RequestType firstType;
    Link linkToBuild;
    std::vector<NetPath>* resultPaths = NULL;
    if (resultNeeded) resultPaths = new std::vector<NetPath>;
    for(Requests::iterator i = requests.begin(); i != requests.end(); i ++)
    {
        const Request::VirtualLinks& links = (*i)->getVirtualLinks();
        if (resultNeeded) resultPaths->reserve(resultPaths->size()+links.size());
        for (Request::VirtualLinks::const_iterator lk = links.begin(); lk != links.end(); lk ++)
        {
            firstRes = paths[ant]->findPointer((*lk)->getFirst(), firstVertex);
            secondRes = paths[ant]->findPointer((*lk)->getSecond(), secondVertex);
            if (firstRes && secondRes)
            {
//                std::cerr << "Routing link from " << firstVertex << '(' << (*lk)->getFirst() << ") to " << secondVertex << '(' << (*lk)->getSecond() << 
//                             "), capacity = " << (*lk)->getCapacity() << '\n';
                firstType = (firstRes->isNode()) ? GraphComponent::VMACHINE : GraphComponent::STORAGE;
                linkToBuild.bindElements(firstRes, secondRes);
                NetPath channel = VirtualLinkRouter::route(&linkToBuild, network, VirtualLinkRouter::DEJKSTRA);
//                NetPath channel = VirtualLinkRouter::route(&linkToBuild, network, VirtualLinkRouter::K_SHORTEST_PATHS);
                if (channel.empty())
                {
                    //TODO:replication
                    // replication failed, removing first is enough
                    std::cerr << "There is no virtual channel\n";
                    removeRequestElements(firstVertex, paths[ant], emptySet, firstType);
                }
/*                else
                {
                    std::cerr << "Found channel: ";
                    for (int index = 0; index < channel.size(); ++ index)
                    {
                        std::cerr << channel[index]->getName() << '(' << channel[index]->getCapacity() << ")-";
                    }
                    std::cerr << '\n';
                }*/
                if (resultNeeded) resultPaths->push_back(channel);
            }
        }
    }
    return resultPaths;
}
