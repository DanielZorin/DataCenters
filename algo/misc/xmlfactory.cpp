#include "xmlfactory.h"

#include "computer.h"
#include "store.h"
#include "link.h"
#include "switch.h"

#include <QtXml/QDomElement>

Computer * XMLFactory::createComputer(const QDomElement & element) {
    Computer * computer = new Computer();
    return computer;
}

Store * XMLFactory::createStore(const QDomElement & element) {
    Store * store = new Store();
    return store;
}

Switch * XMLFactory::createSwitch(const QDomElement & element) {
    Switch * sw = new Switch();
    return sw;
}

Link * XMLFactory::createLink(const QDomElement & element) {
    Link * link = new Link();
    return link;
}

