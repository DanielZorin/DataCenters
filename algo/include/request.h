#pragma once

#include "graph.h"
#include "element.h"
#include "operation.h"
#include "criteria.h"

class Request : public Graph {
public:
    Request(const Elements & e, std::string name = ""): name(name) {
        elements = Operation::filter(e, Criteria::isVirtual);
    }

    Request(const Request & other) { 
       name = other.name; 
       elements = other.elements;
    }

    const std:string & getName() const { return name; }

    inline Elements assignedElements() const {
        return Operation::filter(getElements(), Criteria::isAssigned);
    }

    inline Elements elementsToAssign() const {
        Elements assigned = assignedElements();
        return Operation::minus(getElements(), assigned);
    }

    inline Elements getMachines() const {
        return Operation::filter(getElements(), Criteria::isComputer);
    }

    inline Elements getStorages() const {
        return Operation::filter(getElements(), Criteria::isStore);
    }

    inline Elements getVSwitches() const {
        return Operation::filter(getElements(), Criteria::isSwitch);
    } 

    inline Elements getTunnels() const {
        return Operation::filter(getElements(), Criteria::isLink);
    }

    inline bool isAssigned() const {
        return elementsToAssign().empty(); 
    }

    inline void purgeAssignments() {
        Elements assigned = assignedElements();
        Operation::forEach(assigned, Operation::unassign);
    }

    inline std::string getName() const {
    	return name;
    }

    inline void addExternalLink(Element * link) {
        elements.insert(link); 
    }
    
    inline void omitElement(Element * element) {
        elements.erase(element);  
    }

private:

    std::string name;
};
