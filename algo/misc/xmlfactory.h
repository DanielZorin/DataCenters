#pragma once

#include "defs.h"

#include <QMap>
#include <QtXml/QDomDocument>

class QDomElement;

class XMLFactory
{
    XMLFactory(const QString & contents);
    QString getXML() const;
    Network * getNetwork() const { return network; }
    Requests getRequests() const { return requests; }
private:
    Computer * createComputer(const QDomElement & element);
    Store * createStore(const QDomElement & element);
    Switch * createSwitch(const QDomElement & element);
    Link * createLink(const QDomElement & element);
    Request * createRequest(const QDomElement & element);
    Network * createNetwork(const QDomElement & element);
private:
    QDomDocument document;
    Network * network;
    Requests requests;
    QMap<Element *, QDomElement> elementsXML;
    QMap<Request *, QDomElement> requestsXML;
};
