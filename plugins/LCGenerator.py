import random
from Core.Resources import ResourceGraph, Computer, Storage, Router
from Core.Demands import *

class LCGenerator:
    storagePercent = 50
    compPercent = 50
    number = 10

    def __init__(self):
        pass

    @staticmethod
    def GetName():
        return "Loosely coupled"

    def Generate(self, resources):
        medianStorage = self.storagePercent / self.number
        medianComp = self.compPercent / self.number

        comps = {}
        storages = {}
        for v in resources.vertices:
            if isinstance(v, Computer):
                comps[v] = [v.speed]
            elif isinstance(v, Storage):
                storages[v] = [v.volume]

        requests = []
        usedSt = 0
        usedComp = 0

        sumSt = sum([storages[v][0] for v in storages.keys()])
        sumComp = sum([comps[v][0] for v in comps.keys()])

        compTotal = int(sumComp * float(self.compPercent) / 100.0)
        stTotal = int(sumSt * float(self.storagePercent) / 100.0)

        while (usedSt < stTotal) or (usedComp < compTotal):
            st = min([int(max([0, random.gauss(medianStorage, 2)]) * sumSt / 100.0), stTotal - usedSt])
            comp = min([int(max([0, random.gauss(medianComp, 2)]) * sumComp / 100.0), compTotal - usedComp])
            usedSt += st
            usedComp += comp
            requests += [[st, comp]]
        requests[0][0] += stTotal - usedSt
        requests[0][1] += compTotal - usedComp

        res = []
        for r in requests:
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
                        comps[v].append([elem, d])
                        d.AddVertex(elem)
                        total += st
                        st = 1
                        break
            res.append(d)
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
                [self.tr("Computers"), self.parent.compPercent]
                        ]
        t = Translator(self)
        return t.getTranslatedSettings()

    def UpdateSettings(self, dict):
        # importing here to allow using the class without Qt
        self.number = dict[0][1]
        self.storagePercent = dict[1][1]
        self.compPercent = dict[2][1]

def pluginMain():
    return LCGenerator