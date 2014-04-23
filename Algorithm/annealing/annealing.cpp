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
#include <map>

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
	for (Nodes::iterator vmi = vms.begin(); vmi != vms.end(); vmi++) {
		int i = (rand() % (nodes.size() + 1)) - 1;
		if (i == -1 || i >= nodes.size()) {
			return FAILURE;
		}
        Node * vm = *vmi;
        Node *node = vectorNodes[i];
        int counter = 0;
        while (!node->isAssignmentPossible(*vm) )
        {
			i = (rand() % (nodes.size() + 1)) - 1;
			if (i == -1 || i >= nodes.size()) {
				return FAILURE;
			}
		    node = vectorNodes[i];
            ++counter;
            if (counter > 100) {
				return FAILURE;
			}    
        }
        node->assign(*vm);
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
		currentAssignment ->forcedCleanup();
		return FAILURE;
	}
	if (generateStorageAssignmentCurNetwork(request) == FAILURE) {
		currentAssignment->forcedCleanup();
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
		currentAssignment->forcedCleanup();
		return FAILURE;
	}
	if (generateStorageAssignmentPrevNetwork(request) == FAILURE) {
		
		currentAssignment->forcedCleanup();
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
        //cout << "generating assigning of request " << request->getName() << endl;
        if (generateAssignmentPrevNetwork(request) == SUCCESS) {
			//cout << "Request  is assigned" << endl;
			prevAssignments.insert(currentAssignment);
		}
	}
	return SUCCESS;
 } 
 	
void Annealing::printAssignments(Assignments a)
{
	getchar();
	multimap<int, int> resultNodes, resultStores ;
	for (Assignments::iterator i = a.begin(); i != a.end(); ++i) {
		multimap<int, int> aNodes = (*i)->printAssignmentNodes();
		multimap<int, int> aStores = (*i)->printAssignmentStores();
		//Request *request = (*i)->getRequest();
		//request->printRequest();
		for (multimap<int, int>::iterator iter = aNodes.begin(); iter != aNodes.end(); ++iter) {
			resultNodes.insert(*iter);
		}
		for (multimap<int, int>::iterator iter = aStores.begin(); iter != aStores.end(); ++iter) {
			resultStores.insert(*iter);
		}
	}
	cout << "printing ordered assignments of Nodes" << endl;
	for (multimap<int, int>::iterator i = resultNodes.begin(); i != resultNodes.end(); ++i) {
		cout << i->first << " " << i->second << endl;
	}  
	cout << "printing ordered assignments of Stores" << endl;
	for (multimap<int, int>::iterator i = resultStores.begin(); i != resultStores.end(); ++i) {
		cout << i->first << " " << i->second << endl;
	}  
}

void Annealing::printAssignment(Assignment *a)
{
	getchar();
	multimap<int, int> aNodes = (a)->printAssignmentNodes();
	multimap<int, int> aStores = (a)->printAssignmentStores();
	cout << "printing ordered assignments of Nodes" << endl;
	for (multimap<int, int>::iterator i = aNodes.begin(); i != aNodes.end(); ++i) {
		cout << i->first << " " << i->second << endl;
	}  
	cout << "printing ordered assignments of Stores" << endl;
	for (multimap<int, int>::iterator i = aStores.begin(); i != aStores.end(); ++i) {
		cout << i->first << " " << i->second << endl;
	}  
}

void Annealing::printRequests()
{
	getchar();
	int request_counter = 0;
	cout << endl << "<><><><><><><><>Print all requests...<><><><><><><><><><><><><><><><><><><>" << endl;
	for (Requests::iterator i = requests.begin(); i != requests.end(); i++)
    {
		++request_counter;
        Request *request = *i;
        request->printRequest();
	} 
	cout << "<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>" << endl;
	printf("Total of %d requests\n\n", request_counter); 
}

