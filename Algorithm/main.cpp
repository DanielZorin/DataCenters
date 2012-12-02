#include "xmlconverter.h"
#include "algorithmdispatcher.h"

#include "algorithm.h"

#include <iostream>
using std::cerr;
using std::endl;

#include <QtCore>

int main(int argc, char ** argv)
{
    if ( argc < 2 && argc > 4 )
        cerr << "Usage: " << argv[0] << " <input_file> [output_file = input_file] [algorithm = [a|c|d]]" << endl;

    QString inputName = QString(argv[1]);
    QString outputName = argc > 2 ? QString(argv[2]) : inputName;
    QString algorithmType = argc > 3 ? QString(argv[3]) : QString("d");
    
    QString input;
    {
        QFile inputFile(inputName);
        inputFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream inputStream(&inputFile);
        input = inputStream.readAll();
        inputFile.close();
    }

    XMLConverter converter(input);
    Network * network = converter.getNetwork();
    Requests requests = converter.getRequests();

    Algorithm * algorithm = AlgorithmDispatcher::Dispatch(algorithmType, network, requests); 
    algorithm->schedule();

    Assignments assignments = algorithm->getAssignments();
    converter.setAssignments(assignments);

    {
        QFile outputFile(outputName);
        outputFile.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream outputStream(&outputFile);
        outputStream << converter.getMixdownContent();
        outputFile.close();
    }
    
    
    delete algorithm;
}
