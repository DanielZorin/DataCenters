#ifndef XMLCONVERTER_H
#define XMLCONVERTER_H

#include "publicdefs.h"

#include <string>
using std::string;

class XMLConverter {
private:
    Assignment * generateAssignment(string const& xml);
    Element * generateElement(string const& xml);
    Request * generateRequest(string const& xml);
    Network * generateNetwork(string const& xml);

    string generateSingleAssignmentXML(Assignment * assignment);
public:
    void ParseXML(string const& xml);
    string generateAssignmentsXML(Assignments assignments);
    Requests getRequests();
    Network * getNetwork();
private:
    Network * network;
    Requests requests;
    string currentXML;
};

#endif // XMLCONVERTER_H
