#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <vector>
#include <map>
#include "internalgraph.h"
#include "../common/publicdefs.h"

Heuristic * GraphComponent::heurCalc = NULL;

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

GraphComponent::GraphComponent(unsigned long req, int phys, Element * pr, RequestType t, unsigned int sType, unsigned int ramReq)
: type(t)
, required(req)
, ramRequired(ramReq)
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
    ramRequired = gc.ramRequired;

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
    ramRequired = gc.ramRequired;
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
            justHeurs[i] = initJustHeurs[i] = physArcs[i]->heur = heurCalc->calculate(res[i], required, cap[i]);
            if (physArcs[i]->heur > maxHeur) maxHeur = physArcs[i]->heur;
        }
    }

    if (!ZERO(maxHeur))
    {
        for (int i = 0; i < physArcs.size(); ++ i)
        {
            if (!(type == STORAGE && types[i] != storageType)) physArcs[i]->heur /= maxHeur;
            physHeurs[i] = physArcs[i]->heur;
        }
    }
}

void GraphComponent::updateHeuristic(unsigned int resNum, unsigned int resCur, unsigned int resCap)
{
    double maxHeur = 0;
    if (!(ZERO(physArcs[resNum]->heur+1)))
    {
//        std::cerr << "before justHeurs[" << resNum << "] = " << justHeurs[resNum] << ", resCur = " << resCur << ", required = " << required << ", after ";
        justHeurs[resNum] = heurCalc->calculate(resCur, required, resCap);

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
        else physArcs[resNum]->heur = 0;
    }
//    std::cerr << '\n';
}

void GraphComponent::updatePheromone(unsigned int res, double value)
{
    double maxPher = 0;
    maxPher = (physArcs[res]->pher += value);
    // Normalize if needed
    if (maxPher > 1)
        for (int i = 0; i < physArcs.size(); ++ i)
            physArcs[i]->pher /= maxPher;
}

