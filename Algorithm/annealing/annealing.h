#ifndef ANNEALING_H
#define ANNEALING_H

#include "common/publicdefs.h"
#include "common/algorithm.h"

class Annealing : public Algorithm
{
private:
    Annealing();
    
    Result tryToAssignVM(Node * vm, Node * node);
    Result tryToAssignStorage(Store * storage, Store * store);
    
    void cleanUpAssignment(Assignment * assignment);
    
    Result generateVMAssignment(Request *request, Network *cnetwork);
    Result generateStorageAssignment(Request *request, Network *cnetwork);
    
    Result generateAssignment(Request *request, Network *cnetwork);
    
    Result generateCurAssignments();
    Result generatePrevAssignments();
    
    Result tryToInsertNewAssignment();
    Result changeAssignments();
    Result changeCurAssignments();
    
public:
    Annealing(Network * n, Requests const & r);

    virtual ~Annealing() {}
    virtual Algorithm::Result schedule();

    Assignment *currentAssignment;
    Assignments curAssignments;
	Assignments prevAssignments;
	
	Network *curNetwork;
	Network *prevNetwork;
};

#endif
