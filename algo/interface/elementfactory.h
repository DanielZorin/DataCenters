#pragma once

#include <defs.h>

#include <QMap>
#include <QString>
#include <QVariant>

class ElementFactory
{
public:
    static Element * populate(Element * element, const QVariant & properties);
};
