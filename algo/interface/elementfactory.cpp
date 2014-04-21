#include "elementfactory.h"

#include "computer.h"
#include "store.h"
#include "switch.h"
#include "link.h"

#include "parameter.h"
#include "interface/tenantxmlfactory.h"

Element * ElementFactory::populate(Element * element, const QVariant & properties, 
        const TenantXMLFactory * factory)
{
    element->physical = false;

    Parameters params = parametersFromProperties(properties.toMap());
    element->parameters = params;

    return element;
}

Parameters ElementFactory::parametersFromProperties(const QMap<QString, QVariant> & pr) {
    Parameters result;
    foreach(QString name, pr.keys()) {
        QVariant value = pr[name];
        Parameter * par = new Parameter(name.toStdString());
        ParameterValue * val = 0;
        if ( value.type() == QVariant::Double )
            val = new ParameterReal(value.toFloat());
        else if ( value.type() == QVariant::Int )
            val = new ParameterInt(value.toInt());
        else
            val = new ParameterString(value.toString().toStdString());
        result[par] = val;
    }
    
    return result;
}
