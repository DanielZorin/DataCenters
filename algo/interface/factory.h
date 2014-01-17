#pragma once

#include "defs.h"

#include <QString>

class Factory
{
public:
    Factory();
    virtual ~Factory();
    Network * getNetwork() const { return network; }
    const Requests & getRequests() const { return requests; }
    virtual QString getResult() const = 0;
protected:
    Network * network;
    Requests requests;
};
