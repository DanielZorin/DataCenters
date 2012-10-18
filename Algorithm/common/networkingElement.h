#ifndef NETWORKINGELEMENT_H
#define NETWORKINGELEMENT_H

#include "element.h"

#include <string>
using std::string;

class NetworkingElement : public Element {
private:
   NetworkingElement();
protected:
   NetworkingElement(string name = "unnamed_networking_element"
      , unsigned long capacity = 0)
   : Element(name, capacity)
   {}
};

#endif // NETWORKINGELEMENT_H
