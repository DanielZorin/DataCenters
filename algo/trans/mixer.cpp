#include "mixer.h"

#include "network.h"

#include "interface/snapshot.h"

Mixer::Mixer() {}

bool Mixer::read(const Snapshot & from, const Snapshot & to) {
    network = from.getNetwork(); 
    return false;
}
