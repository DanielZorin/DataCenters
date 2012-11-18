#ifndef PUBLICDEFS_H
#define PUBLICDEFS_H

class Element;

class NetworkingElement;

class Link;
class Switch;

class ComputationalElement;

class Store;
class Node;

class Replication;

#include <set>

typedef std::set<Node *> Nodes;
typedef std::set<Store *> Stores;
typedef std::set<Switch *> Switches;
typedef std::set<Link *> Links;

#include <vector>

typedef std::vector<NetworkingElement *> NetPath;

#include <map>

typedef std::pair<Node *, Node *> NodeAssignment;
typedef std::pair<Store *, Store *> StoreAssignment;
typedef std::pair<Link *, NetPath> LinkAssignment;

class Assignment;
typedef std::set<Assignment *> Assignments;

class Request;
typedef std::set<Request *> Requests;

class Network;

// To differ virtual resources from physical resources
typedef Node VirtualMachine;
typedef Store Storage;
typedef Link VirtualLink;

#endif // PUBLICDEFS_H
