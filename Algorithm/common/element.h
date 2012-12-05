#ifndef ELEMENT_H
#define ELEMENT_H

/// common datacenter and request underlying model

#include <string>
using std::string;

class Element {
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
    Element(string elementName = "unnamed", unsigned long elementCapacity = 0, unsigned long max = 0)
    :   name(elementName),
        capacity(elementCapacity)
    {
        if (max == 0) maxCapacity = elementCapacity;
        else maxCapacity = max;
        setType(ELEMENT);
    }

    void setType(Type t) { type = t; }

    virtual ~Element() {}

public:
    virtual unsigned long getCapacity() const { return capacity; }
    virtual unsigned long getMaxCapacity() const { return maxCapacity; }
    virtual string getName() const { return name; }
    virtual bool isAssignmentPossible(Element const & other) const { return capacity >= other.capacity; }
    virtual void assign(Element const & other)
    { 
        if (isAssignmentPossible(other)) 
            capacity -= other.capacity;
    }
    void setCapacity(unsigned long c) { capacity = c; }
    void setMaxCapacity(unsigned long mc) { maxCapacity = mc; }

    // Remove the assignment, it is assumed that the other
    // element is assigned in this element.
    virtual void RemoveAssignment(Element const * other)
    {
        capacity += other->capacity;
    }

    // This function is to be called on all linked 
    // elements to notify of unlinkage of that element
    virtual void elementDestructionNotification(Element *) {}

public:
    inline bool isLink() { return type == LINK; }
    inline bool isNode() { return type == NODE; }
    inline bool isStore() { return type == STORE; }
    inline bool isSwitch() { return type == SWITCH; }
    inline bool isNetworking() { return isSwitch() || isLink(); }
    inline bool isComputational() { return isNode() || isStore(); }
private:
    unsigned long capacity;
    unsigned long maxCapacity;
    string name; 
protected:
    Type type;
};

#endif // ELEMENT_H
