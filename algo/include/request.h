#pragma once

#include "graph.h"
#include "element.h"
#include "operation.h"

class Request : public Graph {
public:
    Request(const Elements & e) {
        elements = Operation::filter(e, Element::isVirtual);
    }
    inline Elements assignedElements() const {
        return Operation::filter(getElements(), Element::isAssigned);
    }

    inline Elements elementsToAssign() const {
        Elements assigned = assignedElements();
        return Operation::minus(getElements(), assigned);
    }

    inline Elements getMachines() const {
        return Operation::filter(getElements(), Element::isComputer);
    }

    inline Elements getStorages() const {
        return Operation::filter(getElements(), Element::isStore);
    }

    inline Elements getTunnels() const {
        return Operation::filter(getElements(), Element::isLink);
    }
};
