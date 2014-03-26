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

Algorithm::Result Annealing::generateVMAssignment(Request *request, Network *cnetwork)
{
	Nodes &vm = request->getVirtualMachines();
	Nodes &nodes = cnetwork->getNodes();
	vector<Node *> vectorNodes(nodes.begin(), nodes.end());
	if (vm.size() < 1) {
		return SUCCESS;
	}
	for (Nodes::iterator iter = vm.begin(); iter != vm.end(); ++iter) {
		Result res = FAILURE;
		int counter = 0;
		Node * vm = *iter;
		while (res == FAILURE) {
			int i = (rand() % (nodes.size() + 1)) - 1;
            if (i == -1) {
				return FAILURE;
			}
		    res = tryToAssignVM(vm, vectorNodes[i]);
		    ++counter;
		    if (counter > 10) 
				break;
		}
		if (res == FAILURE) {
			return FAILURE;
		}
	}
	return SUCCESS;
}

Algorithm::Result 
Annealing::changeAssignments()
{
	cout << "Try to change assignments..." << endl;
	currentAssignment = new Assignment();
	int i = 0, k = 0;
	if (curAssignments.size() > 0) {
		i = rand() % (curAssignments.size());
		for (Assignments::iterator iter = curAssignments.begin(); iter != curAssignments.end(); ++iter) {
			if (k < i) {
				++k;
				continue;
			}
			if (k == i) {
				cout << "1.Needed request found" << endl;
				currentAssignment = *iter;
				curAssignments.erase(*iter);
				break;
			}
		}
	} else {
		cout << "curAssignments size = 0" << endl;
		return SUCCESS;
	}
	//every assignment has an appropriate request. That's why we will randomly choose one of the assignments (one of the requests) and try to change it (to assign this request to other resource)
	//chosen assignment is to be cleared and the request is to be reassigned
	//extract the needed request
	//clear network
		
	Request *request = currentAssignment->getRequest();
	Stores &storages = request->getStorages();
	Nodes &vm = request->getVirtualMachines();
	int flag = 0;
	for (Stores::iterator i = storages.begin(); i != storages.end(); ++i) {
		//cout << "Removing store assignments" << endl; 
		flag = 1;
//is curNetwork be changed?
		Store *store = currentAssignment->GetAssignment(*i);
		(store)->RemoveAssignment(*i);
	}
	for (Nodes::iterator i = vm.begin(); i != vm.end(); ++i) {
		flag = 1;
		//cout << "Removing node assignments" << endl; 
		Node *node = currentAssignment->GetAssignment(*i);
		(node)->RemoveAssignment(*i);
	}
	if (flag) {
		cout << "2.removed assignment" << endl;
	}

	Result res = FAILURE;
	int counter = 0;
    while (res == FAILURE) {
		res = generateAssignment(request, curNetwork);
		if (res == SUCCESS) { 
			cout << "3.request reassigned" << endl;
			curAssignments.insert(currentAssignment);
			break;
		}
		++counter;
		if (counter > 100) {
			break;
		}
	}
	return res;
}

Algorithm::Result 
Annealing::tryToInsertNewAssignment() {
	//one of assignments chosen randomly was moved
	//now look for not assigned requests and try to assign it
	//need to provide random
	cout << endl << "Try to assign some else request..." << endl;
	int flag = 0;
	Request *request;
	for (Requests::iterator i = requests.begin(); i != requests.end(); i++) {
		flag = 0;
		for (Assignments::iterator iter = curAssignments.begin(); iter != curAssignments.end(); ++iter) {
            currentAssignment = new Assignment;
            currentAssignment = *iter;
			if (currentAssignment->getRequest() == *i) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			cout << "1. not assigned request found" << endl;
			request = *i;
			break;
		}
	}
	//request is not assigned
	
	Result res = FAILURE;
	int counter = 0;
    while (res == FAILURE) {
		res = generateAssignment(request, curNetwork);
		if (res == SUCCESS) { 
			cout << "2. not assigned request now assigned" << endl;
			curAssignments.insert(currentAssignment);
			break;
		}
		++counter;
		if (counter > 100) {
			break;
		}
	}
	return res;
}			

