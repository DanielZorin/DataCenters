#ifndef ALGORITHMDISPATCHER_H
#define ALGORITHMDISPATCHER_H

#include "publicdefs.h"
#include "algorithm.h"
#include <QtCore>

class AlgorithmDispatcher
{
public:
    static Algorithm * Dispatch(QString &, Network *, Requests);
};

#endif // ALGORITHMDISPATCHER_H
