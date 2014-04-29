#pragma once

#include "defs.h"
#include "factory.h"

class QDomElement;
class NetworkXMLFactory;

class TenantXMLFactory {
public:
    //typedef QMap<QString, Element *> IDS;

    TenantXMLFactory(const QDomElement & element);
    virtual ~TenantXMLFactory();
    Request * getRequest() const;
    // void commitAssignmentData(NetworkXMLFactory * nf);

private:
    Request * request;
    QDomElement tenant;
    Factory::ElementsMap elementsXML;
};