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
        capacity(elementCapacity),
        ID (-1)
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
    virtual long getID() const { return ID; }
    virtual bool isAssignmentPossible(Element const & other) const { return capacity >= other.capacity; }
    virtual void assign(Element const & other)
    {
        if (isAssignmentPossible(other))
            capacity -= other.capacity;
    }
    void setCapacity(unsigned long c) { capacity = c; }
    void setMaxCapacity(unsigned long mc) { maxCapacity = mc; }
    void setID(long num) { ID = num; }

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
    inline bool isLink() const { return type == LINK; }
    inline bool isNode() const { return type == NODE; }
    inline bool isStore() const { return type == STORE; }
    inline bool isSwitch() const { return type == SWITCH; }
    inline bool isNetworking() const { return isSwitch() || isLink(); }
    inline bool isComputational() const { return isNode() || isStore(); }
private:
    unsigned long capacity;
    unsigned long maxCapacity;
    string name;
    // ID used to uniquely identify an object in a set of objects of the same type.
    // Assigned only by network when the element is added
    // Used for Network::operator=, it's copy constructor and assign
    // -1 means that object hasn't been added to any set
    // FIXME: should it be assigned somewhere else?
    long ID;
protected:
    Type type;
};

#endif // ELEMENT_H
