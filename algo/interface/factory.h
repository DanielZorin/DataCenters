#pragma once

#include "defs.h"

#include <QString>

class Factory
{
public:
    Factory();
    virtual ~Factory();
    virtual const Element * getElementById(uint id) const = 0;
    Network * getNetwork() const { return network; }
    const Requests & getRequests() const { return requests; }
    virtual QString getResult() const = 0;
protected:
    Network * network;
    Requests requests;
};
