#ifndef PATH_H
#define PATH_H

#include <vector>
#include "../common/element.h"

// Class representing a single vertex in the path
struct PathElement
{
    // what request the vertex corresponds to
    unsigned int request;
    // pointer to that request
    Element * requestPointer;
    // what resource the vertex is connected to
    unsigned int resource;
    // pointer to that resource
    Element * resourcePointer;

    PathElement(int req, Element * preq, int res, Element * pres)
    : request(req)
    , requestPointer(preq)
    , resource(res)
    , resourcePointer(pres)
    {}

    PathElement()
    : request(0)
    , requestPointer(NULL)
    , resource(0)
    , resourcePointer(NULL)
    {}

    PathElement(const PathElement & p);
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
    AntPath& operator=(const AntPath & p);

    // Reserve elements
    bool setLength(int len);
    // Add an element
    void addElement(PathElement * element);
    // Erase an element
    void eraseElement(int index);
    // Find and erase element with request == req
    int eraseRequest(unsigned int req);

    // Getters
    const std::vector<PathElement *>& getPath() const { return path; }
    unsigned int getLength() const { return path.size(); }

    ~AntPath();
private:
    // Sequence of vertices
    std::vector<PathElement *> path;
};

#endif
