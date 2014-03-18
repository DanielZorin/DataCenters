#pragma once

#include "defs.h"
#include "resourceType.h"

class Element {
    friend class ElementFactory;
    friend class Criteria;
public:
    enum Type {
        NONE            = 0x0,
        COMPUTER        = 0x1,
        STORE           = COMPUTER << 1,
        SWITCH          = STORE << 1,
        LINK            = SWITCH << 1,
        COMPUTATIONAL   = COMPUTER | STORE,
        NETWORK         = SWITCH | LINK,
        NODE            = COMPUTATIONAL | SWITCH
    };
protected:    

    virtual bool typeCheck(const Element * other) const = 0;

    virtual bool physicalCheck(const Element * other) const {
        ResourceVector::const_iterator it = resourceVector.begin();
        for ( ; it != resourceVector.end(); ++it ) {
            ResourceType* type = it->first;
            if ( other->resourceVector.find(type) != other->resourceVector.end() &&
                 !type->isAssignmentPossible(other->getResourceValue(type), it->second) )
                return false;
        }
        return true;
    }
    virtual bool attributeCheck(const Element * other) const {
        return ( attributes & other->attributes) == other->attributes;
    }

    virtual void decreaseResources(const Element * other) {
        ResourceVector::iterator it = resourceVector.begin();
        for ( ; it != resourceVector.end(); ++it ) {
            ResourceType* type = it->first;
            if ( other->resourceVector.find(type) != other->resourceVector.end() &&
                 !type->isCountable() )
                it->second -= other->getResourceValue(type);
        }
    }

    virtual void restoreResources(const Element * other)  {
        ResourceVector::iterator it = resourceVector.begin();
        for ( ; it != resourceVector.end(); ++it ) {
            ResourceType* type = it->first;
            if ( !type->isCountable() )
                it->second += other->getResourceValue(type);
        }
    }
    
    virtual unsigned long weight() const { return 0; }
public:
    Element() : type(NONE), physical(false),
        available(false), attributes(0), assignee(0) {}

    virtual ~Element() {}

    inline void setAvailable(bool available = true) {
        this->available = available; 
    } 
    
    inline bool canHostAssignment(const Element * other ) const {
        if ( isVirtual() ) return false;
        if ( !other->isVirtual() ) return false;

        if ( !isAvailable() ) return false;
        
        if ( !typeCheck(other) ) return false;
        if ( !attributeCheck(other) ) return false;
        if ( !physicalCheck(other) ) return false;

        return true;
    }

    inline unsigned getResourceValue(ResourceType* type) const {
        return resourceVector.at(type);
    }

    inline void setResourceValue(ResourceType* type, unsigned value) {
        resourceVector[type] = value;
    }

    Element * getAssignee() const {
        return assignee;
    }

    const Elements & getAssignments() const {
        return assignments; 
    }

    virtual bool assign(Element * other) {
        if ( !canHostAssignment(other) )
          return false;

        decreaseResources(other);
        
        other->assignee = this;
        assignments.insert(other);
        return true;
    }

    virtual void removeAssignment(Element * other) {
        Elements::iterator a = assignments.find(other);
        if ( a == assignments.end() )
            return;

        restoreResources(other);

        other->assignee = 0;
        assignments.erase(a);
    }

    virtual Elements adjacent() const = 0;

public:
    inline bool isAdjacent(const Element * other) const {
        Elements adj = adjacent();
        Element * f = const_cast<Element *>(other);
        return adj.find(f) != adj.end();
    }

    inline bool isComputer() const { 
        return type & COMPUTER; 
    }

    inline bool isStore() const {
        return type & STORE;
    }

    inline bool isSwitch() const { 
        return type & SWITCH;
    }

    inline bool isLink() const { 
        return type & LINK;
    }

    inline  bool isComputational() const {
        return type & COMPUTATIONAL;
    }

    inline  bool isNetwork() const {
        return type & NETWORK;
    }

    inline  bool isNode() const {
        return type & NODE;
    }

    inline  bool isEdge() const {
        return isLink();
    }

    inline  bool isPhysical() const {
        return physical;
    }

    inline  bool isVirtual() const {
        return !physical;
    }

    inline  bool isAvailable() const {
        return isPhysical() && available;
    }

    inline  bool isAssigned() const {
        return isVirtual() && assignee != 0;
    }

    inline Node * toNode() const {
        if ( !isNode() )
           return 0;
        return (Node *)this; 
    }

    inline Computer * toComputer() const {
        if ( !isComputer() )
           return 0;
        return (Computer *)this; 
    }

    inline Store * toStore() const {
        if ( !isStore() )
            return 0;
        return (Store *)this;
    }

    inline Switch * toSwitch() const {
        if ( !isSwitch() )
            return 0;
        return (Switch *)this;
    }

    inline Edge * toEdge() const {
        if ( !isEdge() )
            return 0;
        return (Edge *)this;
    }

    inline Link * toLink() const {
        if ( !isLink() )
           return 0;
        return (Link *)this;
    }


protected:
    Type type;
    bool physical;
    bool available;
    int attributes;
    Element * assignee;
    Elements assignments;

    // Resource values of element
    ResourceVector resourceVector;
};
