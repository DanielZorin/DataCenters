#include "path.h"
// To add NULL constant definition independently
#include <cstddef> 

PathElement::PathElement(const PathElement & p)
{
    request = p.request;
    resource = p.resource;
}

PathElement& PathElement::operator=(const PathElement & p)
{
    if (&p == this) return *this;
    request = p.request;
    resource = p.resource;
    return *this;
}

AntPath::AntPath(int max)
{
    path.reserve(max);
}

AntPath::~AntPath()
{
    for (int i = 0; i < path.size(); ++ i)
    {
        if (path[i] == NULL) break;
        delete path[i];
    }
}

AntPath::AntPath(const AntPath & p)
{
    path.resize(p.path.size());
    for (int i = 0; i < path.size(); ++ i)
    {
        if (p.path[i]) path[i] = new PathElement(*p.path[i]);
        else path[i] = NULL;
    }
}

AntPath& AntPath::operator=(const AntPath & p)
{
    if (&p == this) return *this;
    for (int i = 0; i < path.size(); ++ i)
    {
        if (path[i] == NULL) break;
        delete path[i];
    }

    path.resize(p.path.size());
    for (int i = 0; i < path.size(); ++ i)
    {
        if (p.path[i]) path[i] = new PathElement(*p.path[i]);
        else path[i] = NULL;
    }
    return *this;
}

bool AntPath::setLength(int len)
{
    unsigned int oldLength = path.size();
    if (len < oldLength) return false;

    path.reserve(len);
    return true;
}

void AntPath::addElement(PathElement * element)
{
    path.push_back(element);
}

int AntPath::eraseRequest(unsigned int req)
{
    for (int i = 0; i < path.size(); ++ i)
    {
        if (path[i]->request == req)
        {
            int res = path[i]->resource;
            eraseElement(i);
            return res;
        }
    }
    return -1;
}

void AntPath::eraseElement(int index)
{
    if (index < path.size()) path.erase(path.begin()+index);
}
