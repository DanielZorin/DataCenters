#ifndef XMLCONVERTER_H
#define XMLCONVERTER_H

#include <string>
using std::string;

class Assignment;
class Element;
class Request;

class XMLConverter {
public:
    static Assignment * GenerateAssignment(string const& xml);
    static Element * GenerateElement(string const& xml);
    static Request * GenerateRequest(string const& xml);

    static string GenerateAssignmentXML(Assignment const* assignment);
};

#endif // XMLCONVERTER_H
