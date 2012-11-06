#ifndef COMPUTATIONALELEMENT_H
#define COMPUTATIONALELEMENT_H

#include "element.h"

#include <string>
using std::string;

class ComputationalElement : public Element {
protected:
    ComputationalElement(string name = "unnamed_computational_element"
        , unsigned long capacity = 0, unsigned long max = 0)
    : Element(name, capacity, max)
    {
        setType(Element::COMPUTATIONAL);
    }
};

#endif // COMPUTATIONALELEMENT_H
