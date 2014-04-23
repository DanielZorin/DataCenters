#ifndef ANNEALING_H
#define ANNEALING_H

#include "common/publicdefs.h"
#include "common/algorithm.h"
#include "common/network.h"
#include "common/assignment.h"

class Assignment;

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
    
    Result changeAssignments();
    Result changeCurAssignments();
    
    void printAssignments(Assignments a);
    void printRequests(void);
    void printAssignment(Assignment *a);
    void printUnassignedRequests();
    
    //virtual Assignments getAssignments() { return Annealing::bestAssignments; }
    virtual Network & getNetwork() { return bestNetwork; }
    
    void copyPrevAssignmentsToCur();
    void copyPrevAssignmentsToBest();
    void copyCurAssignmentsToBest();
    void copyCurAssignmentsToPrev();
    
    Result deleteAssignmentFromCur();
    Result tryToInsertNewAssignment();
    Result reassignRandRequest();
    
    void mutation();
    
    Assignment *copyAssignmentToCurrent(Assignment *a);
     
public:
    Annealing(Network * n, Requests const & r);
    int kol;
    virtual ~Annealing() {}
    virtual Algorithm::Result schedule();

    Assignment *currentAssignment;
    Assignments curAssignments;
	Assignments prevAssignments;
	//Assignments bestAssignments;
	
    int state; //1, 2, or 3
	int counterFail;
	
	Network curNetwork;
	Network prevNetwork;
	Network bestNetwork;
};

#endif