#include "node.h"
Node::Node(string name, unsigned long capacity, unsigned long max)
: ComputationalElement(name, capacity, max),
    ramCapacity(0),
    maxRamCapacity(0)
{
    setType(Element::NODE);
}

bool Node::isAssignmentPossible(Element const & other) const
{
    if ( ! other.isNode() )
        return false;

    Element * element = const_cast<Element *>(&other);
    Node * node = static_cast<Node *>(element);
    return Element::isAssignmentPossible(other) && ramCapacity >= node->getRamCapacity();
}

void Node::assign(Element const & other)
{
    if (! isAssignmentPossible(other) )
        return;

    Element::assign(other);
    
    Element * element = const_cast<Element *>(&other);
    Node * node = static_cast<Node *>(element);
    ramCapacity -= node->getRamCapacity();
}

void Node::RemoveAssignment(Element const * other)
{
    Element::RemoveAssignment(other);

    Element * element = const_cast<Element *>(other);
    Node * node = static_cast<Node *> (element);
    ramCapacity += node->getRamCapacity();

}
