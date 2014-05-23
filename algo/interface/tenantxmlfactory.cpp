#include "tenantxmlfactory.h"
#include "resourcesxmlfactory.h"

#include "factory.h"
#include "request.h"
#include "port.h"
#include "node.h"
#include "link.h"
#include <list>

TenantXMLFactory::TenantXMLFactory(const QDomElement & element) 
:
    tenant(element)
{
    QStringList elementTypes;
    elementTypes << "vm" << "st" << "netelement" << "vnf" << "domain" << "link";
    elementsXML = Factory::getXmlElementsByTypes(elementTypes, tenant);

    std::list<Element*> list = elementsXML.keys().toStdList();
    Elements elements = Elements(list.begin(), list.end());

    request = new Request(elements, element.attribute("name").toStdString());
}

TenantXMLFactory::~TenantXMLFactory() {
    foreach( Element* elem, elementsXML.keys() ) {
        delete elem;
    }
    delete request;
}

Request * TenantXMLFactory::getRequest() const {
    return request;
}

void setPortAssignee(QDomNodeList portsXml, QString portName, QString assigneeName) {
    for ( int i = 0; i < portsXml.size(); ++i ) {
        if (portsXml.item(i).toElement().attribute("name") == portName)
            portsXml.item(i).toElement().attribute("assignedTo", assigneeName);
    }
}

void TenantXMLFactory::commitPartialAssignmentData(const class ResourcesXMLFactory& resourceFactory) {
    Elements elements =  request->getElements();
    for ( Elements::iterator it = elements.begin(); it != elements.end(); ++it ) {
        Element * e = *it;

        if ( !e->isAssigned() )
            continue;

        // check whether this is virtual link first and it is assigned
        if ( e->isLink() ) {
            Path route = e->toLink()->getRoute();
            elementsXML[e].setAttribute("assignedTo", getPathXml(route, resourceFactory));
        } else {
            elementsXML[e].setAttribute("assignedTo", resourceFactory.getName(e->getAssignee()));
        }
        // assign ports
        if ( !e->isNode() )
            continue;

        Ports ports = (*it)->toNode()->getPorts();
        for ( Ports::const_iterator pit = ports.begin(); pit != ports.end(); ++pit ) {
            Port * p = *pit;
            if ( p->getAssignee() == 0 )
                continue;

            QString portName = QString::fromUtf8((*pit)->getName().c_str());
            QString assigneeName = QString::fromUtf8((*pit)->getAssignee()->getName().c_str());
            setPortAssignee(elementsXML[*it].elementsByTagName("port"), portName, assigneeName);
        }
    }
}

void TenantXMLFactory::commitAssignmentData(const ResourcesXMLFactory& resourceFactory) {
    if ( !request->isAssigned() )
        return;

    commitPartialAssignmentData(resourceFactory);

}

QString TenantXMLFactory::getPathXml(Path& route, const ResourcesXMLFactory & resourceFactory) const {
    std::vector<Element *> path = route.getPath();
    QStringList unjoinedResult;

    for (std::vector<Element *>::iterator it = path.begin(); it != path.end(); ++it) {
        Element *e = *it;
        if ( !e->isEdge() )
            continue;
        Port* port1 = e->toEdge()->getFirst();
        Port* port2 = e->toEdge()->getSecond();

        unjoinedResult << getPhysicalPortXML(port1, resourceFactory)
            << getPhysicalPortXML(port2, resourceFactory);
    }

    return unjoinedResult.join("; ");
}

QString TenantXMLFactory::getPhysicalPortXML(Port * port, const ResourcesXMLFactory & rf) const {
    return QString("%1:%2")
        .arg(rf.getName(port->getParentNode()))
        .arg(QString::fromStdString(port->getName()));
}
