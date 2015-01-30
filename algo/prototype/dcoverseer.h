#pragma once

#include "defs.h"
#include <map>

class DCOverseer {
public:
    DCOverseer(Network * network);
    int dcCount() const { return dcs.size(); }
    Elements dcPositionPool(int i) const;
    Elements dcPool(int dc) const;
private:
    std::map<int, Elements> dcs;
};
