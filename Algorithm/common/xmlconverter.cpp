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
#include <QMap>

class Overseer
{
protected:
    QMap<uint, Node *> nodes;
    QMap<uint, Store *> stores;
    QMap<uint, Switch *> switches;
public:
    Overseer() {}
    virtual ~Overseer() {}
    virtual void parse(QDomElement &) = 0;

    virtual void parseNodes(QDomNodeList & nodes)
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

    virtual void parseStores(QDomNodeList & stores)
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

    virtual void parseSwitches(QDomNodeList & switches)
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

    virtual void parseLinks(QDomNodeList & links)
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

    virtual void addNode(uint uid, Node * node)
    {
        nodes[uid] = node;
    }

    virtual void addStore(uint uid, Store * store)
    {
        stores[uid] = store;
    }

    virtual void addSwitch(uint id, Switch * aswitch)
    {
        switches[id] = aswitch;
    }

    virtual void addLink(uint idFrom, uint idTo, Link * link)
    {
        Element * from = getElementByID(idFrom);
        Element * to = getElementByID(idTo);
        link->bindElements(from, to); 
    }

    Element * getElementByID(uint id)
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


};

class NetworkOverseer : public Overseer
{
public:
    NetworkOverseer()
    {
        network = new Network();
    }

    virtual ~NetworkOverseer()
    {
        delete network;
    }

    Network * getNetwork()
    {
        return network;
    }

    virtual void parse(QDomElement & resources)
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

    virtual void addNode(uint uid, Node * node)
    {
        Overseer::addNode(uid, node);
        network->addNode(node);
    }

    virtual void addStore(uint uid, Store * store)
    {
        Overseer::addStore(uid, store);
        network->addStore(store);
    }

    virtual void addSwitch(uint uid, Switch * aswitch)
    {
        Overseer::addSwitch(uid, aswitch);
        network->addSwitch(aswitch);
    }

    virtual void addLink(uint idFrom, uint idTo, Link * link)
    {
        Overseer::addLink(idFrom, idTo, link);
        network->addLink(link);
    }

private:
    Network * network;
};

class RequestOverseer : public Overseer
{
public:
    RequestOverseer()
    {
        request = new Request();
    }

    ~RequestOverseer()
    {
        delete request;
    }

    Request * getRequest()
    {
        return request;
    }

    virtual void parse(QDomElement & demand)
    {
        QString name = demand.attribute("id");
        request->setName(name.toStdString());
        QDomNodeList nodes = demand.elementsByTagName("vm");
        parseNodes(nodes);
        QDomNodeList stores = demand.elementsByTagName("storage");
        parseStores(stores);
        QDomNodeList links = demand.elementsByTagName("link");
        parseLinks(links);
    }

    virtual void addNode(uint uid, Node * node)
    {
        Overseer::addNode(uid, node);
        request->addVirtualMachine(node);
    }

    virtual void addStore(uint uid, Store * store)
    {
        Overseer::addStore(uid, store);
        request->addStorage(store);
    }

    virtual void addLink(uint idFrom, uint idTo, Link * link)
    {
        Overseer::addLink(idFrom, idTo, link);
        request->addLink(link);
    }
private:
    Request * request;
};



// XMLConverter implementation


XMLConverter::XMLConverter(std::string const& contents)
    :
        document("XMLConversion")
{
    QByteArray bytestreamContents(contents.c_str());
    document.setContent(bytestreamContents);
    parseContents();
}

XMLConverter::~XMLConverter()
{
    delete networkOverseer;
    for( uint i = 0; i < requestOverseers.length(); i++)
        delete requestOverseers[i];
}

Network* XMLConverter::getNetwork()
{
    return networkOverseer->getNetwork();
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
    networkOverseer = new NetworkOverseer();
    networkOverseer->parse(resources);
}

void XMLConverter::parseRequests(QDomNodeList & requests)
{
    for ( uint i = 0; i < requests.length(); i++ )
    {
        QDomElement request = requests.item(i).toElement();
        RequestOverseer * requestOverseer = new RequestOverseer();
        requestOverseer->parse(request);
        requestOverseers.append(requestOverseer);
    }
}
