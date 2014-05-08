#include "factory.h"

#include "switch.h"
#include "store.h"
#include "computer.h"
#include "port.h"

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
        Element* element = createElementFromXML(xmlElement, elementsMap);
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

void Factory::addPortsFromXML(const QDomElement& element, Node* node) {
	QDomNodeList portsXml = element.elementsByTagName("port");
	for (int i = 0; i < portsXml.size(); ++i ) {
		Port* port = new Port(portsXml.at(i).toElement().attribute("name").toStdString(), node);
		node->addPort(port);
	}

	// TODO: add external ports
}

Element * Factory::createElementFromXML(const QDomElement & element, const ElementsMap& elementsMap) {
    QString type = element.tagName();
    Element * result = 0;

    if ( type == "link" )
        result = createLink(element, elementsMap);
    else
        result = createNode(element);

    return result;
}

Element* getElementByName(const QString name, const Factory::ElementsMap& elementsMap) {
	foreach ( QDomElement elem, elementsMap.values() ) {
		if ( elem.attribute("name") == name ) {
			return elementsMap.key(elem);
		}
	}
	return 0;
}

Link * Factory::createLink(const QDomElement & e, const ElementsMap& elementsMap) {
    Element* elem1 = getElementByName(e.attribute("node1"), elementsMap);
    Element* elem2 = getElementByName(e.attribute("node2"), elementsMap);
    if ( elem1 == 0 || elem2 == 0 )
    	return 0;

    Link * link = new Link();
    Port* port1 = elem1->toNode()->getPortByName(e.attribute("port1").toStdString());
    Port* port2 = elem2->toNode()->getPortByName(e.attribute("port2").toStdString());
    link->connect(port1, port2);
    port1->connect(link, port2);
    port2->connect(link, port1);
    return link;
}

void Factory::setSwitchAttributes(Switch* sw, const QDomElement & e) {
	if ( e.attribute("is_router") == "1" ) {
		sw->attributes |= Switch::ROUTER;
		QDomNodeList services = e.elementsByTagName("service");
		for ( int i = 0; i < services.size(); ++i ) {
			QString service = services.item(i).toElement().attribute("name");
			if ( service == "FW")
				sw->attributes |= Switch::FW;

			if ( service == "DHCP")
				sw->attributes |= Switch::DHCP;

			if ( service == "NAT")
				sw->attributes |= Switch::NAT;

			if ( service == "PAT")
				sw->attributes |= Switch::PAT;
		}
	}
}

Element * Factory::createNode(const QDomElement & e) {
    Element * node = 0;
    QString type = e.tagName();
    Properties properties = getParametersFromXML(e.elementsByTagName("parameter"));


    if ( type == "vm" || type == "vnf" || type == "server" ) {
        Computer * vm = new Computer();
        node = ElementFactory::populate(vm, properties);
    } else if ( type == "st" || type == "storage" ) {
        Store * st = new Store();
        node = ElementFactory::populate(st, properties);
    } else if ( type == "netelement" ) {
        Switch * sw = new Switch();

        // switch has additional attributes (is_router, ...)
        setSwitchAttributes(sw, e);
        node = ElementFactory::populate(sw, properties);
    }

    if ( node != 0 )
    	addPortsFromXML(e, node->toNode());

    return node;
}
