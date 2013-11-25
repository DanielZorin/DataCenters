#ifndef TESTROUTER_H
#define TESTROUTER_H

#include "router.h"

class TestRouter : public Router
{
public:
    TestRouter(Link * virtualLink, Network * net)
    :
        Router(virtualLink, net)
    {
        type = TEST;   
    }

    virtual bool route();
    virtual NetPath search() { return NetPath(); }
    virtual void print() const;
};

#endif //TESTROUTER_H
