#include "xmlconverter.h"

#include "network.h"

#include "element.h"
#include "request.h"
#include "link.h"
#include "switch.h"
#include "node.h"
#include "store.h"
#include "assignment.h"

#include <QDomNode>
#include <QDomElement>

XMLConverter::XMLConverter(std::string const& contents)
:
    document("XMLConversion")
{
    network = new Network();
    QByteArray bytestreamContents(contents.c_str());
    document.setContent(bytestreamContents);
    parseContents();
}

void XMLConverter::parseContents()
{
    QDomElement root = document.documentElement();
    QDomNodeList resourcesList = root.elementsByTagName("resources");
    QDomElement resources = resourcesList.item(0).toElement();
    parseNetwork(resources);
    QDomNodeList requests = root.elementsByTagName("demand");
    parseRequests(requests);
}

void XMLConverter::parseNetwork(QDomElement & resources)
{
    QDomNodeList nodes = resources.elementsByTagName("computer");
    parseNodes(nodes);
    QDomNodeList stores = resources.elementsByTagName("storage");
    parseStores(stores);
    QDomNodeList switches = resources.elementsByTagName("router");
    parseSwitches(switches);
    QDomNodeList links = resources.elementsByTagName("link");
    parseLinks(links);
}

void XMLConverter::parseNodes(QDomNodeList & nodes)
{
    for( uint i = 0; i < nodes.length(); i++)
    {
        QDomElement node = nodes.item(i).toElement();
        uint uid = node.attribute("number").toUInt();
        QString name = node.attribute("name");
        uint capacity = node.attribute("speed").toUInt();
        Node * anode = new Node(name.toStdString(), capacity, capacity);
        addNode(uid, anode);
    }
}

void XMLConverter::addNode(uint uid, Node * node)
{
    network->addNode(node);
    nodes[uid] = node;
}

void XMLConverter::parseStores(QDomNodeList & stores)
{
    for ( uint i = 0; i < stores.length(); i++)
    {
        QDomElement store = stores.item(i).toElement();
        uint uid = store.attribute("number").toUInt();
        QString name = store.attribute("name");
        uint capacity = store.attribute("volume").toUInt();
        uint type = store.attribute("type").toUInt();
        Store * astore = new Store(name.toStdString(), capacity, capacity, type);
        addStore(uid, astore);
    }
}

void XMLConverter::addStore(uint uid, Store * store)
{
    network->addStore(store);
    stores[uid] = store;
}

void XMLConverter::parseSwitches(QDomNodeList & switches)
{
    for ( uint i = 0; i < switches.length(); i++)
    {
        QDomElement eswitch = switches.item(i).toElement();
        uint uid = eswitch.attribute("number").toUInt();
        QString name = eswitch.attribute("name");
        uint capacity = eswitch.attribute("capacity").toUInt();
        Switch * aswitch = new Switch(name.toStdString(), capacity, capacity);
        addSwitch(uid, aswitch);
    }
}

void XMLConverter::addSwitch(uint uid, Switch * aswitch)
{
    network->addSwitch(aswitch);
    switches[uid] = aswitch;
}

void XMLConverter::parseLinks(QDomNodeList & links)
{
    for ( uint i = 0; i < links.length(); i++)
    {
        QDomElement link = links.item(i).toElement();
        uint idFrom = link.attribute("from").toUInt();
        uint idTo = link.attribute("to").toUInt();
        uint capacity = link.attribute("capacity").toUInt();
        Link * alink = new Link(QString("%1_%2").arg(idFrom).arg(idTo).toStdString(),
                capacity, capacity);
        addLink(idFrom, idTo, alink);
    }
}

void XMLConverter::addLink(uint idFrom, uint idTo, Link * link)
{
    Element * from = getElementByID(idFrom);
    Element * to = getElementByID(idTo);
    link->bindElements(from, to); 
    network->addLink(link);
}

Element * XMLConverter::getElementByID(uint id)
{
    if ( nodes.contains(id) )
        return nodes[id];
    else if ( stores.contains(id) )
        return stores[id];
    else if ( switches.contains(id) )
        return switches[id];
    else
        return NULL;
}

void XMLConverter::parseRequests(QDomNodeList & requests)
{
}
