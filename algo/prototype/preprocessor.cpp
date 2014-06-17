#include "preprocessor.h"

#include "request.h"
#include "switch.h"
#include "link.h"
#include "operation.h"
#include "criteria.h"

#include "stdio.h"

Request * Preprocessor::fakeNetElements(Request * r) {
    Request * fakeRequest = new Request(*r);
    Elements netElements = Operation::filter(r->getElements(), Criteria::isSwitch);
    for ( Elements::iterator i = netElements.begin(); i != netElements.end(); i++ ) {
        Switch * netElement = (*i)->toSwitch();
        Elements adjacentElements = netElement->adjacentNodes();
        while( !adjacentElements.empty() ) {
            Element * pivot = *(adjacentElements.begin());
            adjacentElements.erase(pivot); 
            for (Elements::iterator a = adjacentElements.begin(); a != adjacentElements.end(); a++) {
                Element * anchor = *a;
                Link * fake = getFakeLink(pivot, anchor);
                fakeRequest->addExternalLink(fake);
            }
        } 

        Elements adjacentEdges = netElement->adjacentEdges();
        for (Elements::iterator e = adjacentEdges.begin(); e != adjacentEdges.end(); e++) {
            Link * link = (*e)->toLink();
            Port * port = link->getFirst();
            port->getParentNode()->toNode()->removePort(port);
            port = link->getSecond();
            port->getParentNode()->toNode()->removePort(port); 
            fakeRequest->omitElement(link);
        } 
        fakeRequest->omitElement(netElement);
    }
    printf("Faking request: %d elements against %d in the original\n", fakeRequest->size(), r->size());
    return fakeRequest;
}

Link * Preprocessor::getFakeLink(Element * first, Element * second) {
    Link * link = new Link();
    Port * port1 = new Port("dummy", first);
    Port * port2 = new Port("dummy", second);
    first->toNode()->addPort(port1);
    second->toNode()->addPort(port2);

    link->connect(port1, port2);
    port1->connect(link, port2);
    port2->connect(link, port1);
    return link;
}