Algorithm::Result Annealing::changeCurAssignments()
{
	copyPrevAssignmentsToCur();
	
	//cout << "Printing curNetwork before changing..." << endl;
	//curNetwork.printNetwork();
	
	//cout << "printing cur_assignments before changing" << endl;
	//printAssignments(curAssignments);
    changeAssignments();
    Result res = tryToInsertNewAssignment();
    if (res == SUCCESS) {
		return SUCCESS;
	}
    int n_counts = 0;
    if (curAssignments.size() >= requests.size() / 3) {
		cout << "here: cur = "<< curAssignments.size() << ", req = " << requests.size() << endl;
		n_counts = 20;
	} else {
		n_counts = 2;
	}
    for (int counter = 0; counter != n_counts; ++counter) {
		changeAssignments();
		//deleteRandRequest();
		res = tryToInsertNewAssignment();
		if (res == SUCCESS) {
			return SUCCESS;
		}
		
	}	
	if (res == FAILURE) {
		generateCurAssignments();
		//deleteRandRequest();

		return SUCCESS;
	}
	return SUCCESS;
}   


void Annealing::mutation()
{
	//cout << endl << "state = " << state << ", counterFail = " << counterFail << endl;
	if (counterFail >= 10) {
		state = 3;	
	}
	//cout << "mutation..." << endl;
	if (state == 1) {
		
		if (tryToInsertNewAssignment() == FAILURE) {
			++counterFail;
			state = 2;
			return;
		} else {
			counterFail = 1;
			state = 1;
			return;
		}
		return;
		
	}
	if (state == 2) {
		Result res = reassignRandRequest();
		if (res == FAILURE && counterFail >= 10) {
			//cout << "res == FAILURE && counterFail == 10" << endl;
			state = 3;
			counterFail = 1;
			return;
		} else if (res == SUCCESS) {
			//cout << "res == SUCCESS" << endl;
			state = 1;
			return;
		} else if (res == FAILURE && counterFail < 10) {
			//cout << "res == FAILURE && counterFail < 10" << endl;
			++counterFail;
			state = 1;
			return;
		}
		return;
	}
	if (state == 3) {
		counterFail = 1;
		state = 1;
		deleteAssignmentFromCur();
		return;
	}
	return;
}	

Algorithm::Result
Annealing::reassignRandRequest()
{
	cout << "move.............." << endl;
	if (curAssignments.begin() == curAssignments.end()) {
		return FAILURE;
	}
	vector<Assignment *> a(curAssignments.begin(), curAssignments.end());
	int i = rand() % curAssignments.size();
	
	multimap<int, int> aNodes = a[i]->printAssignmentNodes();
	multimap<int, int> aStores = a[i]->printAssignmentStores();
	Assignments::iterator it;
    for (it = curAssignments.begin(); it != curAssignments.end(); ++it) {
		multimap<int, int> tmpNodes = (*it)->printAssignmentNodes();
		multimap<int, int> tmpStores = (*it)->printAssignmentStores();
		if (tmpNodes == aNodes && tmpStores == aStores) {
			//assignmentToDelete = *it;
			break;
		}
	}
	if (it == curAssignments.end()) {
		return FAILURE;
	}
    
    curAssignments.erase(*it);	
	
	Request *request = a[i]->getRequest();
	//cout << "It is request " << request->getName() << endl;
	Stores &storages = request->getStorages();
	Nodes &vm = request->getVirtualMachines();
	int flag = 0;
	for (Stores::iterator iter = storages.begin(); iter != storages.end(); ++iter) {
		//cout << "Removing store assignments" << endl; 
		flag = 1;
        //is curNetwork be changed?
		Store *store = a[i]->GetAssignment(*iter);
		(store)->RemoveAssignment(*iter);
		a[i]->RemoveAssignment(store);
	}
	for (Nodes::iterator iter = vm.begin(); iter != vm.end(); ++iter) {
		flag = 1;
		//cout << "Removing node assignments" << endl; 
		Node *node = a[i]->GetAssignment(*iter);
		node->RemoveAssignment(*iter);
		a[i]->RemoveAssignment(*iter);
	}
	//currentAssignment->forcedCleanup();
	if (!flag) {
		return FAILURE;
	}
	Result res = FAILURE;
	int counter = 0;
	while (res == FAILURE) {
		res = generateAssignmentCurNetwork(request);
		if (res == SUCCESS) { 
			++kol;
			//cout << "2. not assigned request now assigned" << endl;
			curAssignments.insert(currentAssignment);
			break;
		}
		++counter;
		if (counter > 100) {
			break;
		}
	}
	if (res == SUCCESS) {
		return SUCCESS;
	}
	if (requests.size() == curAssignments.size()) {
		return SUCCESS;
	}
	return SUCCESS;
	
}

