#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "publicdefs.h"

class NetworkManager
{
public:
    NetworkManager(Links &, Switches &);
private:
    Links links;
    Switches switches;
};

#endif // NETWORKMANAGER_H
