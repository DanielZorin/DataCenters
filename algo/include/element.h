#pragma once

#include "defs.h"

class Element {
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
    Element() : type(NONE), assignee(0) {}

    virtual bool typeCheck(const Element * other) const = 0;
    virtual bool physicalCheck(const Element * other) const = 0;
    virtual bool attributeCheck(const Element * other) const = 0;

    virtual void decreaseResources(const Element * other) = 0;
    virtual void restoreResources(const Element * other) = 0;
public:
    virtual ~Element() {}

    void setAvailable(bool available = true) {
        this->available = available; 
    } 
    
    bool canHostAssignment(const Element * other ) const {
        if ( Element::isVirtual(this) ) return false;
        if ( !Element::isVirtual(other) ) return false;

        if ( !Element::isAvailable(this) ) return false;
        
        if ( !typeCheck(other) ) return false;
        if ( !attributeCheck(other) ) return false;
        if ( !physicalCheck(other) ) return false;

        return true;
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
        other->assigned = true;
        assignments.insert(other);
        return true;
    }

    virtual void removeAssignment(Element * other) {
        Elements::iterator a = assignments.find(other);
        if ( a == assignments.end() )
            return;

        restoreResources(other);

        other->assignee = 0;
        other->assigned = false;
        assignments.erase(a);
    }

public:
    inline static bool isComputer(const Element * e) { 
        return e->type & COMPUTER; 
    }

    inline static bool isStore(const Element * e) {
        return e->type & STORE;
    }

    inline static bool isSwitch(const Element * e) { 
        return e->type & SWITCH;
    }

    inline static bool isLink(const Element * e) { 
        return e->type & LINK;
    }

    inline static bool isComputational(const Element * e) {
        return e->type & COMPUTATIONAL;
    }

    inline static bool isNetwork(const Element * e) {
        return e->type & NETWORK;
    }

    inline static bool isNode(const Element * e) {
        return e->type & NODE;
    }

    inline static bool isEdge(const Element * e) {
        return isLink(e);
    }

    inline static bool isPhysical(const Element * e) {
        return e->physical;
    }

    inline static bool isVirtual(const Element * e) {
        return !e->physical;
    }

    inline static bool isAvailable(const Element * e) {
        return isPhysical(e) && e->available;
    }

    inline static bool isAssigned(const Element * e) {
        return isVirtual(e) && e->assigned;
    }

    inline Node * toNode() const {
        if ( !isNode(this) )
           return 0;
        return (Node *)this; 
    }

    inline Computer * toComputer() const {
        if ( !isComputer(this) )
           return 0;
        return (Computer *)this; 
    }

    inline Store * toStore() const {
        if ( !isStore(this) )
            return 0;
        return (Store *)this;
    }

    inline Switch * toSwitch() const {
        if ( !isSwitch(this) )
            return 0;
        return (Switch *)this;
    }

    inline Edge * toEdge() const {
        if ( !isEdge(this) )
            return 0;
        return (Edge *)this;
    }

    inline Link * toLink() const {
        if ( !isLink(this) )
           return 0;
        return (Link *)this;
    }


protected:
    Type type;
    bool physical;
    bool available;
    bool assigned;
    Element * assignee;
    Elements assignments;
};
