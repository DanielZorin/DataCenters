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
};

#endif //TESTROUTER_H
