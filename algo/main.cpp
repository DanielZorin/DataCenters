#include <stdio.h>
#include <vector>
#include "interface/tenantxmlfactory.h"
#include "interface/resourcesxmlfactory.h"
#include "test/testalgorithm.h"

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
        printf("Usage: %s <input file> <outpup file>\n", *argv);
        return 1;
    }

    QFile input(argv[1]);

    input.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream inputStream(&input);

    QDomDocument document("XmlDocument");
    document.setContent(inputStream.readAll());

    QDomElement root = document.documentElement();
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

    QDomElement resources = root.elementsByTagName("resources").item(0).toElement();
    ResourcesXMLFactory resourcesFactory = ResourcesXMLFactory(resources);
    Network * network= resourcesFactory.getNetwork();
    if ( network != 0 ) {
    	printf("Network has %d elements\n", network->getElements().size());
    }

    // TODO: algorithm scheduling
    for ( std::vector<TenantXMLFactory *>::iterator it = tenantsFactory.begin(); it != tenantsFactory.end(); ++it ) {
    	if ( (*it)->getRequest()->isAssigned() ) {
    		(*it)->commitAssignmentData(resourcesFactory);
    	}
    	delete (*it);
    }

    QFile output(argv[2]);
    output.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream outStream(&output);
    outStream << document.toString(4);

    return 0;
}