Algorithm::Result 
Annealing::changeAssignments()
{
	//cout << "Try to change assignments..." << endl;
	currentAssignment->forcedCleanup();
	currentAssignment = new Assignment();
	int i = 0, k = 0;
	//cout << "Printing curAssignments before removing element" << endl;
	//printAssignments(curAssignments);
	if (curAssignments.size() > 0) {
		i = rand() % (curAssignments.size());
		for (Assignments::iterator iter = curAssignments.begin(); iter != curAssignments.end(); ++iter) {
			if (k < i) {
				++k;
				continue;
			}
			if (k == i) {
				//cout << "1.Needed request found" << endl;
				currentAssignment = *iter;
				curAssignments.erase(*iter);
				break;
			}
		}
	} else {
		//cout << "curAssignments size = 0" << endl;
		return SUCCESS;
	}
	
	//every assignment has an appropriate request. That's why we will randomly choose one of the assignments (one of the requests) and try to change it (to assign this request to other resource)
	//chosen assignment is to be cleared and the request is to be reassigned
	//extract the needed request
	//clear network
		
	Request *request = currentAssignment->getRequest();
	//cout << "It is request " << request->getName() << endl;
	Stores &storages = request->getStorages();
	Nodes &vm = request->getVirtualMachines();
	int flag = 0;
	for (Stores::iterator i = storages.begin(); i != storages.end(); ++i) {
		//cout << "Removing store assignments" << endl; 
		flag = 1;
        //is curNetwork be changed?
		Store *store = currentAssignment->GetAssignment(*i);
		(store)->RemoveAssignment(*i);
		currentAssignment->RemoveAssignment(store);
	}
	for (Nodes::iterator i = vm.begin(); i != vm.end(); ++i) {
		flag = 1;
		//cout << "Removing node assignments" << endl; 
		Node *node = currentAssignment->GetAssignment(*i);
		node->RemoveAssignment(*i);
		currentAssignment->RemoveAssignment(*i);
	    
	}
	if (flag) {
		//cout << "2.removed assignment" << endl;
	}
	//cout << endl<< endl << "Printing curNetwork after removing one request" << endl;
	//curNetwork.printNetwork();
	//cout << "printing cur_assignments after removing one request" << endl;
	//printAssignments(curAssignments);
	Result res = FAILURE;
	int counter = 0;
	currentAssignment = new Assignment(request);
    while (res == FAILURE) {
		res = generateAssignmentCurNetwork(request);
		if (res == SUCCESS) { 
			//cout << "3.request reassigned" << endl;
			curAssignments.insert(currentAssignment);
			break;
		}
		++counter;
		if (counter > 100) {
			break;
		}
	}	
	
	//cout << endl<< endl << "Printing curNetwork after removing and reassigning one request" << endl;
	//curNetwork.printNetwork();
	//cout << "printing cur_assignments after removing and reassigning one request" << endl;
	//printAssignments(curAssignments);
	return res;
}

