#include "annealing.h"
#include "common/request.h"
#include "common/assignment.h"

#include "common/network.h"
#include "common/node.h"
#include "common/store.h"
#include "common/link.h"

#include "decentralized/virtualLinkRouter.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>
#include <stdio.h>

using namespace std;

Annealing::Annealing(Network * n, Requests const & r)
:
    Algorithm(n, r)
{}

Algorithm::Result Annealing::tryToAssignVM(Node * vm, Node * node)
{
    if (!node->isAssignmentPossible(*vm))
        return FAILURE;

    node->assign(*vm);
    currentAssignment->AddAssignment(vm, node);
    return SUCCESS;
}

Algorithm::Result Annealing::tryToAssignStorage(Store * storage, Store * store)
{
    if (!store->isAssignmentPossible(*storage))
        return FAILURE;

    store->assign(*storage);
    currentAssignment->AddAssignment(storage, store);
    return SUCCESS;
}

void Annealing::cleanUpAssignment(Assignment * assignment)
{
    assignment->forcedCleanup();
    delete assignment;
}

Algorithm::Result Annealing::generateVMAssignment(Request *request)
{
	Nodes &vm = request->getVirtualMachines();
	Nodes &nodes = Algorithm::network->getNodes();
	vector<Node *> vectorNodes(nodes.begin(), nodes.end());
	for (Nodes::iterator i = vm.begin(); i != vm.end(); ++i) {
		Result res = FAILURE;
		int counter = 0;
		Node * vm = *i;
		while (res == FAILURE && counter < 10000) {
			int i = rand() % nodes.size();
		    res = tryToAssignVM(vm, vectorNodes[i]);
		    ++counter;
		}
		if (res == FAILURE) {
			cleanUpAssignment(currentAssignment);
			return FAILURE;
		}
	}
	return SUCCESS;
}

Algorithm::Result Annealing::generateStorageAssignment(Request *request)
{
	Stores & storages = request->getStorages();
	Stores & stores = network->getStores();
	vector<Store *> vectorStores(stores.begin(), stores.end());
	for (Stores::iterator i = storages.begin(); i != storages.end(); ++i) {
		Result res = FAILURE;
		int counter = 0;
		Store * st = *i;
		while (res == FAILURE && counter < 10000) {
			int i = rand() % stores.size();
		    res = tryToAssignStorage(st, vectorStores[i]);
		    ++counter;
		}
		if (res == FAILURE) {
			cleanUpAssignment(currentAssignment);
			return FAILURE;
		}
	}
	return SUCCESS;
}

Algorithm::Result Annealing::generateAssignment(Request *request)
{
	if (currentAssignment) {
		cleanUpAssignment(currentAssignment);
	}
	*currentAssignment = Assignment(request);
	if (generateVMAssignment(request) == FAILURE) {
		return FAILURE;
	}
	if (generateStorageAssignment(request) == FAILURE) {
		return FAILURE;
	}	
	return SUCCESS;	
}

Algorithm::Result Annealing::generateCurAssignments()
{
    curAssignments.clear();
    for (Requests::iterator i = requests.begin(); i != requests.end(); i++)
    {
        Request *request = *i;
        Algorithm::Result result = FAILURE;
        if (generateAssignment(request) == SUCCESS) {
			curAssignments.insert(currentAssignment);
		}
	}
	return SUCCESS;
} 
     

Algorithm::Result Annealing::generatePrevAssignments()
{
    prevAssignments.clear();
    for (Requests::iterator i = requests.begin(); i != requests.end(); i++)
    {
        Request *request = *i;
        Algorithm::Result result = FAILURE;
        if (generateAssignment(request) == SUCCESS) {
			prevAssignments.insert(currentAssignment);
		}
	}
	return SUCCESS;
 } 

Algorithm::Result Annealing::schedule()
{
	Result result = FAILURE;
	while (result == FAILURE)
         result = generatePrevAssignments();
    assignments = prevAssignments;
	
	double delta = 0, temperature, start_temperature = 50;
	double p = 1; 
	int step = 0;
	do {
		//std::cout << "step " << step << std::endl;
		temperature = start_temperature / log(1 + step);
		for (int i = 0; i < 10; ++i) {
			result = FAILURE;
			while (result == FAILURE) {
			     result = generateCurAssignments();
			 }
			delta = prevAssignments.size() - curAssignments.size(); 
			double h = (double)rand() / RAND_MAX;
			p = exp(-delta / temperature);
			if (delta <= 0 || h > p) { // cтало лучше
			    prevAssignments = curAssignments;
			}
			if (prevAssignments.size() < assignments.size()) {
				assignments = prevAssignments;
			}
		}
		++step;
	} while (temperature > 7 && assignments.size() != requests.size() && step < 5000);
	
    printf("Assigned total of %d from %d requests", assignments.size(), requests.size());
    if ( assignments.size() == requests.size() )
        return SUCCESS;
    else if ( assignments.size() != 0 )
        return PARTIAL;
    else
        return FAILURE;
}

