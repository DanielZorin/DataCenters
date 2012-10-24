#include "../common/network.h"
#include "../common/request.h"
#include "ant.h"
#include "internalgraph.h"

int main(int argc, char *argv[])
{
    Network n;
    Algorithm::Requests r;
    AntAlgorithm alg(&n, r);
    alg.start();
    return 0;
}

