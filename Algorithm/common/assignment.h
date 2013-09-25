#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include "publicdefs.h"
#include "replication.h"

#include <string>
using std::string;

class Assignment
{
public:
    typedef std::map<Node *, Node *> NodeAssignments;
    typedef std::map<Store *, Store *> StoreAssignments;
    typedef std::map<Link *, NetPath> LinkAssignments;
    typedef std::set<Replication *> Replications;
public:
    Element * GetAssignment(Element *);
    Node * GetAssignment(Node *);
    Nodes GetAssigned(Node *);
    Store * GetAssignment(Store *);
    Stores GetAssigned(Store *);
    NetPath GetAssignment(Link *);
    Links GetAssigned(NetworkingElement *);
    
    Assignment()
    : request(NULL) {}
    Assignment(Request * r) { request = r; }
    string getName();

    // Get request
    Request* getRequest() const
    {
       return request;
    }

    ~Assignment(); // deleting only replications

    void AddAssignment(Node * w, Node * p)
    {
        nodeAssignments.insert(NodeAssignment(w, p));
    }

    void AddAssignment(Store * s, Store * m)
    {
        storeAssignments.insert(StoreAssignment(s, m));
    }

    void AddAssignment(Link * e, NetPath & path)
    {
        linkAssignments.insert(LinkAssignment(e, path));
    }

    // Remove assignments for any virtual resource
    void RemoveAssignment(Node * w)
    {
        nodeAssignments.erase(w);
    }

    void RemoveAssignment(Store * s)
    {
        storeAssignments.erase(s);
    }

    void RemoveAssignment(Link * e)
    {
        linkAssignments.erase(e);
    }

public:
    // Parsing replications
    void AddReplication(Replication* replication)
    {
        replications.insert(replication);
    }

    void RemoveReplication(Replication* replication)
    {
       replications.erase(replication);
    }

    // Check whether the store keeps the replica of the storage
    bool isReplicaOnStore(Storage * storage, Store * store);
    // Check whether the store keeps the replica of the storage but return false if not
    bool checkReplicaOnStore(Storage * storage, Store * store);

    // Set/Get the full set of replications of current request
    void setReplications(Replications& replications)
    {
       this->replications = replications;
    }

    Replications& getReplications() { return replications; }
private:
    NodeAssignments nodeAssignments;
    StoreAssignments storeAssignments;
    LinkAssignments linkAssignments;
    Replications replications;

    Request * request;
};

#endif // ASSIGNMENT_H
