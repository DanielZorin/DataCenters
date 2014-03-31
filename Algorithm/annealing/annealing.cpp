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
    //cout << node->getID()<< endl;
    //cout << "vm " << vm->getID() << endl;
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

Algorithm::Result Annealing::generateVMAssignmentCurNetwork(Request *request)
{
	Nodes &vms = request->getVirtualMachines();
	Nodes &nodes = curNetwork.getNodes();
	vector<Node *> vectorNodes(nodes.begin(), nodes.end());
	if (vms.size() < 1) {
		return SUCCESS;
	}
	//cout << "nodes size: " << nodes.size() << endl;
	//cout << "1" << endl;
	//cout << "2" << endl;
	for (Nodes::iterator vmi = vms.begin(); vmi != vms.end(); vmi++) {
		int i = (rand() % (nodes.size() + 1)) - 1;
		if (i == -1 || i >= nodes.size()) {
			//cout << "one" << endl;
			return FAILURE;
		}
		//cout << "3" << endl;
        Node * vm = *vmi;
        Node *node = vectorNodes[i];
        int counter = 0;
        while (!node->isAssignmentPossible(*vm) )
        {
			//cout << "4" << endl;
			i = (rand() % (nodes.size() + 1)) - 1;
			if (i == -1 || i >= nodes.size()) {
				//cout << "one" << endl;
				return FAILURE;
			}
		    node = vectorNodes[i];
            ++counter;
            if (counter > 100) {
				return FAILURE;
			}    
        }
        //cout << "5" << endl;
        node->assign(*vm);
        //cout << "6" << endl;
        currentAssignment->AddAssignment(vm, node);
    }
	return SUCCESS;
}

Algorithm::Result 
Annealing::generateStorageAssignmentCurNetwork(Request *request)
{
	Stores & storages = request->getStorages();
	Stores & stores = curNetwork.getStores();
	vector<Store *> vectorStores(stores.begin(), stores.end());
	if (storages.size() < 1) {
		return SUCCESS;
	}
	//cout << "stores size: " << stores.size() << endl;

    for (Stores::iterator s = storages.begin(); s != storages.end(); s++)
	{
		int i = (rand() % (stores.size() + 1)) - 1;
		if (i == -1) {
			return FAILURE;
		}
        Store * storage = *s;
        Store *store = vectorStores[i];
        int counter = 0;
        while ( !store->isAssignmentPossible(*storage) )
        {
			i = (rand() % (stores.size() + 1)) - 1;
			if (i == -1 || i >= stores.size()) {
				return FAILURE;
			}
		    store = vectorStores[i];
            ++counter;
            if (counter > 100) {
				return FAILURE;
			}    
        }
        store->assign(*storage);
        currentAssignment->AddAssignment(storage, store);
    }
	return SUCCESS;
}

Algorithm::Result 
Annealing::generateAssignmentCurNetwork(Request *request)
{
	currentAssignment = new Assignment(request);
	if (generateVMAssignmentCurNetwork(request) == FAILURE) {
		currentAssignment = new Assignment;
		return FAILURE;
	}
	if (generateStorageAssignmentCurNetwork(request) == FAILURE) {
		
		currentAssignment = new Assignment;
		return FAILURE;
	}	
	
	return SUCCESS;	
}

Algorithm::Result Annealing::generateCurAssignments()
{
	curNetwork = (*network);
	//curNetwork = network; 
	curAssignments.clear();
	curAssignments = Assignments();
    for (Requests::iterator i = requests.begin(); i != requests.end(); i++)
    {
        Request *request = *i;
        if (generateAssignmentCurNetwork(request) == SUCCESS) {
			curAssignments.insert(currentAssignment);
		}
	}
	return SUCCESS;
}     

Algorithm::Result Annealing::generateVMAssignmentPrevNetwork(Request *request)
{
	Nodes &vms = request->getVirtualMachines();
	Nodes &nodes = prevNetwork.getNodes();
	vector<Node *> vectorNodes(nodes.begin(), nodes.end());
	if (vms.size() < 1) {
		return SUCCESS;
	}
	//cout << "nodes size: " << nodes.size() << endl;
	//cout << "1" << endl;
	//cout << "2" << endl;
	for (Nodes::iterator vmi = vms.begin(); vmi != vms.end(); vmi++) {
		int i = (rand() % (nodes.size() + 1)) - 1;
		if (i == -1 || i >= nodes.size()) {
			//cout << "one" << endl;
			return FAILURE;
		}
		//cout << "3" << endl;
        Node * vm = *vmi;
        Node *node = vectorNodes[i];
        int counter = 0;
        while (!node->isAssignmentPossible(*vm) )
        {
			//cout << "4" << endl;
			i = (rand() % (nodes.size() + 1)) - 1;
			if (i == -1 || i >= nodes.size()) {
				//cout << "one" << endl;
				return FAILURE;
			}
		    node = vectorNodes[i];
            ++counter;
            if (counter > 100) {
				return FAILURE;
			}    
        }
        //cout << "5" << endl;
        node->assign(*vm);
        //cout << "6" << endl;
        currentAssignment->AddAssignment(vm, node);
    }
	return SUCCESS;
}

