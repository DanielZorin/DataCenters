#include "tenantxmlfactory.h"

#include "factory.h"
#include "request.h"
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

    request = new Request(elements);
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


