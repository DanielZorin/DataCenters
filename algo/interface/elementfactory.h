#pragma once

#include <defs.h>

#include <QMap>
#include <QString>
#include <QVariant>

class ElementFactory
{
public:
    static void debugPrint(Element * element);
    static Element * populate(Element * element, const QVariant & properties);
    static Parameters parametersFromProperties(const QMap<QString, QVariant> &);
};
