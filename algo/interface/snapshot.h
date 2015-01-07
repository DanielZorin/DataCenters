#pragma once

#include "defs.h"

#include <QList>
#include <QMap>

class ResourcesXMLFactory;
class TenantXMLFactory;
class QDomDocument;

class Snapshot {
public:
    typedef QMap<QString, QMap<QString, QString> > Assignments;
public:
    Snapshot();
    ~Snapshot();

    bool read(const QString & filename);
    void write(const QString & filename) const;

    Network * getNetwork() const;
    Requests getRequests() const;
    Assignments parseReverseAssignments() const; 

    void print();

    Element * getNetworkElement(const QString & name) const;
    Element * getTenantElement(const QString & tenant, const QString & name) const;
private:
    void commit();
private:
    QDomDocument * document;
    ResourcesXMLFactory * network;
    QMap<QString, TenantXMLFactory *> tenants;
};
