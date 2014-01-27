#include "elementfactory.h"

#include "computer.h"
#include "store.h"
#include "switch.h"
#include "link.h"

Computer * ElementFactory::populate(Computer * element, const QVariant & properties)
{
   return element;
}

Store * ElementFactory::populate(Store * element, const QVariant & properties)
{
   return element;
}

Switch * ElementFactory::populate(Switch * element, const QVariant & properties)
{
   return element;
}

Link * ElementFactory::populate(Link * element, const QVariant & properties)
{
   return element;
}

Element * ElementFactory::populate(Element * element, const QVariant & properties)
{
   const QMap<QString, QVariant> & p = properties.toMap();
   element->physical = p["physical"].toBool();
   element->available = p["available"].toBool();
   return element;
}
