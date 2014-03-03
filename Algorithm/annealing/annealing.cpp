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
	//cout << "		generate VM assignment" << endl;
	Nodes &vm = request->getVirtualMachines();
	Nodes &nodes = Algorithm::network->getNodes();
	vector<Node *> vectorNodes(nodes.begin(), nodes.end());
	for (Nodes::iterator iter = vm.begin(); iter != vm.end(); ++iter) {
		Result res = FAILURE;
		int counter = 0;
		Node * vm = *iter;
		while (res == FAILURE && counter < 10000) {
			int i = rand() % nodes.size();
		    res = tryToAssignVM(vm, vectorNodes[i]);
		    ++counter;
		}
		//cout << "		counter: " << counter << endl;
		if (res == FAILURE) {
			return FAILURE;
		}
	}
	return SUCCESS;
}

Algorithm::Result Annealing::generateStorageAssignment(Request *request)
{
	//cout << "		generate storage assignment" << endl;
	Stores & storages = request->getStorages();
	Stores & stores = Algorithm::network->getStores();
	vector<Store *> vectorStores(stores.begin(), stores.end());
	//if (storages.size() < 1) {
		//cout << "storages.size() = 0" << endl;
	//}
	for (Stores::iterator iter = storages.begin(); iter != storages.end(); ++iter) {
		cout << "cycle..." << endl;
		Result res = FAILURE;
		int counter = 0;
		Store * st = *iter;
		while (res == FAILURE && counter < 10000) {
			cout << "..." << endl;
			int i = rand() % stores.size();
		    res = tryToAssignStorage(st, vectorStores[i]);
		    ++counter;
		}
		if (res == FAILURE) {
			//cout << "		fail" << endl;
			return FAILURE;
		}
		//cout << "		counter: " << counter << endl;
	}
	return SUCCESS;
}

Algorithm::Result Annealing::generateAssignment(Request *request)
{
	currentAssignment = new Assignment(request);
	if (generateVMAssignment(request) == FAILURE) {
		//cout << "	failed to genVM" << endl;
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
		//cout << "===================="<< endl;
        Request *request = *i;
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
		//cout << "	===================="<< endl;
        Request *request = *i;
        if (generateAssignment(request) == SUCCESS) {
			prevAssignments.insert(currentAssignment);
		}
	}
	return SUCCESS;
 } 

Algorithm::Result Annealing::schedule()
{
    Result result = generatePrevAssignments();
    assignments = prevAssignments;
	cout << "Requests: " << requests.size() << endl;
	double delta = 0, temperature, start_temperature = 50;
	double p = 1; 
	int step = 0;
	do {
		std::cout << "step " << step << std::endl;
		temperature = start_temperature / log(1 + step);
		for (int i = 0; i < 10; ++i) {
			result = generateCurAssignments();
			delta = prevAssignments.size() - curAssignments.size(); 
			double h = (double)rand() / RAND_MAX;
			p = exp(-delta / temperature);
			if (delta <= 0 || h > p) { // cтало лучше
			    prevAssignments = curAssignments;
			}
			if (prevAssignments.size() > assignments.size()) {
				assignments = prevAssignments;
			}
		}
		++step;
	} while (temperature > 7 && assignments.size() != requests.size() && step < 5);
	
    printf("Assigned total of %d from %d requests", assignments.size(), requests.size());
    if ( assignments.size() == requests.size() )
        return SUCCESS;
    else if ( assignments.size() != 0 )
        return PARTIAL;
    else
        return FAILURE;
}

