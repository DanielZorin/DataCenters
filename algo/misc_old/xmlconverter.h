#ifndef XMLCONVERTER_H
#define XMLCONVERTER_H

#include "common/publicdefs.h"

#include <string>

#include <QtXml/QDomDocument>
#include <QtCore/QList>

class NetworkOverseer;
class RequestOverseer;
class TunnelOverseer;

class XMLConverter {
public:
    XMLConverter(QString &);
    ~XMLConverter();

    QString getMixdownContent();

    Requests getRequests();
    Network * getNetwork();
    Link * getTunnel();

    void setAssignments(Assignments &);
    void setTunnelAssignment(std::set<NetPath> & pathes);
private:
    void parseContents();
    void parseNetwork(QDomElement &);
    void parseRequests(QDomNodeList &);
    void parseTunnel(QDomElement &);

    RequestOverseer * getOverseerByRequest(const Request *);

private:
    NetworkOverseer* networkOverseer;
    QList<RequestOverseer *> requestOverseers;
    TunnelOverseer* tunnelOverseer;
    QDomDocument document;
};

#endif // XMLCONVERTER_H
