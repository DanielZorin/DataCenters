#ifndef PATH_H
#define PATH_H

#include <vector>
#include <set>
#include "../common/element.h"
#include "../common/link.h"

// Class representing a single vertex in the path
struct PathElement
{
    // what request element this object corresponds to
    unsigned int request;
    // pointer to that request element
    Element * requestPointer;
    // what resource the request element is connected to
    unsigned int resource;
    // pointer to that resource
    Element * resourcePointer;
    // a list of virtual channels involving the request element
    std::set<Link *> * chan;

    PathElement(int req, Element * preq, int res, Element * pres, std::set<Link *> * ch = NULL)
    : request(req)
    , requestPointer(preq)
    , resource(res)
    , resourcePointer(pres)
    , chan(ch) // it is ok to copy a pointer, because these pointers were created in AntAlgorithm and exist until it is destroyed
    {
    }

    PathElement()
    : request(0)
    , requestPointer(NULL)
    , resource(0)
    , resourcePointer(NULL)
    , chan(NULL)
    {
    }

    ~PathElement() {}

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
    // Find and erase element with request == req and return resource number the request was assigned to
    int eraseRequest(unsigned int req);
    // Find element with requestPointer == ptr and return resource pointer and request number
    Element * findPointer(Element * ptr, int& req);
    // Find and erase element with requestPointer == ptr and return request and resource numbers
    int erasePointer(Element * ptr, int& req);

    // Getters
    const std::vector<PathElement *>& getPath() const { return path; }
    unsigned int getLength() const { return path.size(); }

    ~AntPath();
private:
    // Sequence of vertices
    std::vector<PathElement *> path;
};

#endif
