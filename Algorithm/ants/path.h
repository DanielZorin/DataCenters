#ifndef PATH_H
#define PATH_H

#include <vector>

// Class representing a single vertex in the path
struct PathElement
{
    // what request the vertex corresponds to
    unsigned int request;
    // what resource the vertex is connected to
    unsigned int resource;

    PathElement(int req, int res)
    : request(req)
    , resource(res)
    {}

    PathElement()
    : request(0)
    , resource(0)
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