Algorithm::Result 
Annealing::generateStorageAssignmentPrevNetwork(Request *request)
{
	Stores & storages = request->getStorages();
	Stores & stores = prevNetwork.getStores();
	vector<Store *> vectorStores(stores.begin(), stores.end());
	if (storages.size() < 1) {
		return SUCCESS;
	}
	//cout << "stores size: " << stores.size() << endl;

    for (Stores::iterator s = storages.begin(); s != storages.end(); s++)
	{
		int i = (rand() % (stores.size() + 1)) - 1;
		if (i == -1) {
			return FAILURE;
		}
        Store * storage = *s;
        Store *store = vectorStores[i];
        int counter = 0;
        while ( !store->isAssignmentPossible(*storage) )
        {
			i = (rand() % (stores.size() + 1)) - 1;
			if (i == -1 || i >= stores.size()) {
				return FAILURE;
			}
		    store = vectorStores[i];
            ++counter;
            if (counter > 100) {
				return FAILURE;
			}    
        }
        store->assign(*storage);
        currentAssignment->AddAssignment(storage, store);
    }
	return SUCCESS;
}

Algorithm::Result 
Annealing::generateAssignmentPrevNetwork(Request *request)
{
	currentAssignment = new Assignment(request);
	if (generateVMAssignmentPrevNetwork(request) == FAILURE) {
		currentAssignment = new Assignment;
		return FAILURE;
	}
	if (generateStorageAssignmentPrevNetwork(request) == FAILURE) {
		
		currentAssignment = new Assignment;
		return FAILURE;
	}	
	
	return SUCCESS;	
}

Algorithm::Result Annealing::generatePrevAssignments()
{
	//prevNetwork = network;
	prevNetwork = (*network);
	prevAssignments.clear(); 
	prevAssignments = Assignments();
    for (Requests::iterator i = requests.begin(); i != requests.end(); i++)
    {
        Request *request = *i;
        if (generateAssignmentPrevNetwork(request) == SUCCESS) {
			cout << "SUCCESS" << endl;
			prevAssignments.insert(currentAssignment);
		}
		//currentAssignment->printAssignment();
    
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
	network->printNetwork();
	cout << "prevAssignments size: generated: " << prevAssignments.size() << endl;
	curAssignments = prevAssignments;
	do {
		temperature = start_temperature / log(1 + step);
		cout << "=================================" << endl;
		cout << "step " << step <<  ";         temperature = " << temperature << ";" << endl;
		for (int i = 0; i < 10; ++i) {
			//cout << "=================================" << endl;
			//result = changeCurAssignments();
			result = generateCurAssignments();
			cout << "cur:  " << curAssignments.size() << ", " << prevAssignments.size() << endl;
			delta = prevAssignments.size() - curAssignments.size(); 
			double h = (double)rand() / RAND_MAX;
			
			p = exp(-delta / temperature);
			cout << "probability = " << p << endl;
			if (delta <= 0 || h > p) {
			    prevAssignments = curAssignments;
			}
			if (prevAssignments.size() > assignments.size()) {
				assignments = prevAssignments;
			}
			//cout << "assignments:  " << assignments.size() << " ";
		}
		++step;
	} while (temperature > 10 && assignments.size() != requests.size() && step < 100);
	if (temperature <= 20) {
		cout << "Temperature is low to continue" << endl;
	}
	//network = prevNetwork;
	network = new Network(prevNetwork);
	printf("Assigned total of %d from %d requests\n", assignments.size(), requests.size());
	cout << endl << endl << endl;
	//for (Assignments::iterator i = assignments.begin(); i != assignments.end(); ++i) {
		////(*i)->printAssignment();
		//Request *request = (*i)->getRequest();
		//request->printRequest();
	//}
	
	//network->printNetwork();
	
    cout << "???" << endl;
    if ( assignments.size() == requests.size() )
        return SUCCESS;
    else if ( assignments.size() != 0 )
        return PARTIAL;
    else
        return FAILURE;
}

