#pragma once

#include <defs.h>

#include <QMap>
#include <QString>
#include <QVariant>

class ElementFactory
{
public:
    static Computer * populate(Computer * element, const QVariant & properties);
    static Store * populate(Store * element, const QVariant & properties);
    static Switch * populate(Switch * element, const QVariant & properties);
    static Link * populate(Link * element, const QVariant & properties);
    static Element * populate(Element * element, const QVariant & properties);
};
