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
    void commitAssignmentData(const class ResourcesXMLFactory& resourceFactory);
    void commitPartialAssignmentData(const class ResourcesXMLFactory & resourceFactory);

    // Get string representation for path in the form "NodeName: PortName; NodeName:PortName ..."
    QString getPathXml(class Path& path, const class ResourcesXMLFactory & resourceFactory) const;
    QString getPhysicalPortXML(Port * port, const class ResourcesXMLFactory & rf) const;
private:

    // Method to check whether the element is non-router net-element
    bool isNonRouterSwitch(const Element* elem);
private:
    Request * request;
    QDomElement tenant;
    Factory::ElementsMap elementsXML;
};
