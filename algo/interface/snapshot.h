#pragma once

#include "defs.h"

#include <QList>
#include <QMap>

class ResourcesXMLFactory;
class TenantXMLFactory;
class QDomDocument;

class Snapshot {
public:
    Snapshot();
    ~Snapshot();

    bool read(const QString & filename);
    void write(const QString & filename) const;

    Network * getNetwork() const;
    Requests getRequests() const;

    void print();
private:
    void parseReverseAssignments(); 
private:
    QDomDocument * document;
    ResourcesXMLFactory * network;
    QList<TenantXMLFactory *> tenants;
    QMap<QString, QMap<QString, QString> > assignments;
};
