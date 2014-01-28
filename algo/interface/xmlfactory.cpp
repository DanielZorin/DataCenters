#include "xmlfactory.h"
#include "elementfactory.h"

#include "computer.h"
#include "store.h"
#include "link.h"
#include "switch.h"
#include "network.h"
#include "request.h"
#include "operation.h"
#include "criteria.h"

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

    QDomNodeList requestList = root.elementsByTagName("tenant");
    for ( uint i = 0; i < requestList.length(); i++ )
        requests.insert(createRequest(requestList.item(i).toElement()));
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
    {
        QDomNodeList vms = element.elementsByTagName("vm");
        QDomNodeList storages = element.elementsByTagName("storage");
        QDomNodeList switches = element.elementsByTagName("vswitch");
        QDomNodeList links = element.elementsByTagName("tunnel");

        for(uint i = 0; i < vms.length(); i++)
            elements.insert(createComputer(vms.item(i).toElement()));
        for(uint i = 0; i < storages.length(); i++ )
            elements.insert(createStore(storages.item(i).toElement()));
        for(uint i = 0; i < switches.length(); i++ )
            elements.insert(createSwitch(switches.item(i).toElement()));
        for(uint i = 0; i < links.length(); i++ )
            elements.insert(createLink(links.item(i).toElement()));
    }

    QMap<QString, QVariant> defaultRequestValues;
    defaultRequestValues["physical"] = "false";
    for ( Elements::iterator i = elements.begin(); i != elements.end(); i++ )
        ElementFactory::populate(*i, defaultRequestValues);

    Elements nodes = Operation::filter(elements, Criteria::isNode);
    IDS ids = populateIds(nodes);

    Elements links = Operation::filter(elements, Criteria::isEdge);
    wireLinks(links, ids);

    Request * request = new Request(elements);

    requestsXML.insert(request, element);
    return request;
}

Network * XMLFactory::createNetwork(const QDomElement & element) {
    Elements elements;
    {
        QDomNodeList computers = element.elementsByTagName("computer");
        QDomNodeList stores = element.elementsByTagName("store");
        QDomNodeList switches = element.elementsByTagName("switch");
        QDomNodeList links = element.elementsByTagName("link");

        for ( uint i = 0; i < computers.length(); i++ )
            elements.insert(createComputer(computers.at(i).toElement()));
        for ( uint i = 0; i < stores.length(); i++ )
            elements.insert(createStore(stores.at(i).toElement()));
        for ( uint i = 0; i < switches.length(); i++ )
            elements.insert(createSwitch(switches.at(i).toElement()));
        for ( uint i = 0; i < links.length(); i++ )
            elements.insert(createLink(links.at(i).toElement()));

    }
    QMap<QString, QVariant> defaultNetworkValues;
    defaultNetworkValues["physical"] = true;
    defaultNetworkValues["available"] = true;

    for ( Elements::iterator i = elements.begin(); i != elements.end(); i++ ) {
        Element * element = *i;
        ElementFactory::populate(element, defaultNetworkValues); 
    }

    Elements nodes = Operation::filter(elements, Criteria::isNode);
    networkIds = populateIds(nodes);

    Elements links = Operation::filter(elements, Criteria::isLink);
    wireLinks(links, networkIds);

    Network * network = new Network(elements);
    return network;
}

Element * XMLFactory::getElementById(uint id) const {
    
}

void XMLFactory::insertElement(Element * e, const QDomElement & element) {
    elementsXML.insert(e, element);
}

QString XMLFactory::getXML() {
    pushAssignments();
    document.toString(4);
}

void XMLFactory::wireLinks(Elements & links, const IDS & ids) {
    for ( Elements::iterator i = links.begin(); i != links.end(); i++ ) {
        Link * link = (*i)->toLink();
        const QDomElement & linkElement = elementsXML[link];
        Node * from = ids[linkElement.attribute("from", "0").toUInt()]->toNode();
        Node * to = ids[linkElement.attribute("to", "0").toUInt()]->toNode();

        link->connect(from, to);
        from->addEdge(link);
        to->addEdge(link);
    }
}

XMLFactory::IDS XMLFactory::populateIds(Elements & nodes) const {
    IDS ids;

    for ( Elements::iterator i = nodes.begin(); i != nodes.end(); i++ ) {
        Element * element = *i;
        uint id = elementsXML[element].attribute("id", "0").toUInt();
        ids.insert(id, element);
    }

    return ids;
}

void XMLFactory::pushAssignments()
{

}
