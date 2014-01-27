#pragma once

#include "factory.h"

#include <QMap>
#include <QtXml/QDomDocument>

class QDomElement;

class XMLFactory : public Factory
{
public:
    XMLFactory(const QString & contents);
    virtual const Element * getElementById(uint id) const;
    virtual QString getResult() const { return getXML(); }
private:
    Computer * createComputer(const QDomElement & element);
    Store * createStore(const QDomElement & element);
    Switch * createSwitch(const QDomElement & element);
    Link * createLink(const QDomElement & element);
    Request * createRequest(const QDomElement & element);
    Network * createNetwork(const QDomElement & element);

    void insertElement(Element *, const QDomElement & element);
    QString getXML() const;
private:
    QDomDocument document;
    QMap<Element *, QDomElement> elementsXML;
    QMap<Request *, QDomElement> requestsXML;
};
