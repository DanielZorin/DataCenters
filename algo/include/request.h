#pragma once

#include "graph.h"
#include "element.h"
#include "operation.h"
#include "criteria.h"

class Request : public Graph {
public:
    Request(const Elements & e) {
        elements = Operation::filter(e, Criteria::isVirtual);
    }

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
};
