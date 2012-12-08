#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "internalgraph.h"
#include "../common/publicdefs.h"

/////////////////////////////////////////////////////////////////////////////////////////////
// Arc

Arc::Arc(const Arc & a)
{
    pher = a.pher;
    heur = a.heur;
}

Arc& Arc::operator=(const Arc & a)
{
    if (&a == this) return *this;
    pher = a.pher;
    heur = a.heur;
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// GraphComponent

GraphComponent::GraphComponent(unsigned long req, int phys, Element * pr, RequestType t, unsigned int sType)
: type(t)
, required(req)
, storageType(sType)
, request(pr)
{
    if (!init(phys)) success = false;
    else success = true;
}

GraphComponent::~GraphComponent()
{
    if (success)
        for (int i = 0; i < physArcs.size(); ++ i) delete physArcs[i];
}

GraphComponent::GraphComponent(const GraphComponent & gc)
{
    success = gc.success;
    type = gc.type;
    required = gc.required;

    physArcs.resize(gc.physArcs.size());
    for (int i = 0; i < physArcs.size(); ++ i)
    {
        if (gc.physArcs[i]) physArcs[i] = new Arc(*gc.physArcs[i]);
        else physArcs[i] = NULL;
    }
}

GraphComponent& GraphComponent::operator=(const GraphComponent & gc)
{
    if (&gc == this) return *this;
    for (int i = 0; i < physArcs.size(); ++ i) delete physArcs[i];

    physArcs.resize(gc.physArcs.size());
    success = gc.success;
    type = gc.type;
    required = gc.required;
    for (int i = 0; i < physArcs.size(); ++ i)
    {
        if (gc.physArcs[i]) physArcs[i] = new Arc(*gc.physArcs[i]);
        else physArcs[i] = NULL;
    }
    return *this;
}

bool GraphComponent::init(int num)
{
    int i = 0;

    try
    {
        if (num < 0) throw "Wrong arguments for graph component\n";

        physArcs.resize(num);
        physHeurs.resize(num);
        justHeurs.resize(num);
        initJustHeurs.resize(num);
        for (i = 0; i < num; ++ i)
            physArcs[i] = new Arc;

        std::cerr << "Created graph component, num = " << num << ", capacity = " << required << '(' << request->getCapacity() << "), type = " << storageType <<
                     ", pointer = " << request << '\n';
        return true;
    }
    catch (const char* s)
    {
        std::cerr << s;
        for (int j = 0; j < i; ++ i) delete physArcs[j];
        return false;
    }
    catch (std::bad_alloc)
    {
        std::cerr << "Error while allocating memory\n";
        for (int j = 0; j < i; ++ i) delete physArcs[j];
        return false;
    }
    catch (...)
    {
        std::cerr << "Unknown error\n";
        for (int j = 0; j < i; ++ i) delete physArcs[j];
        return false;
    }
}

void GraphComponent::nextPath()
{
    for (int i = 0; i < physArcs.size(); ++ i)
        physArcs[i]->heur = physHeurs[i];

    for (int i = 0; i < physArcs.size(); ++ i)
        justHeurs[i] = initJustHeurs[i];
}

void GraphComponent::initValues(std::vector<unsigned long> & res, std::vector<unsigned long> & cap, std::vector<unsigned int> & types)
{
    double maxHeur = 0;
    for (int i = 0; i < physArcs.size(); ++ i)
    {
        if (type == STORAGE && types[i] != storageType) justHeurs [i] = initJustHeurs[i] = physArcs[i]->heur = -1;
        else
        {
            physArcs[i]->pher = 1;
//            justHeurs[i] = initJustHeurs[i] = physArcs[i]->heur = cap[i]-res[i]+required;
            justHeurs[i] = initJustHeurs[i] = physArcs[i]->heur = res[i]-required;
            if (physArcs[i]->heur > cap[i] || physArcs[i]->heur < 0) justHeurs[i] = initJustHeurs[i] = physArcs[i]->heur = 0;
            if (physArcs[i]->heur > maxHeur) maxHeur = physArcs[i]->heur;
        }
    }

    if (!ZERO(maxHeur))
    {
        for (int i = 0; i < physArcs.size(); ++ i)
        {
            if (!(type == STORAGE && types[i] != storageType)) physArcs[i]->heur /= maxHeur;
            physHeurs[i] = physArcs[i]->heur;
//            std::cerr << "physArcs[" << i << "] = " << physArcs[i]->heur << ' ';
        }
    }
//    std::cerr << '\n';
}

void GraphComponent::updateHeuristic(unsigned int resNum, unsigned int resCur, unsigned int resCap)
{
    double maxHeur = 0;
    if (!(ZERO(physArcs[resNum]->heur+1)))
    {
//        justHeurs[resNum] = resCap-resCur+required;
        justHeurs[resNum] = resCur-required;
        if (justHeurs[resNum] > resCap || justHeurs[resNum] < 0) justHeurs[resNum] = 0;

        for (int i = 0; i < physArcs.size(); ++ i)
            if (justHeurs[i] > maxHeur) maxHeur = justHeurs[i];

//        std::cerr << "Updated with resNum = " << resNum << ", resCur = " << resCur << ", resCap = " << resCap << ", maxHeur = " << maxHeur << '\n';
        if (!ZERO(maxHeur))
        {
            for (int i = 0; i < physArcs.size(); ++ i)
            {
                if (!(ZERO(physArcs[i]->heur+1))) physArcs[i]->heur = justHeurs[i]/maxHeur;
//                std::cerr << "physArcs[" << i << "] = " << physArcs[i]->heur << ' ';
            }
        }
    }
//    std::cerr << '\n';
}

void GraphComponent::updatePheromone(unsigned int res, double value)
{
    double maxPher = 0;
    maxPher = (physArcs[res]->pher += value);
//    std::cerr << "physArcs[" << res << "] += " << value << ", maxPher = " << maxPher << '\n';
    // Normalize if needed
    if (maxPher > 1)
    {
        for (int i = 0; i < physArcs.size(); ++ i)
        {
            physArcs[i]->pher /= maxPher;
//            std::cerr << "physArcs[" << i << "] = " << physArcs[i]->pher << ' ';
        }
    }
//    std::cerr << '\n';
}

unsigned int GraphComponent::chooseResource(double pherDeg, double heurDeg)
{
    unsigned int size = physArcs.size();
    std::vector<double> roulette(size);
    double value = 0, sum = 0;
    for (unsigned int i = 0; i < size; i ++)
    {
        if (ZERO(physArcs[i]->heur+1)) value = 0;
        else value = pow(physArcs[i]->pher, pherDeg)*pow(physArcs[i]->heur, heurDeg);
        sum += value;
        roulette[i] = sum;
    }
    if (ZERO(sum)) return size+1;


    double choose = rand()/(double)RAND_MAX * sum;
/*    std::cerr << "physArcs = ";
    for (int q = 0; q < physArcs.size(); ++ q) std::cerr << physArcs[q]->heur << ' ';
    std::cerr << ", roulette = ";
    for (int q = 0; q < roulette.size(); ++ q) std::cerr << roulette[q] << ' ';
    std::cerr << ", sum = " << sum << ", choose = " << choose << '\n';*/
    for (unsigned int i = 0; i < size; ++ i)
        if (choose < roulette[i]) return i;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// InternalGraph

InternalGraph::InternalGraph(unsigned int nodes, unsigned int stores, unsigned int vm, unsigned int st, // request set and network parameters
                             std::vector<unsigned long> & res, // current physical resources capacity
                             std::vector<unsigned long> & cap, // maximum physical resources capacity
                             std::vector<unsigned long> & req, // resources demanded by requests
                             std::vector<unsigned int> & types, // physical store types
                             std::vector<unsigned int> & reqTypes, // virtual storage types
                             std::vector<Element *> & pn, // pointers to physical nodes
                             std::vector<Element *> & ps, // pointers to physical stores
                             std::vector<Element *> & virtElems // pointer to virtual requests
                             )
: nodesNum(nodes)
, storesNum(stores)
, vmNum(vm)
, stNum(st)
, heurDeg(0)
, pherDeg(0)
{
    srand((unsigned)time(NULL));
//    srand(0);
    if (!init(res, cap, req, reqTypes, pn, ps, virtElems)) success = false;
    else
    {
        success = true;
        initValues(req, types);
    }
}

void InternalGraph::requestErased(int resource, unsigned int request, GraphComponent::RequestType t)
{
    if (t == GraphComponent::VMACHINE)
    {
//        std::cerr << "deleting = " << request << ", curNodesRes[" << resource << "] = " << curNodesRes[resource] << '\n';
        curNodesRes[resource] += vertices[request-1]->getRequired();
        updateInternalHeuristic(resource, GraphComponent::VMACHINE);
//        std::cerr << "After: curNodesRes[" << resource << "] = " << curNodesRes[resource] << '\n';
    }

    else if (t == GraphComponent::STORAGE)
    {
//        std::cerr << "deleting = " << request << ", curStoresRes[" << resource << "] = " << curStoresRes[resource] << '\n';
        curStoresRes[resource] += vertices[request-1]->getRequired();
        updateInternalHeuristic(resource, GraphComponent::STORAGE);
//        std::cerr << "After: curStoresRes[" << resource << "] = " << curStoresRes[resource] << '\n';
    }

}

void InternalGraph::nextPath()
{
    for (int i = 0; i < nodesNum; ++ i)
    {
        if (curNodesRes[i] < 0) std::cerr << "!!!curNodesRes[" << i << "] < 0 !!!\n";
        curNodesRes[i] = nodesRes[i];
    }
    for (int i = 0; i < storesNum; ++ i)
    {
        if (curStoresRes[i] < 0) std::cerr << "!!!curStoresRes[" << i << "] < 0 !!!\n";
        curStoresRes[i] = storesRes[i];
    }

    for (int i = 0; i < vertices.size(); ++ i)
        vertices[i]->nextPath();
}

void InternalGraph::updatePheromone(std::vector<AntPath*> & paths, std::vector<double> & objValues, double evapRate)
{
/*
    for (int i = 0; i < paths.size(); ++ i)
    {
        std::cerr << "paths[" << i << "]: ";
        const std::vector<PathElement *> & path = paths[i]->getPath();
        for (int j = 0; j < path.size(); ++ j)
            std::cerr << '(' << path[j]->request <<  "," << path[j]->resource << ")-";

        std::cerr << '\n';
    }
*/
    for (int i = 0; i < arcs.size(); ++ i)
        for (int j = 0; j < arcs[i].size(); ++ j)
            arcs[i][j]->pher *= 1-evapRate;

    unsigned int from, to, tmp;
    double tmpMax = 0, maxPherST = 0, maxPherVM = 0;
//    std::cerr << "Updating.\n";
    for (int i = 0; i < paths.size(); ++ i)
    {
        const std::vector<PathElement *> & path = paths[i]->getPath();
//        std::cerr << i << ":\n";
        for (int j = 0; j < path.size()-1; ++ j)
        {
            to = path[j+1]->request;
            if (to == 0)
            {
                if (tmpMax > maxPherVM) maxPherVM = tmpMax;
                tmpMax = 0;
                continue;
            }
            from = path[j]->request;
            arcs[from][to]->pher += objValues[i];
//            std::cerr << "arcs[" << from << "][" << to << "] += " << objValues[i] << ", vertices[" << to-1 << "]->";
            vertices[to-1]->updatePheromone(path[j+1]->resource, objValues[i]);

            if (arcs[from][to]->pher > tmpMax) tmpMax = arcs[from][to]->pher;
//            std::cerr << ", tmpMax = " << tmpMax << ". ";
        }
        if (tmpMax > maxPherST) maxPherST = tmpMax;
        tmpMax = 0;
//        std::cerr << "\n\n";
    }

//    std::cerr << "maxPherVM = " << maxPherVM << " maxPherST = " << maxPherST << '\n';
    // Normalize if needed
    if (maxPherVM > 1)
    {
        for (int i = 1; i < vmNum+1; ++ i) arcs[0][i]->pher /= maxPherVM;
        for (int i = 0; i < vmNum+1; ++ i)
            for (int j = 0; j < vmNum+1; ++ j)
            {
                arcs[i][j]->pher /= maxPherVM;
//                std::cerr << "arcs[" << i << "][" << j << "] = " << arcs[i][j]->pher << ' ';
            }
    }
    if (maxPherST > 1)
    {
        for (int i = vmNum+1; i < vmNum+stNum+1; ++ i) arcs[0][i]->pher /= maxPherST;
        for (int i = vmNum+1; i < vmNum+stNum+1; ++ i)
            for (int j = vmNum+1; j < vmNum+stNum+1; ++ j)
            {
                arcs[i][j]->pher /= maxPherST;
//                std::cerr << "arcs[" << i << "][" << j << "] = " << arcs[i][j]->pher << ' ';
            }
    }
//    std::cerr << "\n\n";
}

void InternalGraph::updateInternalHeuristic(unsigned int resNum, GraphComponent::RequestType t)
{
    if (t == GraphComponent::VMACHINE)
    {
        for (int i = 0; i < vertices.size(); ++ i)
            if (vertices[i]->getType() == GraphComponent::VMACHINE)
            {
//                std::cerr << i << ":\n";
                vertices[i]->updateHeuristic(resNum, curNodesRes[resNum], nodesCap[resNum]);
            }
    }

    else if (t == GraphComponent::STORAGE)
    {
        for (int i = 0; i < vertices.size(); ++ i)
            if (vertices[i]->getType() == GraphComponent::STORAGE)
            {
//                std::cerr << i << ":\n";
                vertices[i]->updateHeuristic(resNum, curStoresRes[resNum], storesCap[resNum]);
            }
    }
}

unsigned int InternalGraph::selectVertex(AntPath* pt, unsigned int cur, std::set<unsigned int> & available, bool& s)
{
    // choose request
    unsigned int vertex = 0;
    unsigned int size = available.size();
    std::vector<double> roulette(size);
    double value = 0, sum = 0;
    unsigned int index = 0;
    for (std::set<unsigned int>::iterator i = available.begin(); i != available.end(); i ++, index ++)
    {
        value = pow(arcs[cur][*i]->pher, pherDeg)*pow(arcs[cur][*i]->heur, heurDeg);
        sum += value;
        roulette[index] = sum;
    }

    double choose = rand()/(double)RAND_MAX * sum;
/*
    std::cerr << "arcs [cur][q] = ";
    for (int q = 0; q < arcs[cur].size(); ++ q) std::cerr << arcs[cur][q]->pher << ' ';
    std::cerr << ", roulette = ";
    for (int q = 0; q < roulette.size(); ++ q) std::cerr << roulette[q] << ' ';
    std::cerr << ", sum = " << sum << ", choose = " << choose << '\n';
*/
    if (ZERO(sum))
    {
        std::set<unsigned int>::iterator sel = available.begin();
        vertex = *sel;
        available.erase(sel);
    }
    else
    {
        for (unsigned int i = 0; i < size; ++ i)
        {
            if (choose < roulette[i])
            {
                std::set<unsigned int>::iterator sel = available.begin();
                // set don't have += for iterators
                for (unsigned int j = i; j > 0; -- j) sel ++;
                vertex = *sel;
                available.erase(sel);
                break;
            }
        }
    }

    // choose resource
    GraphComponent * gc = vertices[vertex-1];
    unsigned int res = gc->chooseResource(pherDeg, heurDeg);
//    std::cerr << "cur = " << cur << ", vertex = " << vertex << ", resource = " << res << "...";
    if (res >= gc->getResNum())
    {
        // failed to choose a resource
        s = false;
//        std::cerr << "no\n";
        return vertex;
    }
//    std::cerr << "ok\n";

    if (gc->getType() == GraphComponent::VMACHINE)
    {
//        std::cerr << "cur = " << cur << '\n';
        curNodesRes[res] -= gc->getRequired();
        updateInternalHeuristic(res, GraphComponent::VMACHINE);
        pt->addElement(new PathElement(vertex, gc->getPointer(), res, physNodes[res]));
    }
    else if (gc->getType() == GraphComponent::STORAGE)
    {
//        std::cerr << "cur = " << cur << '\n';
        curStoresRes[res] -= gc->getRequired();
        updateInternalHeuristic(res, GraphComponent::STORAGE);
        pt->addElement(new PathElement(vertex, gc->getPointer(), res, physStores[res]));
    }

    s = true;
    return vertex;
}

void InternalGraph::clean(int i, int j, int k)
{
    for (int p = 0; p < i; ++ p) delete vertices[p];
    for (int p = 0; p <= j; ++ p)
    {
        if (p == j && j != vmNum+stNum+1)
        {
            for (int q = 0; q < k; ++ q) delete arcs[p][q];
        }
        else if (p < j)
        {
            for (int q = 0; q < vmNum+stNum+1; ++ q) delete arcs[p][q];
        }
        else break;
    }
}

bool InternalGraph::init(std::vector<unsigned long> & res, std::vector<unsigned long> & cap, std::vector<unsigned long> & req,
                         std::vector<unsigned int> & reqTypes, std::vector<Element *> & pn, std::vector<Element *> & ps,
                         std::vector<Element *> & virtElems)
{
    int i = 0, j = 0, k = 0;

    try
    {
        if (vmNum < 0 || stNum < 0 || nodesNum < 0 || storesNum < 0) throw "Wrong arguments for internal graph\n";

        vertices.resize(vmNum+stNum);
        for (i = 0; i < vmNum; ++ i)
        {
            vertices[i] = new GraphComponent(req[i], nodesNum, virtElems[i], GraphComponent::VMACHINE);
            if (!vertices[i]->isCreated())
            {
                for (int p = 0; p < i; ++ p) delete vertices[p];
                return false;
            }
        }
        for (i = vmNum; i < vmNum+stNum; ++ i)
        {
            vertices[i] = new GraphComponent(req[i], storesNum, virtElems[i], GraphComponent::STORAGE, reqTypes[i-vmNum]);
            if (!vertices[i]->isCreated())
            {
                for (int p = 0; p < i; ++ p) delete vertices[p];
                return false;
            }
        }

        arcs.resize(vmNum+stNum+1);
        for (j = 0; j < vmNum+stNum+1; ++ j)
        {
            arcs[j].resize(vmNum+stNum+1);
            for (k = 0; k < vmNum+stNum+1; ++ k) arcs[j][k] = new Arc;
        }

        nodesRes.resize(nodesNum);
        curNodesRes.resize(nodesNum);
        nodesCap.resize(nodesNum);
        physNodes.resize(nodesNum);
        for (int p = 0; p < nodesNum; ++ p)
        {
            curNodesRes[p] = res[p];
            nodesRes[p] = res[p];
            nodesCap[p] = cap[p];
            physNodes[p] = pn[p];
        }
        storesRes.resize(storesNum);
        curStoresRes.resize(storesNum);
        storesCap.resize(storesNum);
        physStores.resize(storesNum);
        for (int p = 0; p < storesNum; ++ p)
        {
            curStoresRes[p] = res[p+nodesNum];
            storesRes[p] = res[p+nodesNum];
            storesCap[p] = cap[p+nodesNum];
            physStores[p] = ps[p];
        }

        std::cerr << "Created graph, values: nodes = " << nodesNum << ", stores = " << storesNum << ", vms = " << vmNum << ", sts =  " << stNum << "\nResources:\n";
        for (int p = 0; p < nodesRes.size(); ++ p) std::cerr << nodesRes[p] << '(' << physNodes[p]->getCapacity() << ") ";
        std::cerr << '\n';
        for (int p = 0; p < storesRes.size(); ++ p) std::cerr << storesRes[p] << '(' << physStores[p]->getCapacity() << ") ";
        std::cerr << "\nCapacities:\n";
        for (int p = 0; p < nodesCap.size(); ++ p) std::cerr << nodesCap[p] << '(' << physNodes[p]->getMaxCapacity() << ") ";
        std::cerr << '\n';
        for (int p = 0; p < storesCap.size(); ++ p) std::cerr << storesCap[p] << '(' << physStores[p]->getMaxCapacity() << ") ";
        std::cerr << '\n';
        return true;
    }
    catch (const char* s)
    {
        std::cerr << s;
        clean(i, j, k);
        return false;
    }
    catch (std::bad_alloc)
    {
        std::cerr << "Error while allocating memory\n";
        clean(i, j, k);
        return false;
    }
    catch (...)
    {
        std::cerr << "Unknown error\n";
        clean(i, j, k);
        return false;
    }
}

void InternalGraph::initValues(std::vector<unsigned long> & req, std::vector<unsigned int> & types)
{
    unsigned long maxVM = 0, maxST = 0;
    for (int i = 0; i < vmNum; ++ i)
        if (req[i] > maxVM) maxVM = req[i];
    for (int i = vmNum; i < vmNum+stNum; ++ i)
        if (req[i] > maxST) maxST = req[i];

    for (int i = 1; i <= vmNum; ++ i)
    {
        arcs[0][i]->heur = req[i-1]/(double)maxVM;
        arcs[0][i]->pher = 1;
    }
    for (int i = vmNum+1; i <= vmNum+stNum; ++ i)
    {
        arcs[0][i]->heur = req[i-1]/(double)maxST;
        arcs[0][i]->pher = 1;
    }

    for (int i = 1; i <= vmNum; ++ i)
    {
        for (int j = 1; j <= vmNum; ++ j)
        {
            if (i == j) continue;
            arcs[i][j]->pher = 1;
            arcs[i][j]->heur = req[j-1]/(double)maxVM;
//            std::cerr << "arcs[" << i << "][" << j << "]->heur = " << arcs[i][j]->heur << ' ';
        }
    }

    for (int i = vmNum+1; i <= vmNum+stNum; ++ i)
    {
        for (int j = vmNum+1; j <= vmNum+stNum; ++ j)
        {
            if (i == j) continue;
            arcs[i][j]->pher = 1;
            arcs[i][j]->heur = req[j-1]/(double)maxST;
//            std::cerr << "arcs[" << i << "][" << j << "]->heur = " << arcs[i][j]->heur << ' ';
        }
    }
//    std::cerr << '\n';

    // Init for each graph component
    for (int i = 0; i < vertices.size(); ++ i)
    {
        if (vertices[i]->getType() == GraphComponent::VMACHINE) vertices[i]->initValues(nodesRes, nodesCap, types);
        else if (vertices[i]->getType() == GraphComponent::STORAGE) vertices[i]->initValues(storesRes, storesCap, types);
    }
}

InternalGraph::~InternalGraph()
{
    if (success)
    {
        for (int i = 0; i < vertices.size(); ++ i) delete vertices[i];
        for (int i = 0; i < arcs.size(); ++ i)
            for (int j = 0; j < arcs[i].size(); ++ j) delete arcs[i][j];
    }
}