unsigned int GraphComponent::chooseResource(std::vector<unsigned long> & ram, double pherDeg, double heurDeg)
{
    unsigned int size = physArcs.size();
    std::vector<double> roulette(size);
    double value = 0, sum = 0;
    for (unsigned int i = 0; i < size; i ++)
    {
        if (ZERO(physArcs[i]->heur+1) || ZERO(physArcs[i]->heur) || physArcs[i]->heur < 0 ||
           (type == VMACHINE && ram[i] < ramRequired)) value = 0;
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
    return size+1;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// InternalGraph

InternalGraph::InternalGraph(unsigned int nodes, unsigned int stores, unsigned int vm, unsigned int st, // request set and network parameters
                             std::vector<unsigned long> & res, // current physical resources capacity
                             std::vector<unsigned long> & cap, // maximum physical resources capacity
                             std::vector<unsigned long> & ramRes, // current virtual machine memory capacity
                             std::vector<unsigned long> & ramCap, // maximum virtual machine memory capacity
                             std::vector<unsigned long> & ramReq, // amount of memory requested by virtual machines
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
//    srand((unsigned)time(NULL));
    srand(2);

    GraphComponent::heurCalc = new MoreResourceFirst(0);
    if (!init(res, cap, ramRes, ramCap, req, ramReq, reqTypes, pn, ps, virtElems)) success = false;
    else
    {
        success = true;
        initValues(req, types);
    }
}

void InternalGraph::requestErased(int resource, unsigned int request, GraphComponent::RequestType t, bool update)
{
    if (t == GraphComponent::VMACHINE)
    {
        curNodesRes[resource] += vertices[request-1]->getRequired();
        curNodesRam[resource] += vertices[request-1]->getRequiredRam();
        if (update) updateInternalHeuristic(resource, GraphComponent::VMACHINE);
    }

    else if (t == GraphComponent::STORAGE)
    {
        curStoresRes[resource] += vertices[request-1]->getRequired();
        if (update) updateInternalHeuristic(resource, GraphComponent::STORAGE);
    }

}

void InternalGraph::nextPath()
{
    for (int i = 0; i < nodesNum; ++ i)
    {
        assert(curNodesRes[i] >= 0);
        assert(curNodesRam[i] >= 0);
        curNodesRes[i] = nodesRes[i];
        curNodesRam[i] = nodesRam[i];
    }
    for (int i = 0; i < storesNum; ++ i)
    {
        assert(curStoresRes[i] >= 0);
        curStoresRes[i] = storesRes[i];
    }

    for (int i = 0; i < vertices.size(); ++ i)
        vertices[i]->nextPath();
}

void InternalGraph::assignPath(AntPath * pt)
{
    GraphComponent * gc;
    const std::vector<PathElement *> & path = pt->getPath();
    for (int i = 0; i < path.size(); ++ i)
    {
        if (path[i]->request == 0) continue;
        gc = vertices[path[i]->request-1];
        if (gc->getType() == GraphComponent::VMACHINE)
        {
            curNodesRes[path[i]->resource] -= gc->getRequired();
            curNodesRam[path[i]->resource] -= gc->getRequiredRam();
        }
        else if (gc->getType() == GraphComponent::STORAGE) curStoresRes[path[i]->resource] -= gc->getRequired();
    }
}

void InternalGraph::updatePheromone(std::vector<AntPath*> & paths, std::vector<double> & objValues, double evapRate, double max)
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
        if (objValues[i] < max) continue;
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

unsigned int InternalGraph::selectVertex(AntPath* pt, unsigned int cur, std::set<unsigned int> & available, bool& s, std::map<Element *, std::set<Link *> >& chan)
{
    // choose request
    unsigned int vertex = 0;
    unsigned int size = available.size();
    std::vector<double> roulette(size);
    std::vector<unsigned int> rouletteIndex(size);
    double value = 0, sum = 0;
    unsigned int index = 0;
    for (std::set<unsigned int>::iterator i = available.begin(); i != available.end(); i ++, index ++)
    {
        value = pow(arcs[cur][*i]->pher, pherDeg)*pow(arcs[cur][*i]->heur, heurDeg);
        sum += value;
        roulette[index] = sum;
        rouletteIndex[index] = *i;
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
                vertex = rouletteIndex[i];
                unsigned int res = available.erase(rouletteIndex[i]);
                assert(res == 1);
                break;
            }
        }
    }

// choose resource
    GraphComponent * gc = vertices[vertex-1];
    unsigned int res = gc->chooseResource(curNodesRam, pherDeg, heurDeg);
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
        curNodesRam[res] -= gc->getRequiredRam();
        updateInternalHeuristic(res, GraphComponent::VMACHINE);
        std::map<Element *, std::set<Link *> >::iterator iter = chan.find(gc->getPointer());
        std::set<Link *> * chanPtr = (iter != chan.end()) ? &(iter->second) : NULL;
        pt->addElement(new PathElement(vertex, gc->getPointer(), res, physNodes[res], chanPtr));
    }
    else if (gc->getType() == GraphComponent::STORAGE)
    {
//        std::cerr << "cur = " << cur << '\n';
        curStoresRes[res] -= gc->getRequired();
        updateInternalHeuristic(res, GraphComponent::STORAGE);
        std::map<Element *, std::set<Link *> >::iterator iter = chan.find(gc->getPointer());
        std::set<Link *> * chanPtr = (iter != chan.end()) ? &(iter->second) : NULL;
        pt->addElement(new PathElement(vertex, gc->getPointer(), res, physStores[res], chanPtr));
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

bool InternalGraph::init(std::vector<unsigned long> & res, std::vector<unsigned long> & cap, std::vector<unsigned long> & ramRes, std::vector<unsigned long> & ramCap,
                         std::vector<unsigned long> & req, std::vector<unsigned long> & ramReq, std::vector<unsigned int> & reqTypes, std::vector<Element *> & pn,
                         std::vector<Element *> & ps, std::vector<Element *> & virtElems)
{
    int i = 0, j = 0, k = 0;

    try
    {
        if (vmNum < 0 || stNum < 0 || nodesNum < 0 || storesNum < 0) throw "Wrong arguments for internal graph\n";

        requestHeur = new LargeRequestFirst(0);

        vertices.resize(vmNum+stNum);
        for (i = 0; i < vmNum; ++ i)
        {
            vertices[i] = new GraphComponent(req[i], nodesNum, virtElems[i], GraphComponent::VMACHINE, 0, ramReq[i]);
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
        nodesRam.resize(nodesNum);
        curNodesRes.resize(nodesNum);
        curNodesRam.resize(nodesNum);
        nodesCap.resize(nodesNum);
        nodesCapRam.resize(nodesNum);
        physNodes.resize(nodesNum);
        for (int p = 0; p < nodesNum; ++ p)
        {
            curNodesRes[p] = res[p];
            nodesRes[p] = res[p];
            curNodesRam[p] = ramRes[p];
            nodesRam[p] = ramRes[p];
            nodesCap[p] = cap[p];
            nodesCapRam[p] = ramCap[p];
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

        std::cerr << "Created graph, values: nodes = " << nodesNum << ", stores = " << storesNum << ", vms = " << vmNum << ", sts = " << stNum << '\n';
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
        arcs[0][i]->heur = requestHeur->calculate(req[i-1], maxVM);
        arcs[0][i]->pher = 1;
    }
    for (int i = vmNum+1; i <= vmNum+stNum; ++ i)
    {
        arcs[0][i]->heur = requestHeur->calculate(req[i-1], maxST);
        arcs[0][i]->pher = 1;
    }

    for (int i = 1; i <= vmNum; ++ i)
    {
        for (int j = 1; j <= vmNum; ++ j)
        {
            if (i == j) continue;
            arcs[i][j]->pher = 1;
            arcs[i][j]->heur = requestHeur->calculate(req[j-1], maxVM);
        }
    }

    for (int i = vmNum+1; i <= vmNum+stNum; ++ i)
    {
        for (int j = vmNum+1; j <= vmNum+stNum; ++ j)
        {
            if (i == j) continue;
            arcs[i][j]->pher = 1;
            arcs[i][j]->heur = requestHeur->calculate(req[j-1], maxST);
        }
    }

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
