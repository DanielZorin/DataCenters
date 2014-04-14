#include <stdio.h>
#include "interface/tenantxmlfactory.h"
#include "test/testalgorithm.h"

#include <QString>
#include <QFile>
#include <QTextStream>

int main(int argc, char ** argv)
{
    if ( argc != 2 )
    {
        printf("Usage: %s <input file>\n", *argv);
        return 1;
    }

    TenantXMLFactory sampleFactory = TenantXMLFactory(QDomElement());
    Request * request = sampleFactory.getRequest();


    return 0;
}
