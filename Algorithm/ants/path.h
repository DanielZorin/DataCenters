#ifndef PATH_H
#define PATH_H

#include <vector>

// Class representing a single vertex in the path
struct PathElement
{
    // what request the vertex corresponds to
    int request;
    // what resource the vertex is connected to
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

// Class representing a single path in the internal graph
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
    // Sequence of vertices
    std::vector<PathElement *> path;
};

#endif
