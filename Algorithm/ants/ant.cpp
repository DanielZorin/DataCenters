#include "ant.h"

AntAlgorithm::AntAlgorithm(Network * n, Requests const & r)
: Algorithm(n, r)
, vmCount(0)
, stCount(0)
{
    if (!init()) success = false;
    else success = true;
}

AntAlgorithm::~AntAlgorithm()
{
    delete graph;
}

bool AntAlgorithm::init()
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
    std::vector<unsigned long> nodesRes(cnodes);
    std::vector<unsigned long> storesRes(cstores);

    // get physical resources' capacity
    int iRes = 0;
    for (Nodes::iterator i = nodes.begin(); i != nodes.end(); i ++, ++ iRes)
        nodesRes[iRes] = (*i)->getCapacity();
    iRes = 0;
    for (Stores::iterator i = stores.begin(); i != stores.end(); i ++, ++ iRes)
        storesRes[iRes] = (*i)->getCapacity();

    // get requests' required capacity
    std::vector<unsigned long> reqCapacity(vmCount+stCount);
    int iReq = 0;
    for (Requests::iterator i = requests.begin(); i != requests.end(); i ++)
    {
        const Request::VirtualMachines& vms = (*i)->getVirtualMachines();
        for (Request::VirtualMachines::iterator i = vms.begin(); i != vms.end(); i ++, ++ iReq)
            reqCapacity[iReq] = (*i)->getCapacity();
    }
    for (Requests::iterator i = requests.begin(); i != requests.end(); i ++)
    {
        const Request::Storages& sts = (*i)->getStorages();
        for (Request::Storages::iterator i = sts.begin(); i != sts.end(); i ++, ++ iReq)
            reqCapacity[iReq] = (*i)->getCapacity();
    }

    graph = new InternalGraph(cnodes, cstores, vmCount, stCount, nodesRes, storesRes, reqCapacity);
}

Algorithm::Result AntAlgorithm::schedule()
{
    return Algorithm::SUCCESS;
}

bool AntAlgorithm::buildPath()
{
}

bool AntAlgorithm::buildLink()
{
}