Algorithm::Result 
Annealing::tryToInsertNewAssignment() 
{
	//one of assignments chosen randomly was moved
	//now look for not assigned requests and try to assign it
	//need to provide random
	//cout << endl << "Try to assign some else request..." << endl;
	cout << "insert................" << endl;
	int flag = 0;
	Requests::iterator i;
	for (i = requests.begin(); i != requests.end(); i++) {
		flag = 0;
		for (Assignments::iterator iter = curAssignments.begin(); iter != curAssignments.end(); ++iter) {
			if ((*iter)->getRequest() == *i) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			//cout << "1. not assigned request found" << endl;
			//cout << "It is request " << (*i)->getName() << endl;
			Result res = FAILURE;
			int counter = 0;
			while (res == FAILURE) {
				//cout << "res == FAILURE, in cycle, counter = " << counter << endl;
				//printAssignments(curAssignments);
				//curNetwork.printNetwork();
				//currentAssignment->forcedCleanup();
				res = generateAssignmentCurNetwork(*i);
				if (res == SUCCESS) { 
					++kol;
					//cout << "2. not assigned request now assigned" << endl;
					curAssignments.insert(currentAssignment);
					break;
				} else {
					//cout << "2. not assigned requests can't be assigned" << endl;
				}
				//printAssignments(curAssignments);
				//curNetwork.printNetwork();
				++counter;
				if (counter > 100) {
					break;
				}
			}
			if (res == SUCCESS) {
				return SUCCESS;
			}
		}
	}
	if (requests.size() == curAssignments.size()) {
		return SUCCESS;
	}
	if (i == requests.end()) {
		cout << "All possible requests are already assigned, they must be mixed to assign new one" << endl;
		return FAILURE;
	}
	return SUCCESS;
	//request is not assigned
	
	
	
	//cout << endl<< endl << "Printing curNetwork after assigning free request" << endl;
	//curNetwork.printNetwork();
	//cout << "printing cur_assignments after assigning free request" << endl;
	//printAssignments(curAssignments);
}	

void 
Annealing::copyPrevAssignmentsToBest()
{
	bestNetwork = prevNetwork;
	assignments.clear();
	for (Assignments::iterator iter = prevAssignments.begin(); iter != prevAssignments.end(); ++iter) {
		Assignment *a = *iter;
		Request *req = a->getRequest();
		Assignment *tmp = new Assignment(req);
		for (Assignment::NodeAssignments::iterator nodeAssIter = a->nodeAssignments.begin(); nodeAssIter != a->nodeAssignments.end(); ++nodeAssIter) {
			NodeAssignment pairOfNodes = *nodeAssIter;
			Node *nodeFromRequest = pairOfNodes.first;
			Node *nodeFromPrevNetwork = pairOfNodes.second;
			Node *nodeFromBestNetwork = bestNetwork.nodesIDLookup(nodeFromPrevNetwork->getID());
			tmp->AddAssignment(nodeFromRequest, nodeFromBestNetwork);
		}
		for (Assignment::StoreAssignments::iterator storeAssIter = a->storeAssignments.begin(); storeAssIter != a->storeAssignments.end(); ++storeAssIter) {
			StoreAssignment pairOfStores = *storeAssIter;
			Store *storeFromRequest = pairOfStores.first;
			Store *storeFromPrevNetwork = pairOfStores.second;
			tmp->AddAssignment(storeFromRequest, bestNetwork.storesIDLookup(storeFromPrevNetwork->getID()));
		}
        assignments.insert(tmp);
	}	
}

