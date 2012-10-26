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

GraphComponent::GraphComponent(int phys, RequestType t)
: type(t)
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

    physArcs.reserve(gc.physArcs.size());
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

        physArcs.reserve(num);
        for (i = 0; i < num; ++ i)
            physArcs[i] = new Arc;

        std::cerr << "Created graph component, num = " << num << '\n';
        return true;
    }
    catch (const char* s)
    {
        std::cerr << s;
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

/////////////////////////////////////////////////////////////////////////////////////////////
// InternalGraph

InternalGraph::InternalGraph(unsigned int nodes, unsigned int stores, unsigned int vm, unsigned int st)
: nodesNum(nodes)
, storesNum(stores)
, vmNum(vm)
, stNum(st)
{
    if (!init()) success = false;
    else success = true;
}


bool InternalGraph::init()
{
    int i = 0, j = 0, k = 0;

    try
    {
        if (vmNum < 0 || stNum < 0 || nodesNum < 0 || storesNum < 0) throw "Wrong arguments for internal graph\n";

        vertices.reserve(vmNum+stNum+1);
        for (i = 0; i < vmNum; ++ i)
        {
            vertices[i] = new GraphComponent(nodesNum, GraphComponent::VMACHINE);
            if (!vertices[i]->isCreated())
            {
                for (int p = 0; p < i; ++ p) delete vertices[p];
                return false;
            }
        }
        for (i = vmNum; i < vmNum+stNum+1; ++ i)
        {
            vertices[i] = new GraphComponent(storesNum, GraphComponent::STORAGE);
            if (!vertices[i]->isCreated())
            {
                for (int p = 0; p < i; ++ p) delete vertices[p];
                return false;
            }
        }

        arcs.reserve(vmNum+stNum+1);
        for (j = 0; j < vmNum+stNum+1; ++ j)
        {
            arcs[j].reserve(vmNum+stNum+1);
            for (k = 0; k < vmNum+stNum+1; ++ k) arcs[j][k] = new Arc;
        }

        resources.reserve(nodesNum+storesNum);

        std::cerr << "Created graph, values: " << nodesNum << " " << storesNum << " " << vmNum << " " << stNum << '\n';
        return true;
    }
    catch (const char* s)
    {
        std::cerr << s;
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
        return false;
    }
    catch (...)
    {
        std::cerr << "Unknown error\n";
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
        return false;
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
