#include "algorithmdispatcher.h"

#include "centralized/centralizedalgorithm.h"
#include "decentralized/decentralizedAlgorithm.h"
#include "ants/ant.h"
#include "firstfit/ffalgorithm.h"
#include "randomalg/randomalg.h"

Algorithm * AlgorithmDispatcher::Dispatch(QString & type, Network * network, Requests requests)
{
    if ( type == QString("c") )
        return new CentralizedAlgorithm(network, requests);

    if ( type == QString("d") )
        return new DecentralizedAlgorithm(network, requests);

    if ( type == QString("a") )
        return new AntAlgorithm(network, requests);

    if ( type == QString("f") )
        return new FirstFitAlgorithm(network, requests);

    if ( type == QString("r") )
        return new RandomAlgorithm(network, requests);

    return 0;
}
