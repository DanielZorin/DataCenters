#include "xmlfactory.h"

#include "computer.h"
#include "store.h"
#include "link.h"
#include "switch.h"
#include "network.h"
#include "request.h"

#include <QtXml/QDomElement>
#include <QtXml/QDomNodeList>

Computer * XMLFactory::createComputer(const QDomElement & element) {
    Computer * computer = new Computer();
    elementsXML.insert(computer, element);
    return computer;
}

Store * XMLFactory::createStore(const QDomElement & element) {
    Store * store = new Store();
    elementsXML.insert(store, element);
    return store;
}

Switch * XMLFactory::createSwitch(const QDomElement & element) {
    Switch * sw = new Switch();
    elementsXML.insert(sw, element);
    return sw;
}

Link * XMLFactory::createLink(const QDomElement & element) {
    Link * link = new Link();
    elementsXML.insert(link, element);
    return link;
}

Request * XMLFactory::createRequest(const QDomElement & element) {
    Elements elements;
    Request * request = new Request(elements);
    return request;
}

Network * XMLFactory::createNetwork(const QDomElement & element) {
    QDomNodeList computers = element.elementsByTagName("computer");
    QDomNodeList stores = element.elementsByTagName("storage");
    QDomNodeList switches = element.elementsByTagName("router");
    QDomNodeList links = element.elementsByTagName("link");

    Elements elements;
    for ( uint i = 0; i < computers.length(); i++ )
        elements.insert(createComputer(computers.at(i).toElement()));
    for ( uint i = 0; i < stores.length(); i++ )
        elements.insert(createStore(stores.at(i).toElement()));
    for ( uint i = 0; i < switches.length(); i++ )
        elements.insert(createSwitch(switches.at(i).toElement()));
    for ( uint i = 0; i < links.length(); i++ )
        elements.insert(createLink(links.at(i).toElement()));

    for ( Elements::iterator i = elements.begin(); i != elements.end(); i++ ) {
        Element * element = *i;
        element->physical = true;
        element->available = true;
    }

    Network * network = new Network(elements);
    return network;
}
