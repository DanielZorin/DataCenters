#pragma once

#include "defs.h"

class QDomElement;

class XMLFactory
{
public:
    static Computer * createComputer(const QDomElement & element);
    static Store * createStore(const QDomElement & element);
    static Switch * createSwitch(const QDomElement & element);
    static Link * createLink(const QDomElement & element);
};
