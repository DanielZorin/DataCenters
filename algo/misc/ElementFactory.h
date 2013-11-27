#ifndef ELEMENTFACTORY_H
#define ELEMENTFACTORY_H

class Computer;
class Store;
class Switch;
class Link;
class QDomElement;

class ElementXMLFactory
{
public:
    static Computer * createComputer(const QDomElement & element);
    static Store * createStore(const QDomElement & element);
    static Switch * createSwitch(const QDomElement & element);
    static Link * createLink(const QDomElement & element);
};

#endif // ELEMENTFACTORY_H
