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

        // check whether this is virtual link first and it is assigned
        if ( e->isLink() && e->toLink()->getRoute().length() >= 0 ) {
        	Path route = e->toLink()->getRoute();
        	elementsXML[e].setAttribute("assignedTo", getPathXml(route, resourceFactory));
        }

        if ( !e->isAssigned() )
            continue;
        elementsXML[e].setAttribute("assignedTo", resourceFactory.getName(e->getAssignee()));

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
	QString result = QString("");
	std::vector<Element *> path = route.getPath();

	for (std::vector<Element *>::const_iterator it = path.begin(); it != path.end(); ++it) {
		const Element *e = *it;
		if ( e->isEdge() ) {
			const Port* port1 = e->toEdge()->getFirst();
			const Port* port2 = e->toEdge()->getSecond();

			if (result.length() ==  0) {
				result += resourceFactory.getName(port1->getParentNode());
				result += ":";
				QString portName = QString::fromUtf8(port1->getName().c_str());
				result += portName;
			}

			result += "; ";
			result += resourceFactory.getName(port2->getParentNode());
			result += ":";
			QString portName = QString::fromUtf8(port2->getName().c_str());
			result += portName;
		}
	}

	return result;
}