void 
Annealing::copyCurAssignmentsToBest()
{
	bestNetwork = curNetwork;
	assignments.clear();
	for (Assignments::iterator iter = curAssignments.begin(); iter != curAssignments.end(); ++iter) {
		Assignment *a = *iter;
		Request *req = a->getRequest();
		Assignment *tmp = new Assignment(req);
		for (Assignment::NodeAssignments::iterator nodeAssIter = a->nodeAssignments.begin(); nodeAssIter != a->nodeAssignments.end(); ++nodeAssIter) {
			NodeAssignment pairOfNodes = *nodeAssIter;
			Node *nodeFromRequest = pairOfNodes.first;
			Node *nodeFromCurNetwork = pairOfNodes.second;
			Node *nodeFromBestNetwork = bestNetwork.nodesIDLookup(nodeFromCurNetwork->getID());
			tmp->AddAssignment(nodeFromRequest, nodeFromBestNetwork);
		}
		for (Assignment::StoreAssignments::iterator storeAssIter = a->storeAssignments.begin(); storeAssIter != a->storeAssignments.end(); ++storeAssIter) {
			StoreAssignment pairOfStores = *storeAssIter;
			Store *storeFromRequest = pairOfStores.first;
			Store *storeFromCurNetwork = pairOfStores.second;
			tmp->AddAssignment(storeFromRequest, bestNetwork.storesIDLookup(storeFromCurNetwork->getID()));
		}
        assignments.insert(tmp);
	}	
}

void 
Annealing::copyCurAssignmentsToPrev()
{
	prevNetwork = curNetwork;
	prevAssignments.clear();
	for (Assignments::iterator iter = curAssignments.begin(); iter != curAssignments.end(); ++iter) {
		Assignment *a = *iter;
		Request *req = a->getRequest();
		Assignment *tmp = new Assignment(req);
		for (Assignment::NodeAssignments::iterator nodeAssIter = a->nodeAssignments.begin(); nodeAssIter != a->nodeAssignments.end(); ++nodeAssIter) {
			NodeAssignment pairOfNodes = *nodeAssIter;
			Node *nodeFromRequest = pairOfNodes.first;
			Node *nodeFromCurNetwork = pairOfNodes.second;
			Node *nodeFromPrevNetwork = prevNetwork.nodesIDLookup(nodeFromCurNetwork->getID());
			tmp->AddAssignment(nodeFromRequest, nodeFromPrevNetwork);
		}
		for (Assignment::StoreAssignments::iterator storeAssIter = a->storeAssignments.begin(); storeAssIter != a->storeAssignments.end(); ++storeAssIter) {
			StoreAssignment pairOfStores = *storeAssIter;
			Store *storeFromRequest = pairOfStores.first;
			Store *storeFromCurNetwork = pairOfStores.second;
			tmp->AddAssignment(storeFromRequest, prevNetwork.storesIDLookup(storeFromCurNetwork->getID()));
		}
        prevAssignments.insert(tmp);
	}	
}
	
void 
Annealing::copyPrevAssignmentsToCur()
{
	curNetwork = prevNetwork;
	//prevAssignments are not empty. They are connected to PrevNetwork
	//this function is to full curAssignments connected to CurNetwork. 
	//Requests must be assigned to those resources as in prevAssignments
	curAssignments.clear();
	for (Assignments::iterator iter = prevAssignments.begin(); iter != prevAssignments.end(); ++iter) {
		Assignment *a = *iter;
		Request *req = a->getRequest();
		Assignment *tmp = new Assignment(req);
		for (Assignment::NodeAssignments::iterator nodeAssIter = a->nodeAssignments.begin(); nodeAssIter != a->nodeAssignments.end(); ++nodeAssIter) {
			NodeAssignment pairOfNodes = *nodeAssIter;
			Node *nodeFromRequest = pairOfNodes.first;
			Node *nodeFromPrevNetwork = pairOfNodes.second;
			Node *nodeFromCurNetwork = curNetwork.nodesIDLookup(nodeFromPrevNetwork->getID());
			tmp->AddAssignment(nodeFromRequest, nodeFromCurNetwork);
		}
		for (Assignment::StoreAssignments::iterator storeAssIter = a->storeAssignments.begin(); storeAssIter != a->storeAssignments.end(); ++storeAssIter) {
			StoreAssignment pairOfStores = *storeAssIter;
			Store *storeFromRequest = pairOfStores.first;
			Store *storeFromPrevNetwork = pairOfStores.second;
			tmp->AddAssignment(storeFromRequest, curNetwork.storesIDLookup(storeFromPrevNetwork->getID()));
		}
        curAssignments.insert(tmp);
	}	
}

