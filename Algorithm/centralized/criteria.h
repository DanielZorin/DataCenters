#ifndef CRITERIA_H
#define CRITERIA_H

#include "publicdefs.h"

class Criteria
{
public:
   static unsigned long weight(Element *);
   static unsigned long weight(Request *);
};

#endif // CRITERIA_H
