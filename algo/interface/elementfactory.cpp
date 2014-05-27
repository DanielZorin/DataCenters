#include "elementfactory.h"

#include "computer.h"
#include "store.h"
#include "switch.h"
#include "link.h"

#include "parameter.h"

#include <stdio.h>

QMap<QString, Parameter *> ElementFactory::parameters;

void ElementFactory::debugPrint(Element * element) {
    if ( element->isPhysical() )
        return;

    printf("Element %p of type %d assigned to element %p\n", 
            element, element->type, element->assignee);
}

Element * ElementFactory::populate(Element * element, const QMap<QString, ParameterValue *> & pr)
{
    element->physical = false;

    Parameters params = parametersFromProperties(pr);
    element->parameters = params;

    return element;
}

Parameters ElementFactory::parametersFromProperties(const QMap<QString, ParameterValue *> & pr) {
    Parameters result;
    foreach(QString name, pr.keys()) {
        Parameter * par = parameterByName(name);
        ParameterValue * val = pr[name];
        result[par] = val;
    }
    
    return result;
}

Parameter * ElementFactory::parameterByName(const std::string & name) {
    return parameterByName(QString::fromStdString(name));
}

Parameter * ElementFactory::parameterByName(const QString & name) {
    if ( parameters.contains(name) )
        return parameters[name];

    Parameter * parameter = new Parameter(name.toStdString());
    parameters.insert(name, parameter);
    return parameter;
}
