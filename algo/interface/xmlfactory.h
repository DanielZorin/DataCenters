#pragma once

#include "factory.h"

#include <QMap>
#include <QtXml/QDomDocument>

class QDomElement;

class XMLFactory : public Factory
{
public:
    typedef QMap<uint, Element *> IDS;
    typedef QMap<QString, ResourceType *> ResourceTypes;

    XMLFactory(const QString & contents);
    virtual ~XMLFactory();
    virtual Element * getElementById(uint id) const;
    virtual QString getResult() { return getXML(); }
private:
    Computer * createComputer(const QDomElement & element);
    Store * createStore(const QDomElement & element);
    Switch * createSwitch(const QDomElement & element);
    Link * createLink(const QDomElement & element);
    Request * createRequest(const QDomElement & element);
    Network * createNetwork(const QDomElement & element);

    void insertElement(Element *, const QDomElement & element);
    void wireLinks(Elements & links, const IDS & ids);
    IDS populateIds(Elements & nodes ) const;
    void pushAssignments();
    void pushNodeAssignments(Elements & nodes);
    void pushEdgeAssignments(Elements & edges);
    QString getXML();

    uint getUidByElement(Element * element) const;

    void parseResourceTypes(const QDomElement & resourceTypesXML);
    void parseResourceTypes(ResourceTypes& resourcetypes, const QDomElement & resourceTypeXML);
    void clearResourceTypes(ResourceTypes& resourcetypes);
private:
    QDomDocument document;
    QMap<Element *, QDomElement> elementsXML;
    QMap<Request *, QDomElement> requestsXML;
    IDS networkIds;

    ResourceTypes computerTypes;
    ResourceTypes storeTypes;
};
