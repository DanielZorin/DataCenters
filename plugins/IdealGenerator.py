import random
from Core.Resources import ResourceGraph, Computer, Storage, Router
from Core.Demands import *

class IdealGenerator:
    storagePercent = 50
    compPercent = 50
    number = 10

    def __init__(self):
        pass

    @staticmethod
    def GetName():
        return "Ideal"

    def Generate(self, resources):
        medianStorage = self.storagePercent / self.number
        medianComp = self.compPercent / self.number
        comps = []
        storages = []
        for v in resources.vertices:
            if isinstance(v, Computer):
                comps += [v.speed]
            elif isinstance(v, Storage):
                storages += [v.volume]
        requests = []
        sumSt = 0
        sumComp = 0
        compTotal = int(sum(comps) * float(self.compPercent) / 100.0)
        stTotal = int(sum(storages) * float(self.storagePercent) / 100.0)
        while (sumSt < stTotal) and (sumComp < compTotal):
            st = int(max([0, random.gauss(medianStorage, 2)]) * sum(storages) / 100.0)
            comp = int(max([0, random.gauss(medianComp, 2)]) * sum(comps) / 100.0)
            sumSt += st
            sumComp += comp
            requests += [[st, comp]]
        res = []
        for r in requests:
            d = Demand("demand_" + str(r[0]) + "_" + str(r[1]))
            sumSt = 0
            while sumSt < r[0]:
                st = min([max(storages), random.randint(1, r[0] / 3 + 1)])
                for i in range(len(storages)):
                    if storages[i] > st:
                        storages[i] -= st
                        break
                d.AddVertex(DemandStorage("storage", st, 0, 9000))
                sumSt += st
            sumComp = 0
            while sumComp < r[1]:
                st = min([max(comps), random.randint(1, r[1] / 3 + 1)])
                for i in range(len(comps)):
                    if comps[i] > st:
                        comps[i] -= st
                        break
                d.AddVertex(VM("vm", st))
                sumComp += st
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
    return IdealGenerator