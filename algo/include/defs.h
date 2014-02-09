#pragma once

#include <set>
#include <map>

class Element;
typedef std::set<Element *> Elements;

class Node;

class Computer;
class Store;
class Switch;

class Edge;

class Link;

class Graph;
class Network;
class Request;
class ResourceType;
typedef std::map<ResourceType *, unsigned> ResourceVector;
typedef std::set<Request *> Requests;
