#include "task.h"
#include "element.h"

Task::Task(Element * t, Element * d)
:
    target(t),
    destination(d)
{
}

bool Task::isValid() const {
    if ( target == 0 || destination == 0 ) return false;
    if ( target->isPhysical() || destination->isVirtual() ) return false;
    return true;
}

bool Task::isComplete() const {
    if ( !isValid() ) return false;
    if ( !target->isAssigned() ) return false;
    const Element * source = target->getAssignee();
    if ( source != destination ) return false;
    return true;
}

bool Task::move(Element * newDestination) {
    if ( !isValid() ) return false;
    Element * source = target->getAssignee();
    if ( !newDestination->assign(target) ) return false;
    if ( source != 0 ) history.push_back(source);
    return true;
}
