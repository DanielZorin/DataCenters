#include "router.h"

#include "common/switch.h"
#include "common/link.h"
#include "common/network.h"

void Router::decrease()
{
    Switches & switches = network->getSwitches();
    for ( Switches::iterator i = switches.begin(); i != switches.end(); i++)
    {
        Switch * s = *i;
        if ( s->isAssignmentPossible(*link) )
            s->assign(*link);
        else
           omittedSwitches.insert(s); 
    }

    Links & links = network->getLinks();
    for ( Links::iterator i = links.begin(); i != links.end(); i++)
    {
        Link * l = *i;
        if ( omittedSwitches.find((Switch *)l->getFirst()) != omittedSwitches.end() ||
                omittedSwitches.find((Switch *)l->getSecond()) != omittedSwitches.end())
            omittedLinks.insert(l);
        else
            if ( l->isAssignmentPossible(*link) )
                l->assign(*link);
            else
                omittedLinks.insert(l);
    } 

    for ( Switches::iterator i = omittedSwitches.begin(); i != omittedSwitches.end(); i++ )
        switches.erase(*i);

    for ( Links::iterator i = omittedLinks.begin(); i != omittedLinks.end(); i++)
        links.erase(*i);
}

void Router::restore()
{
    Switches & switches = network->getSwitches();
    Links & links = network->getLinks();

    for ( Switches::iterator i = switches.begin(); i != switches.end(); i++ )
        (*i)->RemoveAssignment(link);

    for ( Links::iterator i = links.begin(); i != links.end(); i++)
        (*i)->RemoveAssignment(link);

    for ( Switches::iterator i = omittedSwitches.begin(); i != omittedSwitches.end(); i++)
        switches.insert(*i);

    for ( Links::iterator i = omittedLinks.begin(); i != omittedLinks.end(); i++)
        links.insert(*i);

}
