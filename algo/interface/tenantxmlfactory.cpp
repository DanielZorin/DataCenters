#include "tenantxmlfactory.h"

#include "operation.h"
#include "request.h"
#include "switch.h"
#include "store.h"
#include "computer.h"
#include "serviceAsProvider.h"

#include "elementfactory.h"

#include <QStringList>
#include <QDomNamedNodeMap>

TenantXMLFactory::TenantXMLFactory(const QDomElement & element) 
:
    tenant(element)
{
    QStringList elementTypes;
    elementTypes << "vm" << "st" << "netelement" << "vnf" << "domain" << "link";
    Elements elements = getElementsByType(elementTypes, tenant);

    request = new Request(elements);
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

TenantXMLFactory::Properties TenantXMLFactory::getAttributesFromXML(const QDomNamedNodeMap & m) const {
    Properties result;
    for (int i = 0; i < m.length(); i++) {
        QDomNode node = m.item(i);
        result.insert(node.nodeName(), node.nodeValue());
    }
    return result;
}

TenantXMLFactory::Properties TenantXMLFactory::getParametersFromXML(const QDomNodeList & l) const {
    Properties result;
    for (int i = 0; i < l.length(); i++) {
        QDomElement e = l.at(i).toElement();
        if ( e.tagName() != "parameter" )
            continue;

        QString parameterName = e.attribute("parameter_name");
        QString parameterType = e.attribute("parameter_type");
        QVariant parameterValue = e.attribute("parameter_value");

        if ( parameterType == "int" )
            parameterValue.convert(QVariant::Int);
        else if ( parameterType == "real" )
            parameterValue.convert(QVariant::Double);

        result.insert(parameterName, parameterValue);
    }

    return result;
}

Element * TenantXMLFactory::createElementFromXML(const QDomElement & element) {
    QString type = element.tagName();
    Element * result = 0;

    if ( type == "link" )
        result = createLink(element);
    else 
        result = createNode(element);

    if ( result != 0 )
        elementsXML.insert(result, element);

    return result;
}

Link * TenantXMLFactory::createLink(const QDomElement & e) const {
    Link * link = new Link(); 
    return link;
}

Element * TenantXMLFactory::createNode(const QDomElement & e) const {
    Element * node = 0;
    QString type = e.tagName();
    Properties properties = getAttributesFromXML(e.attributes());

    if ( type == "vm" || type == "vnf" ) {
        Computer * vm = new Computer();
        node = ElementFactory::populate(vm, properties, this);
    } else if ( type == "st" ) {
        Store * st = new Store();
        node = ElementFactory::populate(st, properties, this);
    } else if ( type == "netelement" ) {
        Switch * sw = new Switch();
        node = ElementFactory::populate(sw, properties, this);
    }

    return node;
}

