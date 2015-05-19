#pragma once

#include "defs.h"
#include "parameter.h"
#include "interface/export.h"

class Element {
    friend class ElementFactory;
    friend class Factory;
    friend class TenantXMLFactory;
    friend class ResourcesXMLFactory;
    friend class Criteria;
    //
    friend class Transmission;
    //
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
        Parameters::const_iterator it = parameters.begin();
        for ( ; it != parameters.end(); ++it ) {
            Parameter* type = it->first;
            ParameterValue* value = it->second;
            if ( other->parameters.find(type) == other->parameters.end() )
               return false;
            if ( !value->compare(other->parameters.at(type)) )
                return false;
        }
        return true;
    }

    virtual bool attributeCheck(const Element * other) const {
        return ( attributes & other->attributes) == other->attributes;
    }

    virtual void decreaseResources(const Element * other) {
        Parameters::iterator it = parameters.begin();
        for ( ; it != parameters.end(); ++it ) {
            Parameter* type = it->first;
            ParameterValue* value = it->second;
            if (other->parameters.find(type) != other->parameters.end()) {
                value->decrease(other->parameters.at(type));
            }
        }
    }

    virtual void restoreResources(const Element * other)  {
        Parameters::iterator it = parameters.begin();
        for ( ; it != parameters.end(); ++it ) {
            Parameter* type = it->first;
            ParameterValue* value = it->second;
            if (other->parameters.find(type) != other->parameters.end()) {
                value->increase(other->parameters.at(type));
            }
        }
    }
    
    virtual double weight() const { 
        double result = 0.0;
        for (Parameters::const_iterator it = parameters.begin(); it != parameters.end(); it ++) {
            ParameterValue * value = it->second;
            Parameter* par = it->first;
            result += value->weight() / EF::getMaxValue(par) * EF::getDeficit(par);
        }

        return result;
    }
public:
    Element() : type(NONE), physical(false),
        available(true), attributes(0), assignee(0) {}

    virtual ~Element() {
    	for ( Parameters::iterator it = parameters.begin(); it != parameters.end(); ++it ) {
    		delete it->second;
    	}
    }

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

    inline ParameterValue* getParameterValue(Parameter* type) const {
        return parameters.at(type);
    }

    inline void setParameterValue(Parameter* type, ParameterValue* value) {
    	parameters[type] = value;
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
        
        other->unassign();
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

    virtual void unassign() {
        if ( !isAssigned() )
           return;

        assignee->removeAssignment(this); 
    }

    virtual Elements adjacent() const = 0;
    virtual Elements adjacentNodes() const = 0;
    virtual Elements adjacentEdges() const = 0;


public:
    inline bool isAdjacent(const Element * other) const {
        Elements adj = adjacent();
        Element * f = const_cast<Element *>(other);
        return adj.find(f) != adj.end();
    }

    // Vnf is in fact virtual machine
    virtual bool isVnf() const {
    	return false;
    }

    virtual bool isRouter() const {
        return false;   
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

    virtual bool isAssigned() const {
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
    Parameters parameters;
};
