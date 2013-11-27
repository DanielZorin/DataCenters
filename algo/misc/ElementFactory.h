#ifndef ELEMENTFACTORY_H
#define ELEMENTFACTORY_H

#include "common/defs.h"

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