Algorithm::Result
Annealing::deleteAssignmentFromCur()
{
	cout << "delete......................" << endl;
	if (curAssignments.begin() == curAssignments.end()) {
		return FAILURE;
	}
	vector<Assignment *> a(curAssignments.begin(), curAssignments.end());
	
	int i = rand() % curAssignments.size();
	
	multimap<int, int> aNodes = a[i]->printAssignmentNodes();
	multimap<int, int> aStores = a[i]->printAssignmentStores();
    
	int aNodesSize = aNodes.size(), aStoresSize = aStores.size();
	Assignments::iterator it;
    for (it = curAssignments.begin(); it != curAssignments.end(); ++it) {
		multimap<int, int> tmpNodes = (*it)->printAssignmentNodes();
		multimap<int, int> tmpStores = (*it)->printAssignmentStores();
		if (tmpNodes.size() != aNodesSize || tmpStores.size() != aStoresSize) {
			continue;
		}
		if (tmpNodes == aNodes && tmpStores == aStores) {
			//printAssignment(*it);
			//cout << "assignment to delete found" << endl;
			break;
		}
	}
	if (it == curAssignments.end()) {
		return FAILURE;
	}
    curAssignments.erase(*it);	
    //cout << "tmp assignment to erase from curAssignments" << endl;
    //printAssignment(*it);
	
	Request *request = a[i]->getRequest();
	Stores &storages = request->getStorages();
	Nodes &vm = request->getVirtualMachines();
	int flag = 0;
	for (Stores::iterator iter = storages.begin(); iter != storages.end(); ++iter) {
		//cout << "Removing store assignments" << endl; 
		flag = 1;
        //is curNetwork be changed?
		Store *store = a[i]->GetAssignment(*iter);
		(store)->RemoveAssignment(*iter);
		a[i]->RemoveAssignment(store);
	}
	for (Nodes::iterator iter = vm.begin(); iter != vm.end(); ++iter) {
		flag = 1;
		//cout << "Removing node assignments" << endl; 
		Node *node = a[i]->GetAssignment(*iter);
		node->RemoveAssignment(*iter);
		a[i]->RemoveAssignment(node);
	    
	}	
	if (flag) {
		return SUCCESS;
	} else {
		return FAILURE;
	}
	
}
	
Assignment *
Annealing::copyAssignmentToCurrent(Assignment *a)
{
	cout << endl << "copy function, step 1" << endl;
    curNetwork.printNetwork();
    cout <<  endl <<"copy function, step 1" << endl;
    printAssignments(curAssignments);
    
	currentAssignment->forcedCleanup();
	currentAssignment = new Assignment(a->getRequest());
	for (Assignment::NodeAssignments::iterator nodeAssIter = a->nodeAssignments.begin(); nodeAssIter != a->nodeAssignments.end(); ++nodeAssIter) {
		NodeAssignment pairOfNodes = *nodeAssIter;
		Node *nodeFromRequest = pairOfNodes.first;
		Node *nodeFromAssignment = pairOfNodes.second;
		Node *nodeFromCurNetwork = curNetwork.nodesIDLookup(nodeFromAssignment->getID());
		currentAssignment->AddAssignment(nodeFromRequest, nodeFromCurNetwork);
	}
	for (Assignment::StoreAssignments::iterator storeAssIter = a->storeAssignments.begin(); storeAssIter != a->storeAssignments.end(); ++storeAssIter) {
		StoreAssignment pairOfStores = *storeAssIter;
		Store *storeFromRequest = pairOfStores.first;
		Store *storeFromAssignment = pairOfStores.second;
		Store *storeFromCurNetwork = curNetwork.storesIDLookup(storeFromAssignment->getID());
		currentAssignment->AddAssignment(storeFromRequest, storeFromCurNetwork);
	}
		cout << endl << "copy function, step 2" << endl;
    curNetwork.printNetwork();
    cout <<  endl <<"copy function, step 2" << endl;
    printAssignments(curAssignments);
	return currentAssignment;
}


