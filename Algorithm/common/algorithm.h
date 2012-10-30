#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <set>

class Request;
class Network;
class Assignment;

class Algorithm
{
public:
    enum Result {
        SUCCESS = 0,
        PARTIAL,
        FAILURE = -1
    };
public:
    typedef std::set<Request *> Requests;
    typedef std::set<Assignment *> Assignments;

private:
    Algorithm();

public:
    Algorithm(Network * n, Requests const & r)
        : network(n)
        , requests(r)
    {}
public:
    virtual Assignments getAssignments() { return assignments; }
public:
    virtual Result schedule() = 0;
protected:
    Network * network;
    Requests requests;
    Assignments assignments;
};

#endif // ALGORITHM_H
