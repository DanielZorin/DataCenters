#ifndef ELEMENT_H
#define ELEMENT_H

/// common datacenter and request underlying model

#include <string>
using std::string;

class Element {
private:
    Element();
public:
    enum Type {
        ELEMENT         = 0x00,
        COMPUTATIONAL   = 0x04,
        NETWORKING      = 0x08,
        NODE            = 0x01 | COMPUTATIONAL,
        STORE           = 0x02 | COMPUTATIONAL,
        LINK            = 0x01 | NETWORKING,
        SWITCH          = 0x02 | NETWORKING
    };
protected:
    Element(string elementName = "unnamed", unsigned long elementCapacity = 0)
    :   name(elementName),
        capacity(elementCapacity)
    {
        setType(ELEMENT);
    }

    void setType(Type t) { type = t; }

    virtual ~Element() {}

public:
    virtual unsigned long getCapacity() { return capacity; }
    virtual string getName() { return name; }
    virtual bool isAssignmentPossible(Element const & other) { return capacity >= other.capacity; }
    virtual void Assign(Element const & other)
    { 
        if (isAssignmentPossible(other)) 
            capacity -= other.capacity;
    }

private:
    unsigned long capacity;
    string name; 
protected:
    Type type;
};

#endif // ELEMENT_H
