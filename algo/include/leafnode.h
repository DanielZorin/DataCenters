#pragma once

#include "node.h"

class LeafNode : public Node {
protected:
    LeafNode() : Node() {
        dcLayer = 0;
        serverLayer = 0;
    }
public:
    static int maxLayer() { return sizeof(int) * 8; }

    void setDCLayer(int l) {
        if ( l <= 0 || l > maxLayer() )
            return;

        dcLayer = 1 << (l - 1);
    }

    void setServerLayer(int l) {
        if ( l <= 0 || l > maxLayer() )
            return;

        serverLayer = 1 << (l - 1);
    }

    inline int dl() const { return dcLayer; }
    
    inline int sl() const { 
        if ( isPhysical() )
            return 0;
        return serverLayer; 
    }
private:
    int dcLayer;
    int serverLayer;
};
