#ifndef ANNEALING_H
#define ANNEALING_H

#include "common/publicdefs.h"
#include "common/algorithm.h"
#include "common/network.h"

class Annealing : public Algorithm
{
private:
    Annealing();
    
    Result tryToAssignVM(Node * vm, Node * node);
    Result tryToAssignStorage(Store * storage, Store * store);
    
    void cleanUpAssignment(Assignment * assignment);
    
    Result generateVMAssignmentCurNetwork(Request *request);
    Result generateStorageAssignmentCurNetwork(Request *request);
    
    Result generateVMAssignmentPrevNetwork(Request *request);
    Result generateStorageAssignmentPrevNetwork(Request *request);
    
    Result generateAssignmentCurNetwork(Request *request);
    Result generateAssignmentPrevNetwork(Request *request);
    
    Result generateCurAssignments();
    Result generatePrevAssignments();
    
    Result tryToInsertNewAssignment();
    Result changeAssignments();
    Result changeCurAssignments();
        //virtual Assignments getAssignments() { return prevAssignments; }
       // virtual Network & getNetwork() { return *network; }
    
public:
    Annealing(Network * n, Requests const & r);

    virtual ~Annealing() {}
    virtual Algorithm::Result schedule();

    Assignment *currentAssignment;
    Assignments curAssignments;
	Assignments prevAssignments;
	
	Network curNetwork;
	Network prevNetwork;
};

#endif
