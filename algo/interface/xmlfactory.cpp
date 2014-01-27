#include "xmlfactory.h"
#include "elementfactory.h"

#include "computer.h"
#include "store.h"
#include "link.h"
#include "switch.h"
#include "network.h"
#include "request.h"

#include <QtXml/QDomElement>
#include <QtXml/QDomNodeList>

XMLFactory::XMLFactory(const QString & contents)
:
    Factory(),
    document("XMLFactory")
{
    document.setContent(contents);
    QDomElement root = document.documentElement();
    QDomNodeList resourceList = root.elementsByTagName("resources");
    QDomElement resources = resourceList.item(0).toElement();
    network = createNetwork(resources);
}

Computer * XMLFactory::createComputer(const QDomElement & element) {
    QMap<QString, QVariant> properties;
    properties["cores"] = element.attribute("cores", "0");
    properties["ram"] = element.attribute("ram", "0");

    Computer * computer = ElementFactory::populate(new Computer(), properties);
    insertElement(computer, element);
    return computer;
}

Store * XMLFactory::createStore(const QDomElement & element) {
    QMap<QString, QVariant> properties;
    properties["capacity"] = element.attribute("capacity", "0");
    properties["readrate"] = element.attribute("readrate", "0");
    properties["writerate"] = element.attribute("writerate", "0");
    properties["replicable"] = element.attribute("replicable", "false");

    Store * store = ElementFactory::populate(new Store(), properties);
    insertElement(store, element);
    return store;
}

Switch * XMLFactory::createSwitch(const QDomElement & element) {
    QMap<QString, QVariant> properties;
    properties["throughput"] = element.attribute("throughput", "0");

    Switch * sw = ElementFactory::populate(new Switch(), properties);
    insertElement(sw, element);
    return sw;
}

Link * XMLFactory::createLink(const QDomElement & element) {
    QMap<QString, QVariant> properties;
    properties["throughput"] = element.attribute("throughput", "0");

    Link * link = ElementFactory::populate(new Link(), properties);
    insertElement(link, element);
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
    QDomNodeList switches = element.elementsByTagName("switch");
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

    QMap<QString, QVariant> defaultNetworkValues;
    defaultNetworkValues["physical"] = true;
    defaultNetworkValues["available"] = true;
 
    for ( Elements::iterator i = elements.begin(); i != elements.end(); i++ ) {
        Element * element = *i;
        ElementFactory::populate(element, defaultNetworkValues); 
    }

    Network * network = new Network(elements);
    return network;
}

const Element * XMLFactory::getElementById(uint id) const {
    return 0;
}

void XMLFactory::insertElement(Element * e, const QDomElement & element) {
    elementsXML.insert(e, element);
}

QString XMLFactory::getXML() const
{
    return QString();
}
