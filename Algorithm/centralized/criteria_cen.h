#ifndef CRITERIA_H
#define CRITERIA_H

#include "publicdefs.h"

class CriteriaCen
{
public:
   static unsigned long weight(Element *);
   static unsigned long weight(Request *);
   static unsigned long computationalCount(Request *);
};

#endif // CRITERIA_H
