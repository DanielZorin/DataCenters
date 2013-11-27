#include "centralizedalgorithm.h"

#include "network.h"
#include "request.h"

int main(int argc, char ** argv)
{
    Network network;
    Request request;
    Requests requests;
    requests.insert(&request);

    Algorithm * algorithm = new CentralizedAlgorithm(&network, requests);
    delete algorithm;
}