Algorithm::Result 
Annealing::generateStorageAssignment(Request *request, Network *cnetwork)
{
	Stores & storages = request->getStorages();
	Stores & stores = cnetwork->getStores();
	vector<Store *> vectorStores(stores.begin(), stores.end());
	if (storages.size() < 1) {
		return SUCCESS;
	}
	for (Stores::iterator iter = storages.begin(); iter != storages.end(); ++iter) {
		Result res = FAILURE;
		int counter = 0;
		Store * st = *iter;
		while (res == FAILURE) {
			int i = (rand() % (stores.size() + 1)) - 1;
            if (i == -1) {
				return FAILURE;
			}
		    res = tryToAssignStorage(st, vectorStores[i]);
		    ++counter;
		    if (counter > 10) {
				break;
			}
		}
		if (res == FAILURE) {
			return FAILURE;
		}
	}
	return SUCCESS;
}

Algorithm::Result 
Annealing::generateAssignment(Request *request, Network *cnetwork)
{
	currentAssignment = new Assignment(request);
	if (generateVMAssignment(request, cnetwork) == FAILURE) {
		return FAILURE;
	}
	if (generateStorageAssignment(request, cnetwork) == FAILURE) {
		return FAILURE;
	}	
	return SUCCESS;	
}

Algorithm::Result Annealing::generateCurAssignments()
{
	curNetwork = new Network;
	(*curNetwork) = (*network); 
	curAssignments.clear();
    for (Requests::iterator i = requests.begin(); i != requests.end(); i++)
    {
        Request *request = *i;
        if (generateAssignment(request, curNetwork) == SUCCESS) {
			curAssignments.insert(currentAssignment);
		}
	}
	return SUCCESS;
}     

Algorithm::Result Annealing::changeCurAssignments()
{
	curNetwork = new Network;
	(*curNetwork) = (*prevNetwork); 
    changeAssignments();
    tryToInsertNewAssignment();
	return SUCCESS;
}       

Algorithm::Result Annealing::generatePrevAssignments()
{
	prevNetwork = new Network;
	(*prevNetwork) = (*network); 
	prevAssignments.clear();
    for (Requests::iterator i = requests.begin(); i != requests.end(); i++)
    {
        Request *request = *i;
        if (generateAssignment(request, prevNetwork) == SUCCESS) {
			prevAssignments.insert(currentAssignment);
		}
	}
	return SUCCESS;
 } 

Algorithm::Result Annealing::schedule()
{
    Result result = generatePrevAssignments();
    assignments = prevAssignments;
	double delta = 0, temperature, start_temperature = 50;
	double p = 1; 
	int step = 2;
	cout << "prevAssignments size: generated: " << prevAssignments.size() << endl;
	curAssignments = prevAssignments;
	do {
		temperature = start_temperature / log(1 + step);
		cout << "=================================" << endl;
		cout << "step " << step <<  ";         temperature = " << temperature << ";" << endl;
		for (int i = 0; i < 10; ++i) {
			cout << "=================================" << endl;
			result = changeCurAssignments();
			//result = generateCurAssignments();
			cout << "cur:  " << curAssignments.size() << endl;
			delta = prevAssignments.size() - curAssignments.size(); 
			double h = (double)rand() / RAND_MAX;
			p = exp(-delta / temperature);
			if (delta <= 0 || h > p) {
			    prevAssignments = curAssignments;
			    (*prevNetwork) = (*curNetwork); 
			}
			if (prevAssignments.size() > assignments.size()) {
				assignments = prevAssignments;
				(*network) = (*prevNetwork);
			}
			//cout << "assignments:  " << assignments.size() << " ";
		}
		++step;
	} while (temperature > /*7*/ 20 && assignments.size() != requests.size());
	if (temperature <= 7) {
		cout << "Temperature is low to continue" << endl;
	}
	(*network) = (*prevNetwork);
    printf("Assigned total of %d from %d requests\n", assignments.size(), requests.size());
    if ( assignments.size() == requests.size() )
        return SUCCESS;
    else if ( assignments.size() != 0 )
        return PARTIAL;
    else
        return FAILURE;
}

