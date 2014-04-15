#pragma once

#include "defs.h"

#include <QMap>
#include <QVariant>
#include <QtXml/QDomElement>

class QDomElement;
class NetworkXMLFactory;

class TenantXMLFactory {
public:
    typedef QMap<QString, Element *> IDS;
    typedef QMap<QString, QVariant> Properties;

    TenantXMLFactory(const QDomElement & element);
    virtual ~TenantXMLFactory();
    Request * getRequest() const;
    void commitAssignmentData(NetworkXMLFactory * nf);
private:
    Elements getElementsByType(const class QStringList &, const QDomElement &);
    Elements createElementsFromNodeList(class QDomNodeList &);
    Element * createElementFromXML(const QDomElement & element);
    Properties getAttributesFromXML(const class QDomNamedNodeMap &) const;
    Properties getParametersFromXML(const class QDomNodeList &) const;

    Link * createLink(const QDomElement & element) const;
    Element * createNode(const QDomElement & element) const;
private:
    Request * request;
    QDomElement tenant;
    QMap<Element *, QDomElement> elementsXML;
};
