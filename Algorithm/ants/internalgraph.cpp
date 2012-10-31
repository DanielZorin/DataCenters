#include <iostream>
#include "internalgraph.h"

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

GraphComponent::GraphComponent(unsigned long req, int phys, RequestType t)
: type(t)
, required(req)
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
        for (i = 0; i < num; ++ i)
            physArcs[i] = new Arc;

        std::cerr << "Created graph component, num = " << num << ", capacity = " << required << '\n';
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

void GraphComponent::updateHeuristic(std::vector<unsigned long> & res)
{
    std::cerr << "updateHeuristic, required = " << required << '\n';
}

/////////////////////////////////////////////////////////////////////////////////////////////
// InternalGraph

InternalGraph::InternalGraph(unsigned int nodes, unsigned int stores, unsigned int vm, unsigned int st,
                             std::vector<unsigned long> & ndRes, std::vector<unsigned long> & stRes, std::vector<unsigned long> & req)
: nodesNum(nodes)
, storesNum(stores)
, vmNum(vm)
, stNum(st)
{
    if (!init(ndRes, stRes, req)) success = false;
    else
    {
        success = true;
        calcHeuristic(req);
    }
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

bool InternalGraph::init(std::vector<unsigned long> & ndRes, std::vector<unsigned long> & stRes, std::vector<unsigned long> & req)
{
    int i = 0, j = 0, k = 0;

    try
    {
        if (vmNum < 0 || stNum < 0 || nodesNum < 0 || storesNum < 0) throw "Wrong arguments for internal graph\n";

        vertices.resize(vmNum+stNum);
        for (i = 0; i < vmNum; ++ i)
        {
            vertices[i] = new GraphComponent(req[i], nodesNum, GraphComponent::VMACHINE);
            if (!vertices[i]->isCreated())
            {
                for (int p = 0; p < i; ++ p) delete vertices[p];
                return false;
            }
        }
        for (i = vmNum; i < vmNum+stNum; ++ i)
        {
            vertices[i] = new GraphComponent(req[i], storesNum, GraphComponent::STORAGE);
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
        nodesRes.swap(ndRes);
        storesRes.resize(storesNum);
        storesRes.swap(stRes);

        std::cerr << "Created graph, values: " << nodesNum << " " << storesNum << " " << vmNum << " " << stNum << "\nResources:\n";
        for (int i = 0; i < nodesRes.size(); ++ i) std::cerr << nodesRes[i] << ' ';
        std::cerr << '\n';
        for (int i = 0; i < storesRes.size(); ++ i) std::cerr << storesRes[i] << ' ';
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

void InternalGraph::calcHeuristic(std::vector<unsigned long> & req)
{
    std::cerr << "Heuristic is calculated with capacities: ";
    for (int i = 0; i < req.size(); ++ i) std::cerr << req[i] << ' ';
    std::cerr << '\n';
    updateInternalHeuristic();
}

void InternalGraph::updateInternalHeuristic()
{
    for (int i = 0; i < vertices.size(); ++ i)
    {
        if (vertices[i]->getType() == GraphComponent::VMACHINE) vertices[i]->updateHeuristic(nodesRes);
        else if (vertices[i]->getType() == GraphComponent::STORAGE) vertices[i]->updateHeuristic(storesRes);
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
