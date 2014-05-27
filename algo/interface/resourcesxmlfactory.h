#pragma once

#include "defs.h"
#include "factory.h"

class QDomElement;
class NetworkXMLFactory;

class ResourcesXMLFactory {
public:
    //typedef QMap<QString, Element *> IDS;

	ResourcesXMLFactory(const QDomElement & element);
    virtual ~ResourcesXMLFactory();
    Network * getNetwork() const;
    // void commitAssignmentData(NetworkXMLFactory * nf);

    class QString getName(Element*) const;

private:
    Network * network;
    QDomElement networkXml;
    Factory::ElementsMap elementsXML;
};
