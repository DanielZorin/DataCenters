#pragma once

#include <set>
#include <map>

// Elements
class Element;
typedef std::set<Element *> Elements;

// Physical Elements
class Node;

class Computer;
class Store;
class Switch;

class Edge;

class Link;
class Port;
typedef std::set<Port*> Ports;

class Graph;
class Network;
class Request;
class Parameter;
class ParameterValue;
typedef std::map<Parameter *, ParameterValue *> Parameters;
typedef std::set<Request *> Requests;
typedef Computer Vnf;

class Path;
