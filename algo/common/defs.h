#ifndef DEFS_H
#define DEFS_H


#include <set>

class Element;
typedef std::set<Element *> Elements;

class Node;
typedef std::set<Node *> Nodes;

class Computer;
typedef std::set<Computer *> Computers;

class Store;
typedef std::set<Store *> Stores;

class Switch;
typedef std::set<Switch *> Switches;

class Link;
typedef std::set<Link *> Links;

class Assignment;
typedef std::set<Assignment *> Assignments;

class Request;
typedef std::set<Request *> Requests;

class Network;

#endif // DEFS_H