void 
Annealing::printUnassignedRequests()
{
	cout << endl << "unassigned requests..." << endl;
	getchar();
	int flag = 0;
	Request *request;
	Requests::iterator i;
	for (i = requests.begin(); i != requests.end(); i++) {
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
			cout << "not assigned request: ";
			request = *i;
			cout << request->getName() << endl;
		}
	}
}
	

Algorithm::Result Annealing::schedule()
{
    Result result = generatePrevAssignments();
    kol = 0;
    curNetwork = (*network);
    state = 1;
    counterFail = 1;
    copyPrevAssignmentsToCur();
   //for (int i = 0; i < 10; ++i) {
	    //mutation();
		//cout << endl << "printing curNetwork" << endl;
		//curNetwork.printNetwork();
		//cout <<  endl <<"printing curAssignments" << endl;
		//printAssignments(curAssignments);
		//printUnassignedRequests();
	//}
    
	double temperature, start_temperature = 50;
	int delta = 0;
	double p = 1; 
	int step = 2;
	state = 1;
	counterFail = 1;
	//network->printNetwork();
	//printRequests();
	//cout << "prevAssignments size: generated: " << prevAssignments.size() << endl;
	//curNetwork = prevNetwork;
	copyPrevAssignmentsToCur();
	//cout << "prevNetwork:" << endl;
	//prevNetwork.printNetwork();
	
	//cout << "curNetwork before all this" << endl;
	//curNetwork.printNetwork();
	//cout << "curAssignments before all this" << endl;
	//printAssignments(curAssignments);	
	do {
		temperature = start_temperature / log(1 + step);
		cout << "step " << step <<  ";         temperature = " << temperature << ";" << endl;
		cout << "cur:  " << curAssignments.size() << ", prev: " << prevAssignments.size() << endl;
		for (int i = 0; i < 10; ++i) {
			mutation();
			//curNetwork.printNetwork();
			//printAssignments(curAssignments);			
			
			if (curAssignments.size() == requests.size()) {
				copyCurAssignmentsToBest();
				break;
			}
			//cout << "cur:  " << curAssignments.size() << ", prev: " << prevAssignments.size() << endl;
			delta = prevAssignments.size() - curAssignments.size(); 
			double h = (double)rand() / RAND_MAX;
			
			p = exp(-delta / temperature);
			//cout << "probability = " << p << "; h = " << h << endl;
			if (delta <= 0) {
				//cout << "probability doesn't matter" << endl;
				copyCurAssignmentsToPrev();
			} else if (h > p) {
				copyCurAssignmentsToPrev();
			}
			if (prevAssignments.size() > assignments.size()) {
				copyPrevAssignmentsToBest();
			}
			
		}
		++step;
	} while (temperature > 12 && assignments.size() != requests.size());
	if (temperature <= 12) {
		cout << "Temperature is low to continue" << endl;
	}
	printf("Assigned total of %d from %d requests\n", assignments.size(), requests.size());
	cout << endl << endl << endl;
	//printAssignments(assignments);
	//cout << kol << endl;
	cout << endl << "printing bestNetwork..." << endl;
	bestNetwork.printNetwork();
	
	cout << endl << "printing bestAssignments" << endl;
	printAssignments(assignments);
	
	    
    if ( assignments.size() == requests.size() )
        return SUCCESS;
    else if ( assignments.size() != 0 )
        return PARTIAL;
    else
        return FAILURE;
}

