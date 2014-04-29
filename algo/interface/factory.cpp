#include "factory.h"

#include "switch.h"
#include "store.h"
#include "computer.h"

#include "elementfactory.h"

#include <QStringList>
#include <QDomNamedNodeMap>

Factory::ElementsMap Factory::getXmlElementsByTypes(const QStringList & types, const QDomElement & root) {
	ElementsMap result;
    for (int i = 0; i < types.size(); i++ ) {
        QString type = types.at(i);
        QDomNodeList elementsList = root.elementsByTagName(type);
        createElementsFromNodeList(elementsList, result);
    }
    return result;
}

void Factory::createElementsFromNodeList(QDomNodeList & list, ElementsMap& elementsMap) {
    for (int i = 0; i < list.size(); i++) {
        QDomElement xmlElement = list.at(i).toElement();
        Element* element = createElementFromXML(xmlElement);
        if ( element != 0 ) {
        	elementsMap[element] = xmlElement;
        }
    }
}

Factory::Properties Factory::getAttributesFromXML(const QDomNamedNodeMap & m) {
    Properties result;
    for (int i = 0; i < m.length(); i++) {
        QDomNode node = m.item(i);
        result.insert(node.nodeName(), node.nodeValue());
    }
    return result;
}

Factory::Properties Factory::getParametersFromXML(const QDomNodeList & l) {
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

Element * Factory::createElementFromXML(const QDomElement & element) {
    QString type = element.tagName();
    Element * result = 0;

    if ( type == "link" )
        result = createLink(element);
    else
        result = createNode(element);

    return result;
}

Link * Factory::createLink(const QDomElement & e) {
    Link * link = new Link();
    return link;
}

Element * Factory::createNode(const QDomElement & e) {
    Element * node = 0;
    QString type = e.tagName();
    Properties properties = getAttributesFromXML(e.attributes());

    if ( type == "vm" || type == "vnf" || type == "server" ) {
        Computer * vm = new Computer();
        node = ElementFactory::populate(vm, properties);
    } else if ( type == "st" || type == "storage" ) {
        Store * st = new Store();
        node = ElementFactory::populate(st, properties);
    } else if ( type == "netelement" ) {
        Switch * sw = new Switch();
        node = ElementFactory::populate(sw, properties);
    }

    return node;
}
