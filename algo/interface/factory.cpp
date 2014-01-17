#include "factory.h"

#include "network.h"
#include "request.h"

Factory::Factory()
:
    network(0)
{

}

Factory::~Factory()
{
    for ( Requests::iterator i = requests.begin(); i != requests.end(); i++)
        delete *i;

    if ( network ) delete network;
}

