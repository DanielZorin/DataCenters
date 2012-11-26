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
    std::string getMixdownContent();

    Requests getRequests();
    Network * getNetwork();

    void setAssignments(Assignments &);
private:
    void parseContents();
    void parseNetwork(QDomElement &);
    void parseRequests(QDomNodeList &);

    RequestOverseer * getOverseerByRequestName(QString &);

private:
    NetworkOverseer* networkOverseer;
    QList<RequestOverseer *> requestOverseers;
    QDomDocument document;
};

#endif // XMLCONVERTER_H
