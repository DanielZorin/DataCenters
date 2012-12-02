#include <iostream>
#include "ant.h"

AntAlgorithm::AntAlgorithm(Network * n, Requests const & r, unsigned int ants, unsigned int iter, double pd, double hd)
: Algorithm(n, r)
, vmCount(0)
, stCount(0)
, bestPath(NULL)
, bestValue(0)
, antNum(ants)
, iterNum(iter)
, pherDeg(pd)
, heurDeg(hd)
{
    if (!init()) success = false;
    else
    {
        success = true;
        paths.resize(ants);
        for (int i = 0; i < ants; ++ i) paths[i] = NULL;
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

        // get physical resources' current capacity and max capacity
        int iVec = 0;
        int iTypes = 0;
        for (Nodes::const_iterator i = nodes.begin(); i != nodes.end(); i ++, ++ iVec)
        {
            res[iVec] = (*i)->getCapacity();
            cap[iVec] = (*i)->getMaxCapacity();
        }
        for (Stores::const_iterator i = stores.begin(); i != stores.end(); i ++, ++ iVec)
        {
            res[iVec] = (*i)->getCapacity();
            cap[iVec] = (*i)->getMaxCapacity();
            types[iTypes] = (*i)->getTypeOfStore();
            ++ iTypes;
        }

        // get requests' required current capacity
        std::vector<unsigned long> reqCapacity(vmCount+stCount);
        std::vector<unsigned int> reqTypes(stCount);
        int iReq = 0;
        int iReqTypes = 0;
        for (Requests::iterator i = requests.begin(); i != requests.end(); i ++)
        {
            const Request::VirtualMachines& vms = (*i)->getVirtualMachines();
            for (Request::VirtualMachines::const_iterator i = vms.begin(); i != vms.end(); i ++, ++ iReq)
                reqCapacity[iReq] = (*i)->getCapacity();
        }
        for (Requests::iterator i = requests.begin(); i != requests.end(); i ++)
        {
            const Request::Storages& sts = (*i)->getStorages();
            for (Request::Storages::const_iterator i = sts.begin(); i != sts.end(); i ++, ++ iReq)
            {
                reqCapacity[iReq] = (*i)->getCapacity();
                reqTypes[iReqTypes] = (*i)->getTypeOfStore();
                ++ iReqTypes;
            }
        }

        graph = new InternalGraph(cnodes, cstores, vmCount, stCount, res, cap, reqCapacity, types, reqTypes);
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
            buildLink(ant);
        }

        // remember the best solution
        iMax = objFunctions();
        if (iMax < antNum && objValues[iMax] > bestValue)
        {
            delete bestPath;
            bestPath = new AntPath(*paths[iMax]);
            bestValue = objValues[iMax];
            std::cerr << "bestValue = " << bestValue << '\n';
        }

        graph->updatePheromone(paths, objValues);

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
    pt->addElement(new PathElement(0, 0));
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
    pt->addElement(new PathElement(0, 0));
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

bool AntAlgorithm::buildLink(unsigned int ant)
{
    return true;
}
