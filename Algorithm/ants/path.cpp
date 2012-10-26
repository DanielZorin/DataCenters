#include "path.h"

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
    for (int i = 0; i < max; ++ i) path[i] = NULL;
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
    path.reserve(p.path.size());
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
    for (int i = oldLength; i < len; ++ i) path[i] = NULL;
    return true;
}