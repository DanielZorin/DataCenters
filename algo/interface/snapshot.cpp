#include "snapshot.h"

#include "tenantxmlfactory.h"
#include "resourcesxmlfactory.h"

#include "request.h"
#include "network.h"

#include <QDomElement>
#include <QDomNodeList>

#include <QFile>
#include <QTextStream>

#include <QDebug>

Snapshot::Snapshot()
:
    network(0)
{
    document = new QDomDocument("XMLDocument");
}

Snapshot::~Snapshot()
{
    delete network;
    foreach(TenantXMLFactory * f, tenants)
        delete f;
    
    delete document;
}

bool Snapshot::read(const QString & filename)
{
    QFile input(filename);
    if ( !input.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        qDebug() << "Unable to open file" << filename;
        return false;
    }

    QTextStream inputStream(&input);

    QString eMessage;
    int eLine, eColumn;
    if ( !document->setContent(inputStream.readAll(), false, &eMessage, &eLine, &eColumn))
    {
        qDebug() << "XML parsing error, reason:" << eMessage
            << "at line" << eLine << ", column" << eColumn;
        return false;
    }

    QDomElement root = document->documentElement();
    QDomElement resources = root.elementsByTagName("resources").item(0).toElement();
    network = new ResourcesXMLFactory(resources);

    QDomNodeList ts = root.elementsByTagName("tenant");
    QList<TenantXMLFactory *> clients;
    for ( int i = 0; i < ts.size(); i++)
    {
        QDomElement e = ts.item(i).toElement();
        if ( TenantXMLFactory::isProviderTenant(e) )
            continue;
        clients.append(new TenantXMLFactory(e));
    }

    for ( int i = 0; i < ts.size(); i++ )
    {
        QDomElement e = ts.item(i).toElement();
        if ( !TenantXMLFactory::isProviderTenant(e) )
            continue;

        TenantXMLFactory * factory = new TenantXMLFactory(e);
        tenants.insert(factory->name(), factory);
        foreach(TenantXMLFactory * f, clients)
            factory->parseExternalPorts(f->name(), f->getPorts());
    }

    foreach(TenantXMLFactory * client, clients)
        tenants.insert(client->name(), client);
    commit();

    return true;
}

Element * Snapshot::getNetworkElement(const QString & name) const
{
    return network->getElement(name);
}

Element * Snapshot::getTenantElement(const QString & tenant, const QString & name) const
{
    return tenants[tenant]->getElement(name);
}

void Snapshot::commit() 
{
   foreach(TenantXMLFactory * f, tenants)
      f->readAssignmentData(*network);
}

void Snapshot::write(const QString & filename) const
{
    foreach(TenantXMLFactory * f, tenants)
        f->commitPartialAssignmentData(*network);

    QFile output(filename);
    if ( !output.open(QIODevice::WriteOnly | QIODevice::Text) )
        return;

    QTextStream outputStream(&output);
    outputStream << document->toString(4);
}

Network * Snapshot::getNetwork() const 
{
    return network->getNetwork();
}

Requests Snapshot::getRequests() const
{
    Requests result;
    foreach(TenantXMLFactory * f, tenants)
        result.insert(f->getRequest());
    return result;
}

void Snapshot::print() 
{
    Assignments assignments = parseReverseAssignments();
    qDebug() << "[SNAPSHOT] Current state:";
    foreach(QString tenant, assignments.keys())
    {
        qDebug() << "tenant" << tenant << ":";
        QMap<QString, QString> & a = assignments[tenant];
        foreach(QString element, a.keys())
            qDebug() << "\t" << element << "->" << a[element];
    }
}

Snapshot::Assignments Snapshot::parseReverseAssignments() const
{
    Assignments assignments;
    foreach(TenantXMLFactory * f, tenants)
    {
        assignments.insertMulti(f->name(), f->assignments());
    }
    return assignments;
}

