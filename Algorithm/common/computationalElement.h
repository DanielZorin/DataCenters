#ifndef COMPUTATIONALELEMENT_H
#define COMPUTATIONALELEMENT_H

#include "element.h"

#include <string>
using std::string;

class ComputationalElement : public Element {
private:
   ComputationalElement();
protected:
   ComputationalElement(string name = "unnamed_computational_element"
      , unsigned long capacity = 0)
   : Element(name, capacity)
   {}
};

#endif // COMPUTATIONALELEMENT_H
