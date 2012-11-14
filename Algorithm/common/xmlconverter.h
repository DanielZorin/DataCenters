#ifndef XMLCONVERTER_H
#define XMLCONVERTER_H

#include "publicdefs.h"

#include <string>

#include <QDomDocument>
#include <QMap>

class Element;

class XMLConverter {
public:
    XMLConverter(std::string const& );

    Requests getRequests();
    Network * getNetwork();
private:

    void parseContents();
    void parseNetwork(QDomElement &);

    void parseNodes(QDomNodeList &);
    void parseStores(QDomNodeList &);
    void parseSwitches(QDomNodeList &);
    void parseLinks(QDomNodeList &);

    void addNode(uint, Node *);
    void addStore(uint, Store *);
    void addSwitch(uint, Switch *);
    void addLink(uint, uint, Link *);

    Element * getElementByID(uint id);

    void parseRequests(QDomNodeList &);

private:
    QMap<uint, Node *> nodes;
    QMap<uint, Store *> stores;
    QMap<uint, Switch *> switches;

    Network * network;
    Requests requests;

    QDomDocument document;
};

#endif // XMLCONVERTER_H
