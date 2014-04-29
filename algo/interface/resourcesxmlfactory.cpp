#include "resourcesxmlfactory.h"

#include "factory.h"
#include "network.h"
#include <list>

ResourcesXMLFactory::ResourcesXMLFactory(const QDomElement & element)
:
	networkXml(element)
{
    QStringList elementTypes;
    elementTypes << "server" << "storage" << "netelement" << "link";
    elementsXML = Factory::getXmlElementsByTypes(elementTypes, networkXml);

    std::list<Element*> list = elementsXML.keys().toStdList();
    Elements elements = Elements(list.begin(), list.end());
    for (Elements::iterator it = elements.begin(); it != elements.end(); ++it ) {
    	(*it)->physical = true;
    }

    network = new Network(elements);
}

ResourcesXMLFactory::~ResourcesXMLFactory() {
	foreach( Element* elem, elementsXML.keys() ) {
		delete elem;
	}
	delete network;
}

Network * ResourcesXMLFactory::getNetwork() const {
    return network;
}


