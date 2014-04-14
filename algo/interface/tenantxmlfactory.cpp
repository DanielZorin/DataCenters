#include "tenantxmlfactory.h"

#include "operation.h"
#include "request.h"

#include <QStringList>

TenantXMLFactory::TenantXMLFactory(const QDomElement & element) 
:
    tenant(element)
{
    QDomElement nodeList = element.elementsByTagName("list_of_nodes").at(0).toElement();

    QStringList elementTypes;
    elementTypes << "vm" << "st" << "netelement" << "vnf" << "domain";
    Elements nodeElements = getElementsByType(elementTypes, nodeList);

    elementTypes.clear();
    elementTypes << "link" << "service_as_provider" << "service_as_user";
    Elements otherElements = getElementsByType(elementTypes, tenant);
}

TenantXMLFactory::~TenantXMLFactory() {

}

Request * TenantXMLFactory::getRequest() const {
    return request;
}

Elements TenantXMLFactory::getElementsByType(const QStringList & types, const QDomElement & root) {
    Elements result;
    for (int i = 0; i < types.size(); i++ ) {
        QString type = types.at(i);
        QDomNodeList elementsList = root.elementsByTagName(type);
        result = Operation::unite(result, createElementsFromNodeList(elementsList));
    }
    return result;
}

Elements TenantXMLFactory::createElementsFromNodeList(QDomNodeList & list) {
    Elements result;
    for (int i = 0; i < list.size(); i++) {
        QDomElement element = list.at(i).toElement();
        result.insert(createElementFromXML(element));
    }
    return result;
}

Element * TenantXMLFactory::createElementFromXML(const QDomElement & element) {
    return 0; 
}
