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
	currentAssignment->forcedCleanup();
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
	Stores &storages = request->getStorages();
	Nodes &vm = request->getVirtualMachines();
	int flag = 0;
	for (Stores::iterator i = storages.begin(); i != storages.end(); ++i) {
		flag = 1;
        //is curNetwork be changed?
		Store *store = currentAssignment->GetAssignment(*i);
		(store)->RemoveAssignment(*i);
		currentAssignment->RemoveAssignment(store);
	}
	for (Nodes::iterator i = vm.begin(); i != vm.end(); ++i) {
		flag = 1;
		Node *node = currentAssignment->GetAssignment(*i);
		node->RemoveAssignment(*i);
		currentAssignment->RemoveAssignment(*i);
	    
	}
	if (flag) {
		//cout << "2.removed assignment" << endl;
	}
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
	return res;
}

Algorithm::Result 
Annealing::tryToInsertNewAssignment() 
{
	//looking for not assigned requests and trying to assign it
	//chosen request is assigned randomly
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
				res = generateAssignmentCurNetwork(*i);
				if (res == SUCCESS) { 
					++kol;
					//cout << "2. not assigned request now assigned" << endl;
					curAssignments.insert(currentAssignment);
					break;
				} else {
					//cout << "2. not assigned requests can't be assigned" << endl;
				}
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
		return FAILURE;
	}
	return SUCCESS;
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

void 
Annealing::copyBestAssignmentsToCur()
{
	curNetwork = bestNetwork;
	//prevAssignments are not empty. They are connected to PrevNetwork
	//this function is to full curAssignments connected to CurNetwork. 
	//Requests must be assigned to those resources as in prevAssignments
	curAssignments.clear();
	for (Assignments::iterator iter = assignments.begin(); iter != assignments.end(); ++iter) {
		Assignment *a = *iter;
		Request *req = a->getRequest();
		Assignment *tmp = new Assignment(req);
		for (Assignment::NodeAssignments::iterator nodeAssIter = a->nodeAssignments.begin(); nodeAssIter != a->nodeAssignments.end(); ++nodeAssIter) {
			NodeAssignment pairOfNodes = *nodeAssIter;
			Node *nodeFromRequest = pairOfNodes.first;
			Node *nodeFromBestNetwork = pairOfNodes.second;
			Node *nodeFromCurNetwork = curNetwork.nodesIDLookup(nodeFromBestNetwork->getID());
			tmp->AddAssignment(nodeFromRequest, nodeFromCurNetwork);
		}
		for (Assignment::StoreAssignments::iterator storeAssIter = a->storeAssignments.begin(); storeAssIter != a->storeAssignments.end(); ++storeAssIter) {
			StoreAssignment pairOfStores = *storeAssIter;
			Store *storeFromRequest = pairOfStores.first;
			Store *storeFromBestNetwork = pairOfStores.second;
			tmp->AddAssignment(storeFromRequest, curNetwork.storesIDLookup(storeFromBestNetwork->getID()));
		}
        curAssignments.insert(tmp);
	}	
}

Algorithm::Result
Annealing::deleteAssignmentFromCur()
{
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
			break;
		}
	}
	if (it == curAssignments.end()) {
		return FAILURE;
	}
    curAssignments.erase(*it);	
	
	Request *request = a[i]->getRequest();
	Stores &storages = request->getStorages();
	Nodes &vm = request->getVirtualMachines();
	int flag = 0;
	for (Stores::iterator iter = storages.begin(); iter != storages.end(); ++iter) {
		flag = 1;
        //is curNetwork be changed?
		Store *store = a[i]->GetAssignment(*iter);
		(store)->RemoveAssignment(*iter);
		a[i]->RemoveAssignment(store);
	}
	for (Nodes::iterator iter = vm.begin(); iter != vm.end(); ++iter) {
		flag = 1;
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

Algorithm::Result
Annealing::assignLinks()
{
	Result result = FAILURE;
	currentAssignment = new Assignment();
	copyBestAssignmentsToCur();
	for (Assignments::iterator iter = curAssignments.begin(); iter != curAssignments.end(); ++iter) {
		currentAssignment = *iter;
		Request *request = currentAssignment->getRequest();
		Links & vlinks = request->getVirtualLinks();
        for (Links::iterator l = vlinks.begin(); l != vlinks.end(); l++)
        {
            Link * dummy = createDummyLink(*l, currentAssignment);

            if ( dummy == 0 )
            {
                result = FAILURE;
                break;
            }

            NetPath netPath = VirtualLinkRouter::routeDejkstra(dummy, network);
            delete dummy;

            if ( netPath.size() == 0 )
            {
                result = FAILURE;
                break;
            } 

            for ( NetPath::iterator i = netPath.begin(); i != netPath.end(); i++)
            {
               (*i)->assign(**l);
            }
            currentAssignment->AddAssignment(*l, netPath);
        }

        if ( result == FAILURE ) {
           delete currentAssignment;
	   } else {
            assignments.insert(currentAssignment);
		}
	}
	return result;
}

Link * Annealing::createDummyLink(Link * virtualLink, Assignment * assignment)
{
    if ( virtualLink == 0 )
        return 0;

    Element * first = getCastedAssignment(virtualLink->getFirst(), assignment);
    Element * second = getCastedAssignment(virtualLink->getSecond(), assignment);

    if ( first == 0 || second == 0 )
        return 0;

    Link * dummyLink = new Link("dummy_link", virtualLink->getCapacity(), virtualLink->getMaxCapacity());
    dummyLink->bindElements(first, second);
    return dummyLink;
}

Element * Annealing::getCastedAssignment(Element * element, Assignment * assignment)
{
    if ( element->isNode() )
        return assignment->GetAssignment((Node *)element);
    if ( element->isStore() )
        return assignment->GetAssignment((Store *)element);
    return 0;
}

	

Algorithm::Result Annealing::schedule()
{
    Result result = generatePrevAssignments();
    kol = 0;
    curNetwork = (*network);
    state = 1;
    counterFail = 1;
    copyPrevAssignmentsToCur();
	double temperature, start_temperature = 100;
	int delta = 0;
	double p = 1; 
	int step = 2;
	state = 1;
	counterFail = 1;
	//network->printNetwork();
	//printRequests();
	copyPrevAssignmentsToCur();
	do {
		temperature = start_temperature * log(1 + step) / (1 + step);
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
			//acout << "cur:  " << curAssignments.size() << ", prev: " << prevAssignments.size() << endl;
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
	} while (temperature > 2 && assignments.size() != requests.size());
	if (temperature <= 2) {
		cout << "Temperature is low to continue" << endl;
	}
	(*network) = bestNetwork;
	//network->printNetwork();
	assignLinks();
	
	printf("Assigned total of %d from %d requests\n", assignments.size(), requests.size());
	//printAssignments(assignments);
	//cout << kol << endl;
	//cout << endl << "printing bestNetwork..." << endl;
	//bestNetwork.printNetwork();
	    
    if ( assignments.size() == requests.size() )
        return SUCCESS;
    else if ( assignments.size() != 0 )
        return PARTIAL;
    else
        return FAILURE;
}

