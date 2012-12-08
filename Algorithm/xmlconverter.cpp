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
#include <QStringList>

class Overseer
{
protected:
    QMap<uint, Node *> nodes;
    QMap<uint, Store *> stores;
    QMap<uint, Switch *> switches;

    QMap<uint, QDomElement> nodeXMLCache;
    QMap<uint, QDomElement> storeXMLCache;
    QMap<uint, QDomElement> switchXMLCache;
    QMap<Link*, QDomElement> linkXMLCache;
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
            nodeXMLCache[uid] = node;
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
            uint replicationCapacity = store.attribute("replicationcapacity").toUInt();
            Store * astore = new Store(name.toStdString(), capacity, capacity, type, replicationCapacity);
            addStore(uid, astore);
            storeXMLCache[uid] = store;
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
            switchXMLCache[uid] = eswitch;
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
            linkXMLCache[alink] = link;
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

    uint getIdByElement(Element * element) const
    {
        if ( element->isNode() )
        {
            Node * node;
            node = static_cast<Node *>(element);
            return nodes.key(node);
        }
        else if ( element->isStore() )
        {
            Store * store;
            store = static_cast<Store *>(element);
            return stores.key(store);
        }
        else if ( element->isSwitch() )
        {
            Switch * aswitch;
            aswitch = static_cast<Switch *>(element);
            return switches.key(aswitch);
        }
        else
        {
            return (uint)-1;
        }
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
        this->demand = demand;
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

    QString getName() { return QString(request->getName().c_str()); }

    void assignVMs(Assignment * a, NetworkOverseer const& network)
    {
        for ( QMap<uint, Node *>::iterator i = nodes.begin(); 
                i != nodes.end(); i++)
        {
            uint vmId = i.key();
            Node * vm = i.value();
            Node * node = a->GetAssignment(vm);
            uint nodeId = network.getIdByElement(node);
            QDomElement xmlNode = nodeXMLCache[vmId];
            xmlNode.setAttribute("assignedto", nodeId);
        }
    }

    void assignStorages(Assignment * a, NetworkOverseer const& network)
    {
        for ( QMap<uint, Store *>::iterator i = stores.begin();
                i != stores.end(); i++)
        {
            uint storageId = i.key();
            Store * storage = i.value();
            Store * store = a->GetAssignment(storage);
            uint storeId = network.getIdByElement(store);
            QDomElement xmlStore = storeXMLCache[storageId];
            xmlStore.setAttribute("assignedto", storeId);
        } 
    }

    QString getAssignmentChain(NetPath path, NetworkOverseer const& network)
    {
        if ( path.size() == 0 )
            return QString("none");

        QStringList result;
        for ( NetPath::iterator i = path.begin(); i != path.end(); i++)
        {
            NetworkingElement * ne = *i;
            if ( ne->isSwitch() ) 
            {
                uint uid = network.getIdByElement(ne);
                result << QString().setNum(uid);
            }
        }

        NetPath::iterator first = path.begin(), last = path.end();
        if ( (*first)->isLink() && (*(--last))->isLink() )
        {
            Link * begin = static_cast<Link *>(*first);
            Link * end = static_cast<Link *>(*last);
            uint firstComp = network.getIdByElement(begin->getLinkedComputationalElement());
            uint lastComp = network.getIdByElement(end->getLinkedComputationalElement());
            result.prepend(QString().setNum(firstComp));
            result.append(QString().setNum(lastComp));
        
        } 
        return result.join(";");
    }

    void assignLinks(Assignment * a, NetworkOverseer const& network)
    {
        for ( QMap<Link *, QDomElement>::iterator i = linkXMLCache.begin();
                i != linkXMLCache.end(); i++)
        {
            Link * link = i.key();
            QDomElement xmlLink = i.value();
            NetPath netPath = a->GetAssignment(link);
            QString assignmentChain = getAssignmentChain(netPath, network);
            xmlLink.setAttribute("assignedto", assignmentChain);
        }
    }

    void assign(Assignment * a, NetworkOverseer const& network)
    {
        demand.setAttribute("assigned", "True");
        assignVMs(a, network);
        assignStorages(a, network);
        assignLinks(a, network);
    }

private:
    Request * request;
    QDomElement demand;
};

// XMLConverter implementation

XMLConverter::XMLConverter(QString & contents)
:
    document("XMLConversion")
{
    document.setContent(contents);
    parseContents();
}

XMLConverter::~XMLConverter()
{
    delete networkOverseer;
    for( uint i = 0; i < requestOverseers.length(); i++)
        delete requestOverseers[i];
}

QString XMLConverter::getMixdownContent()
{
    int indentation = 4;
    return document.toString(indentation);
}

Network* XMLConverter::getNetwork()
{
    return networkOverseer->getNetwork();
}

Requests XMLConverter::getRequests()
{
    Requests result;
    for (uint i = 0; i < requestOverseers.length(); i++)
    {
        RequestOverseer * requestOverseer = requestOverseers[i];
        result.insert(requestOverseer->getRequest());
    }
    return result;
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

RequestOverseer * XMLConverter::getOverseerByRequestName(QString & requestName)
{
    for (uint i = 0; i < requestOverseers.length(); i++)
    {
        RequestOverseer * overseer = requestOverseers[i];
        if ( overseer->getName() == requestName )
            return overseer;
    }
    return 0;
}

void XMLConverter::setAssignments(Assignments & assignments)
{
    for (Assignments::iterator i = assignments.begin(), e = assignments.end();
            i != e; i++)
    {
        Assignment * assignment = *i;
        QString assignmentName(assignment->getName().c_str());
        RequestOverseer * overseer = getOverseerByRequestName(assignmentName);
        overseer->assign(assignment, *networkOverseer);
    }
}
