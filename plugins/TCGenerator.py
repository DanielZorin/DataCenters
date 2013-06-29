import random
from Core.Resources import ResourceGraph, Computer, Storage, Router
from Core.Demands import *

class TCGenerator:
    storagePercent = 50
    compPercent = 50
    netPercent = 50
    storageVar = 2
    compVar = 2
    netVar = 2
    number = 10
    coupling = 0.5
    replicationCapacityRatio = 0.1
    max_x = 380

    def __init__(self):
        pass

    @staticmethod
    def GetName():
        return "Tightly coupled"

    def Generate(self, resources):
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
        for i in range(self.number):
            medianStorage = float(stTotal - usedSt)/(self.number-i)
            medianComp = float(compTotal - usedComp)/(self.number-i)
            medianNet = float(netTotal - usedNet)/(self.number-i)
            if stTotal != usedSt:
                st = int(random.gauss(medianStorage, self.storageVar*medianStorage/100))
                while st<=0:
                    st = int(random.gauss(medianStorage, self.storageVar*medianStorage/100))
            else:
                st = 0
            st = min(st, stTotal - usedSt)
            usedSt += st
            if compTotal != usedComp:
                comp = int(random.gauss(medianComp, self.compVar*medianComp/100))
                while comp<=0:
                    comp = int(random.gauss(medianComp, self.compVar*medianComp/100))
            else:
                comp = 0
            comp = min(comp, compTotal - usedComp)
            usedComp += comp
            if netTotal != usedNet:
                net = int(random.gauss(medianNet, self.netVar*medianNet/100))
                while comp<=0:
                    net = int(random.gauss(medianNet, self.netVar*medianNet/100))
            else:
                net = 0
            net = min(net, netTotal - usedNet)
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

            strgs = [v for v in storages.keys()]
            while total < r[0]:
                maxst = max([storages[v][0] for v in storages.keys()])
                if maxst == 0:
                    maxst = 1
                if st == 1:
                    st = min([maxst, r[0] / 3 + 1, r[0] - total])
                    st = random.randint(1, st)
                else:
                    st -= 1
                random.shuffle(strgs)
                for v in strgs:
                    if storages[v][0] >= st:
                        storages[v][0] -= st
                        type = 1 if self.replication_allowed else 0
                        elem = DemandStorage("storage", st, type, 0)
                        elem.x = cur_x
                        elem.y = cur_y
                        if cur_x > self.max_x:
                            cur_x = 15
                            cur_y += 35
                        else:
                            cur_x += 35
                        storages[v].append([elem, d])
                        d.AddVertex(elem)
                        total += st
                        st = 1
                        break
                
            total = 0
            st = 1
            cmps = [v for v in comps.keys()]
            while total < r[1]:
                maxcmp = max([comps[v][0] for v in comps.keys()])
                if maxcmp == 0:
                    maxcmp = 1
                if st == 1:
                    st = min([maxcmp, r[1] / 3 + 1, r[1] - total])
                    st = random.randint(1, st)
                else:
                    st -= 1
                random.shuffle(cmps)
                for v in cmps:
                    if comps[v][0] >= st:
                        comps[v][0] -= st
                        elem = VM("vm", st)
                        elem.x = cur_x
                        elem.y = cur_y
                        if cur_x > self.max_x:
                            cur_x = 15
                            cur_y += 35
                        else:
                            cur_x += 35
                        comps[v].append([elem, d])
                        d.AddVertex(elem)
                        total += st
                        st = 1
                        break
                
            res.append(d)

        '''total = 0
        cmps = [v for v in comps.keys()]
        it = 0
        while total < netTotal/2:
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
                    if not len(sts):
                        continue
                    random.shuffle(sts)
                    n = random.randint(0,len(sts)-1)
                    for st in sts[:n]:
                        if st[1] == demand:
                            vs = st[0]
                            bandwidth = min(comps[c][1], storages[v][1]) / max(len(comps[c]) - 2, len(storages[v]) - 2)
                            demand.AddLink(DemandLink(vm, vs, bandwidth))
                            total += bandwidth'''

        #TODO: we can't guarantee that all requests can be assigned
        maxNet = min(max([comps[v][1] for v in comps.keys()]),
            max([storages[v][1] for v in storages.keys()]))
        linksNum = 0
        totalBandwidth = 0
        for r,d in zip(res,requests):
            numEdges = int(len(r.vertices)*self.coupling)
            bandwidth = min(int(d[2]/2/numEdges), maxNet/(self.coupling*2))
            dstrgs = [v for v in r.vertices if isinstance(v, DemandStorage)]
            vms = [v for v in r.vertices if isinstance(v, VM)]
            for i in range(numEdges):
                e = None
                iter = 0
                while not e:
                    st = random.choice(dstrgs)
                    edges = r.FindAllEdges(st)
                    if iter>1000:
                        bandwidth /= 2
                    #to distribute edges uniformly
                    if len(edges) > self.coupling*2+1:
                        iter+=1
                        continue
                    net = sum(e.capacity for e in edges)
                    if net+bandwidth>maxNet:
                        iter+=1
                        continue
                    vm = random.choice(vms)
                    edges = r.FindAllEdges(vm)
                    if len(edges) > self.coupling*2+1:
                        iter+=1
                        continue
                    net = sum(e.capacity for e in edges)
                    if net+bandwidth>maxNet:
                        iter+=1
                        continue
                    e = DemandLink(st, vm, bandwidth)
                    linksNum += 1
                    totalBandwidth += bandwidth
                    iter = 0
                r.AddLink(e)
        avgBandwidth = totalBandwidth/linksNum
        for d in res:
            for v in d.vertices:
                if isinstance(v, DemandStorage):
                    v.replicationCapacity = int(self.replicationCapacityRatio * avgBandwidth)
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
                [self.tr("Total Storage Load (%)"), self.parent.storagePercent],
                [self.tr("Total Computers Load (%)"), self.parent.compPercent],
                [self.tr("Total Network Load (%)"), self.parent.netPercent],
                [self.tr("Storage Variance"), self.parent.storageVar],
                [self.tr("Computers Variance"), self.parent.compVar],
                [self.tr("Network Variance"), self.parent.netVar],
                [self.tr("Coupling"), self.parent.coupling],
                [self.tr("Consist.link bandwidth/Virt.link bandwidth"), self.parent.replicationCapacityRatio]
                        ]
        t = Translator(self)
        return t.getTranslatedSettings()

    def UpdateSettings(self, dict):
        # importing here to allow using the class without Qt
        self.number = dict[0][1]
        self.storagePercent = dict[1][1]
        self.compPercent = dict[2][1]
        self.netPercent = dict[3][1]
        self.storageVar = dict[4][1]
        self.compVar = dict[5][1]
        self.netVar = dict[6][1]
        self.coupling = dict[7][1]
        self.replication_allowed = True if dict[9][1]=="True" else False
        self.replicationCapacityRatio = dict[8][1]

def pluginMain():
    return TCGenerator