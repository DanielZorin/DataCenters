#ifndef ALGORITHMDISPATCHER_H
#define ALGORITHMDISPATCHER_H

#include "common/publicdefs.h"
#include "common/algorithm.h"
#include <QtCore/QString>

class AlgorithmDispatcher
{
public:
    static Algorithm * Dispatch(QString &, Network *, Requests);
};

#endif // ALGORITHMDISPATCHER_H
