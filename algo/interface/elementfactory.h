#pragma once

#include <defs.h>

#include <QMap>
#include <QString>
#include <QVariant>

#include <string>

class ElementFactory
{
public:
    static void debugPrint(Element * element);
    static Element * populate(Element * element, const QMap<QString, ParameterValue *> &);
    static Parameters parametersFromProperties(const QMap<QString, ParameterValue *> &);
    static Parameter * parameterByName(const std::string & name);
    static Parameter * parameterByName(const QString & name);
private:
    static QMap<QString, Parameter *> parameters;
};
