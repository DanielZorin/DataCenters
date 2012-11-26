#ifndef XMLCONVERTER_H
#define XMLCONVERTER_H

#include "publicdefs.h"

#include <string>

#include <QDomDocument>
#include <QList>

class NetworkOverseer;
class RequestOverseer;

class XMLConverter {
public:
public:
    XMLConverter(std::string const& );
    ~XMLConverter();

    Requests getRequests();
    Network * getNetwork();
private:

    void parseContents();
    void parseNetwork(QDomElement &);
    void parseRequests(QDomNodeList &);

private:

    NetworkOverseer* networkOverseer;
    QList<RequestOverseer *> requestOverseers;
    QDomDocument document;
};

#endif // XMLCONVERTER_H
