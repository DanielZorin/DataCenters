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

int main(int argc, char ** argv)
{
    if ( argc != 3 )
    {
        printf("Usage: %s <input file> <output file>\n", *argv);
        return 1;
    }

    QFile input(argv[1]);

    input.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream inputStream(&input);

    QDomDocument document("XmlDocument");
    document.setContent(inputStream.readAll());

    QDomElement root = document.documentElement();

    QDomElement resources = root.elementsByTagName("resources").item(0).toElement();
    ResourcesXMLFactory resourcesFactory = ResourcesXMLFactory(resources);
    Network * network= resourcesFactory.getNetwork();
    if ( network != 0 ) {
    	printf("Network has %d elements\n", network->getElements().size());
    }

    QDomNodeList tenants = root.elementsByTagName("tenant");
    Requests requests;
    std::vector<TenantXMLFactory *> tenantsFactory;
    for ( int i = 0; i < tenants.size(); ++i ) {
    	TenantXMLFactory* sampleFactory = new TenantXMLFactory(tenants.item(i).toElement());
    	tenantsFactory.push_back(sampleFactory);
    	Request * request = sampleFactory->getRequest();
    	printf("Request has %d elements\n", request->getElements().size());
    	requests.insert(request);
    }

    PrototypeAlgorithm algorithm(network, requests);
    algorithm.schedule(); 

    int assignedRequests = 0;
    for ( Requests::iterator i = requests.begin(); i != requests.end(); i++ ) {
        Request * r = *i;
        const char * assignedStr = 0;
        if ( r->isAssigned() ) {
            assignedStr = "assigned";
            assignedRequests++;    
        } else {
            assignedStr = "not assigned";
        }
        printf("Request %p is %s\n", r, assignedStr);
        Elements elements = r->getElements();
        for (Elements::iterator i = elements.begin(); i!= elements.end(); i++) {
            printf("\t");
            ElementFactory::debugPrint(*i); 
        }
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

    return 0;
}
