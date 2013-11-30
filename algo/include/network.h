#pragma once

#include "graph.h"
#include "element.h"
#include "operation.h"

class Network : public Graph {
public:
    Network(const Elements & e) {
        elements = Operation::filter(e, Element::isPhysical);
    }

    inline Elements availableElements() const {
        return Operation::filter(getElements(), Element::isAvailable);
    }

    inline Elements getComputers() const {
        return Operation::filter(getElements(), Element::isComputer);
    }

    inline Elements getStores() const {
        return Operation::filter(getElements(), Element::isStore);
    }

    inline Elements getSwitches() const {
        return Operation::filter(getElements(), Element::isSwitch);
    }

    inline Elements getLinks() const {
        return Operation::filter(getElements(), Element::isLink);
    }
};
