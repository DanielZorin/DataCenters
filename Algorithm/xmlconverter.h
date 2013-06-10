#ifndef XMLCONVERTER_H
#define XMLCONVERTER_H

#include "publicdefs.h"

#include <string>

#include <QtXml/QDomDocument>
#include <QtCore/QList>

class NetworkOverseer;
class RequestOverseer;

class XMLConverter {
public:
public:
    XMLConverter(QString &);
    ~XMLConverter();

    QString getMixdownContent();

    Requests getRequests();
    Network * getNetwork();

    void setAssignments(Assignments &);
private:
    void parseContents();
    void parseNetwork(QDomElement &);
    void parseRequests(QDomNodeList &);

    RequestOverseer * getOverseerByRequest(const Request *);

private:
    NetworkOverseer* networkOverseer;
    QList<RequestOverseer *> requestOverseers;
    QDomDocument document;
};

#endif // XMLCONVERTER_H
