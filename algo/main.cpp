#include <stdio.h>
#include "interface/xmlfactory.h"
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

    QFile input(argv[1]);
    input.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream inputStream(&input);
    Factory * factory = new XMLFactory(inputStream.readAll());
    Network * network = factory->getNetwork();
    Algorithm * algorithm = new TestAlgorithm(network, factory->getRequests());
    algorithm->schedule();
    delete algorithm;
    delete factory;

    return 0;
}
