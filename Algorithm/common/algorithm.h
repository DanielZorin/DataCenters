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
        SUCCESS = 0x01,
        FAILURE = 0x02,
        PARTIAL = SUCCESS | FAILURE
    };
public:
    typedef std::set<Request *> Requests;
    typedef std::set<Assignment *> Assignments;

private:
    Algorithm();

public:
    Algorithm(Network * n, Requests const & r);
    virtual ~Algorithm();
public:
    virtual Assignments getAssignments() { return assignments; }
    virtual Network & getNetwork() { return *network; }
public:
    virtual Result schedule() = 0;
protected:
    Network * network;
    Requests requests;
    Assignments assignments;
};

#endif // ALGORITHM_H
