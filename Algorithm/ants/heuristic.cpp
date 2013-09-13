#include "heuristic.h"
#include <iostream>

// param1 = current physical resources
// param2 = required resources
// param3 = maximum physical resources
double MoreResourceFirst::calculate(unsigned int param1, unsigned int param2, unsigned int param3)
{
    double res = (double)(param1 - param2);
    if ((res > 0 && res < 0.01) || ZERO(res)) res = 0.01;
    if (res > param3 || res < 0) res = 0;
    if (res > maxValue) maxValue = res;
    return res;
}

double LargeRequestFirst::calculate(unsigned int param1, unsigned int param2, unsigned int param3)
{
    return (double)(param1)/param2;
}

