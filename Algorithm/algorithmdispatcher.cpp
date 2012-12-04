#include "algorithmdispatcher.h"

#include "centralized/centralizedalgorithm.h"
#include "decentralized/decentralizedAlgorithm.h"
#include "ants/ant.h"

Algorithm * AlgorithmDispatcher::Dispatch(QString & type, Network * network, Requests requests)
{
    Algorithm * algorithm = 0;
    if ( type == QString("c") )
        algorithm = new CentralizedAlgorithm(network, requests);

    if ( type == QString("d") )
        algorithm = new DecentralizedAlgorithm(network, requests);

    return algorithm;
}
