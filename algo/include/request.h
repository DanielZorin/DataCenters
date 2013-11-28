#pragma once

#include "graph.h"

class Request : public Graph {
public:
    const Computers & getMachines() const {
        return machines;
    }

    const Stores & getStorages() const {
        return storages;
    }

    const Links & getTunnels() const {
        return tunnels;
    }

    virtual Nodes & getNodes() {
        if ( nodes.empty() ) {
            for (Computers::iterator i = machines.begin(); i != machines.end(); i++)
                nodes.insert(*i);
            for (Stores::iterator i = storages.begin(); i != storages.end(); i++)
                nodes.insert(*i);
        }
        return nodes;
    }

    virtual Edges & getEdges() {
        if ( edges.empty() )
            for (Links::iterator i = tunnels.begin(); i != tunnels.end(); i++)
                edges.insert(*i);
        return edges;
    }

    bool addMachine(Element * element) {
        if ( !element->isVirtual() )
            return false;

        if ( !element->isComputer() )
            return false;

        Computer * machine = element->toComputer();
        machines.insert(machine);
    }

    bool addStorage(Element * element) {
        if ( !element->isVirtual() )
            return false;

        if ( !element->isStore() )
            return false;

        Store * storage = element->toStore();
        storages.insert(storage);
    }

    bool addLink(Element * element) {
        if ( !element->isVirtual() )
            return false;

        if ( !element->isLink() )
            return false;

        Link * link = element->toLink();
        Nodes & nodes = getNodes();
        if ( nodes.find(link->getFirst()) == nodes.end() ) return false;
        if ( nodes.find(link->getSecond()) == nodes.end() ) return false;

        tunnels.insert(link);
    }

private:
    Computers machines;
    Stores storages;
    Links tunnels;
};
