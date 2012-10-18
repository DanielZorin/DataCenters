#include "algorithm.h"
#include "network.h"
#include "request.h"

#include <set>
#include <string>

int main(int argc, char ** argv){
    Network * network = new Network();
    Request * r1 = new Request();
    Request * r2 = new Request();
    Algorithm::Requests requests;
    requests.insert(r1);
    requests.insert(r2);
    Algorithm::ResultEnum::Result result = Algorithm::ResultEnum::SUCCESS; 
     
    return 0;
}
