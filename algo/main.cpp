#include <stdio.h>
#include <vector>
#include "interface/elementfactory.h"
#include "interface/tenantxmlfactory.h"
#include "interface/resourcesxmlfactory.h"
#include "test/testalgorithm.h"
#include "prototype/prototype.h"

#include <QString>
#include <QFile>
#include <QTextStream>

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomNodeList>

#include "defs.h"
#include "request.h"
#include "network.h"
#include "operation.h"
#include "criteria.h"

enum ReturnCodes {
   SUCCESS = 0,
   PARTIAL_FAILURE,
   INVALID_INPUT,
   INVALID_USAGE
};

int main(int argc, char ** argv)
{
    if ( argc != 3 )
    {
        printf("Usage: %s <input file> <output file>\n", *argv);
        return INVALID_USAGE;
    }

    QFile input(argv[1]);

    input.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream inputStream(&input);

    QDomDocument document("XmlDocument");
    QString errMessage;
    int errorLine, errorColumn;
    if ( !document.setContent(inputStream.readAll(), false, &errMessage, &errorLine, &errorColumn)) {
        printf("XML parsing error, reason: %s at line %d, column %d\n", errMessage.toStdString().c_str(), errorLine, errorColumn);
        return INVALID_INPUT;  
    }

    QDomElement root = document.documentElement();

    QDomElement resources = root.elementsByTagName("resources").item(0).toElement();
    ResourcesXMLFactory resourcesFactory = ResourcesXMLFactory(resources);
    Network * network= resourcesFactory.getNetwork();

    QDomNodeList tenants = root.elementsByTagName("tenant");
    Requests requests;

    std::vector<TenantXMLFactory *> tenantsFactory;
    std::vector<TenantXMLFactory *> clientFactory;
    // Parsing client tenants first
    for ( int i = 0; i < tenants.size(); ++i ) {
    	if ( !TenantXMLFactory::isProviderTenant(tenants.item(i).toElement()) ) {
			TenantXMLFactory* sampleFactory = new TenantXMLFactory(tenants.item(i).toElement());
			tenantsFactory.push_back(sampleFactory);
			clientFactory.push_back(sampleFactory);
			Request * request = sampleFactory->getRequest();
			requests.insert(request);
    	}
    }

    // Parsing provider tenants
    for ( int i = 0; i < tenants.size(); ++i ) {
        if ( TenantXMLFactory::isProviderTenant(tenants.item(i).toElement()) ) {
            TenantXMLFactory* sampleFactory = new TenantXMLFactory(tenants.item(i).toElement());
            tenantsFactory.push_back(sampleFactory);
            Request * request = sampleFactory->getRequest();
            requests.insert(request);

            // Parsing external links
            std::vector<TenantXMLFactory *>::iterator it = clientFactory.begin();
            for ( ; it != clientFactory.end(); ++it ) {
                QString name = QString::fromUtf8((*it)->getRequest()->getName().c_str());
                sampleFactory->parseExternalPorts(name, (*it)->getPorts());
            }
        }
    }

    PrototypeAlgorithm algorithm(network, requests);
    algorithm.schedule(); 

    int assignedRequests = 0;
    int nodeAssignedRequests = 0;
    for ( Requests::iterator i = requests.begin(); i != requests.end(); i++ ) {
        Request * r = *i;
        if ( r->isAssigned() )
            assignedRequests++;    
        Elements unassignedComputational = Operation::filter(r->elementsToAssign(), Criteria::isComputational);
        if ( unassignedComputational.empty() )
            nodeAssignedRequests++;
    }


    for ( std::vector<TenantXMLFactory *>::iterator it = tenantsFactory.begin(); it != tenantsFactory.end(); ++it ) {
        TenantXMLFactory * tenantXML = *it;
        tenantXML->commitPartialAssignmentData(resourcesFactory);
        delete tenantXML;
    }

    QFile output(argv[2]);
    output.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream outStream(&output);
    outStream << document.toString(4);

    if ( nodeAssignedRequests != requests.size())
       return PARTIAL_FAILURE;

    return SUCCESS;

}
