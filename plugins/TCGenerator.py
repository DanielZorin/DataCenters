import random
from Core.Resources import ResourceGraph, Computer, Storage, Router
from Core.Demands import *

class TCGenerator:
    storagePercent = 50
    compPercent = 50
    netPercent = 50
    number = 10
    max_x = 400

    def __init__(self):
        pass

    @staticmethod
    def GetName():
        return "Tightly coupled"

    def Generate(self, resources):
        medianStorage = self.storagePercent / self.number
        medianComp = self.compPercent / self.number
        medianNet = self.netPercent / self.number
        comps = {}
        storages = {}
        for v in resources.vertices:
            if isinstance(v, Computer):
                comps[v] = [v.speed]
            elif isinstance(v, Storage):
                storages[v] = [v.volume]
        for e in resources.edges:
            if isinstance(e.e1, Computer):
                comps[e.e1].append(e.capacity)
            elif isinstance(e.e1, Storage):
                storages[e.e1].append(e.capacity)
            elif isinstance(e.e2, Computer):
                comps[e.e2].append(e.capacity)
            elif isinstance(e.e2, Storage):
                storages[e.e2].append(e.capacity)
        requests = []
        usedSt = 0
        usedComp = 0
        usedNet = 0
        sumSt = sum([storages[v][0] for v in storages.keys()])
        sumComp = sum([comps[v][0] for v in comps.keys()])
        sumNet = sum([comps[v][1] for v in comps.keys()]) + \
            sum([storages[v][1] for v in storages.keys()])
        compTotal = int(sumComp * float(self.compPercent) / 100.0)
        stTotal = int(sumSt * float(self.storagePercent) / 100.0)
        netTotal = int(sumNet * float(self.netPercent) / 100.0)
        while (usedSt < stTotal) or (usedComp < compTotal) or (usedNet < netTotal):
            st = min([int(max([0, random.gauss(medianStorage, 2)]) * sumSt / 100.0), stTotal - usedSt])
            comp = min([int(max([0, random.gauss(medianComp, 2)]) * sumComp / 100.0), compTotal - usedComp])
            net = min([int(max([0, random.gauss(medianNet, 2)]) * sumNet / 100.0), netTotal - usedNet])
            usedSt += st
            usedComp += comp
            usedNet += net
            requests += [[st, comp, net]]
        requests[0][0] += stTotal - usedSt
        requests[0][1] += compTotal - usedComp
        requests[0][2] += netTotal - usedNet
        res = []
        for r in requests:
            cur_x = 15
            cur_y = 15
            d = Demand("demand_" + str(r[0]) + "_" + str(r[1]))
            total = 0
            st = 1
            while total < r[0]:
                maxst = max([storages[v][0] for v in storages.keys()])
                if maxst == 0:
                    maxst = 1
                if st == 1:
                    st = min([maxst, r[0] / 3 + 1, r[0] - total])
                else:
                    st -= 1
                for v in storages.keys():
                    if storages[v][0] >= st:
                        storages[v][0] -= st
                        elem = DemandStorage("storage", st, 0, 9000)
                        elem.x = cur_x
                        elem.y = cur_y
                        if cur_x > self.max_x:
                            cur_x = 15
                            cur_y += 30
                        else:
                            cur_x += 30
                        storages[v].append([elem, d])
                        d.AddVertex(elem)
                        total += st
                        st = 1
                        break
                
            total = 0
            st = 1
            while total < r[1]:
                maxcmp = max([comps[v][0] for v in comps.keys()])
                if maxcmp == 0:
                    maxcmp = 1
                if st == 1:
                    st = min([maxcmp, r[1] / 3 + 1, r[1] - total])
                else:
                    st -= 1
                for v in comps.keys():
                    if comps[v][0] >= st:
                        comps[v][0] -= st
                        elem = VM("vm", st)
                        elem.x = cur_x
                        elem.y = cur_y
                        if cur_x > self.max_x:
                            cur_x = 15
                            cur_y += 30
                        else:
                            cur_x += 30
                        comps[v].append([elem, d])
                        d.AddVertex(elem)
                        total += st
                        st = 1
                        break
                
            res.append(d)

        total = 0
        cmps = [v for v in comps.keys()]
        it = 0
        while total < netTotal:
            it += 1
            # TODO: Beedlowcode, but we have to ensure that the loop isn't endless
            # Theoretically, this procesude is not guaranteed to be finite
            if it > 9000:
                break
            r = random.randint(0, len(cmps) - 1)
            c = cmps[r]
            for elem in comps[c][2:]:
                demand = elem[1]
                vm = elem[0]
                for v in storages.keys():
                    sts = [t for t in storages[v][2:]]
                    random.shuffle(sts)		
                    for st in sts:
                        if st[1] == demand:
                            vs = st[0]
                            bandwidth = min(comps[c][1], storages[v][1]) / max(len(comps[c]) - 2, len(storages[v]) - 2)
                            demand.AddLink(DemandLink(vm, vs, bandwidth))
                            total += bandwidth
        return res

    def GetSettings(self):
        # importing here to allow using the class without Qt
        from PyQt4.QtCore import QObject
        class Translator(QObject):
            def __init__(self, parent):
                QObject.__init__(self)
                self.parent = parent
            def getTranslatedSettings(self):
                return [
                [self.tr("Request count"), self.parent.number],
                [self.tr("Storage"), self.parent.storagePercent],
                [self.tr("Computers"), self.parent.compPercent],
                [self.tr("Network"), self.parent.netPercent]
                        ]
        t = Translator(self)
        return t.getTranslatedSettings()

    def UpdateSettings(self, dict):
        # importing here to allow using the class without Qt
        self.number = dict[0][1]
        self.storagePercent = dict[1][1]
        self.compPercent = dict[2][1]
        self.netPercent = dict[3][1]

def pluginMain():
    return TCGenerator