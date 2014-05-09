#include "tenantxmlfactory.h"
#include "resourcesxmlfactory.h"

#include "factory.h"
#include "request.h"
#include "port.h"
#include "node.h"
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

void TenantXMLFactory::commitAssignmentData(const ResourcesXMLFactory& resourceFactory) {
    if ( !request->isAssigned() )
        return;

    Elements elements =  request->getElements();
    Elements::const_iterator it = elements.begin();
    for ( ; it != elements.end(); ++it ) {
        if ( (*it)->isAssigned() ) {
            elementsXML[*it].attribute("assignedTo", resourceFactory.getName((*it)->getAssignee()));

            // assign ports
            if ( (*it)->isNode() ) {
                Ports ports = (*it)->toNode()->getPorts();
                for ( Ports::const_iterator pit = ports.begin(); pit != ports.end(); ++pit ) {
                    if ( (*pit)->getAssignee() != 0 ) {
                        QString portName = QString::fromUtf8((*pit)->getName().c_str());
                        QString assigneeName = QString::fromUtf8((*pit)->getAssignee()->getName().c_str());
                        setPortAssignee(elementsXML[*it].elementsByTagName("port"), portName, assigneeName);
                    }
                }
            }
        }
    }
}

