#ifndef PATH_H
#define PATH_H

#include <vector>

struct PathElement
{
    int request;
    int resource;

    PathElement(int req, int res)
    : request(req)
    , resource(res)
    {}

    PathElement()
    : request(-1)
    , resource(-1)
    {}

    PathElement(const PathElement& p);
    PathElement& operator=(const PathElement & p);
    //default destructor
};

class AntPath
{
public:
    AntPath(int max);
    AntPath(const AntPath & p);
    AntPath() {}
    AntPath& operator=(const AntPath& p);

    bool setLength(int len);

    ~AntPath();
private:
    std::vector<PathElement *> path;
};

#endif
