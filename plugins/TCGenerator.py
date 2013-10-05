import random
from Core.Resources import ResourceGraph, Computer, Storage, Router, Link
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
    assignInSegments = False
    numberOfSegments = 1
    
    def __init__(self):
        pass

    @staticmethod
    def GetName():
        return "Tightly coupled"

    def AddElemToSegment(self, segment, elem):
        number = elem.number
        segment.AddVertex(elem)
        elem.number = number
        
    # Given first lvl router, find all second lvl routers
    # and attach vertexes to segmetn
    def ParseFirstLvlRouter(self, resources, firstLvlRouter, segment, attachedResources):
        secondLvlRouters = []
        edgesOfRouter = resources.FindAllEdges(firstLvlRouter)
        for edge in edgesOfRouter:
            if not (edge in attachedResources):
                attachedResources.add(edge)
                segment.AddLink(edge)
                
                if edge.e1 != firstLvlRouter:
                    if isinstance(edge.e1, Router):
                        secondLvlRouters.append(edge.e1)
                    if not (edge.e1 in attachedResources): 
                        attachedResources.add(edge.e1)
                        self.AddElemToSegment(segment, edge.e1)
                else:
                    if isinstance(edge.e2, Router):
                        secondLvlRouters.append(edge.e2)
                    if not (edge.e2 in attachedResources): 
                        attachedResources.add(edge.e2)
                        self.AddElemToSegment(segment, edge.e2)
        return secondLvlRouters
    
    def CheckFirstLvl(self, resources, vToParse):
        edgesOfRouter = resources.FindAllEdges(vToParse)
        for edge in edgesOfRouter:
            if isinstance(edge.e1, Computer) or isinstance(edge.e2, Computer) or isinstance(edge.e1, Storage) or isinstance(edge.e2, Storage):
                return True
        return False
    
    # Given first lvl router, find all second lvl routers
    # and attach vertexes to segmetn
    def ParseSecondLvlRouter(self, resources, secondLvlRouter, segment, attachedResources):
        edgesOfRouter = resources.FindAllEdges(secondLvlRouter)
        for edge in edgesOfRouter:
            if ( edge in attachedResources ):
                continue
            
            vToParse = edge.e1 if edge.e1 != secondLvlRouter else edge.e2
            # first, check that the edge is going to the lower level
            if self.CheckFirstLvl(resources, vToParse) and not (vToParse in attachedResources): 
                attachedResources.add(vToParse)
                self.AddElemToSegment(segment, vToParse)
                attachedResources.add(edge)
                segment.AddLink(edge)
                self.ParseFirstLvlRouter(resources, vToParse, segment, attachedResources)
                    
                    
        
    # given one vm, select a segment containing this vm
    # Todo: generate several segments
    def FindSegment(self, resources, vm, attachedResources):
        segment = ResourceGraph()
        attachedResources.add(vm)
        self.AddElemToSegment(segment, vm)
        edgesOfVm = resources.FindAllEdges(vm)
        firstLvlRouter = None
        for edge in edgesOfVm:
            if ( isinstance(edge.e1, Router) ):
                firstLvlRouter = edge.e1
                attachedResources.add(edge)
                segment.AddLink(edge)
                break
            elif ( isinstance(edge.e2, Router) ):
                firstLvlRouter = edge.e2
                attachedResources.add(edge)
                segment.AddLink(edge)
                break
                
        if ( firstLvlRouter == None ):
            raise Exception("Can't find attached router!")
            
        self.AddElemToSegment(segment, firstLvlRouter)
        attachedResources.add(firstLvlRouter)
        
        secondLvlRouters = self.ParseFirstLvlRouter(resources, firstLvlRouter, segment, attachedResources)
        
        for secondLvlRouter in secondLvlRouters:
            self.ParseSecondLvlRouter(resources, secondLvlRouter, segment, attachedResources)
        
        return segment
   
    def RemoveResources(self, segment):
        segment.vertices = []
        segment.edges = []
    
    def Generate(self, resources, segmentGeneration = False):
        comps = {}
        storages = {}
        
        if not segmentGeneration and self.assignInSegments:
            # generating segments first
            res = []
            attachedResources = set()
            fullNumber = self.number
            self.number = int(self.number / self.numberOfSegments)
            for i in range(self.numberOfSegments):
                if i == (self.numberOfSegments - 1):
                    # last attempt, need to correct number of requests
                    self.number = fullNumber - self.number * (self.numberOfSegments - 1)
                for v in resources.vertices:
                    if isinstance(v, Computer) and not v in attachedResources:
                        segment = self.FindSegment(resources, v, attachedResources)
                        segment._buildPaths()
                        
                        demands = self.Generate(segment, True)
                        self.RemoveResources(segment)
                        for d in demands:
                            res.append(d)
                        break
            return res
        
                        
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

        # calculating max capacity of phys elements according to coupling
        maxNet = min(max([comps[v][1] for v in comps.keys()]),
            max([storages[v][1] for v in storages.keys()]))
        # 2 - both for sts and for vms
        maxElemsCount = netTotal / (self.coupling * maxNet) 
        capacityBoundSt = int(stTotal / maxElemsCount)
        capacityBoundComp = int(compTotal / maxElemsCount)
        
        res = []
        index = 0
        for r in requests:
            cur_x = 15
            cur_y = 15
            d = Demand("demand_" + str(index) + "_" + str(r[0]) + "_" + str(r[1]))
            total = 0
            st = 1
            index += 1

            strgs = [v for v in storages.keys()]
            while total < r[0]:
                maxst = max([storages[v][0] for v in storages.keys()])
                if maxst == 0:
                    maxst = 1
                if st == 1:
                    st = min([maxst, r[0] / 3 + 1, r[0] - total, capacityBoundSt])
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
                        resources.PrepareIntervals(d)
                        resources.AssignVertex(d, elem, v, (0, 1))
                        if v.intervals[(0, 1)].usedVolume > v.volume:
                            raise Exception("Used speed is more then available!")
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
                    st = min([maxcmp, r[1] / 3 + 1, r[1] - total, capacityBoundComp])
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
                        resources.PrepareIntervals(d)
                        resources.AssignVertex(d, elem, v, (0, 1))
                        if v.intervals[(0, 1)].usedSpeed > v.speed:
                            raise Exception("Used speed is more then available!")
                        total += st
                        st = 1
                        break
                
            res.append(d)

        total = 0
        cmps = [v for v in comps.keys()]
        it = 0
        '''while total < netTotal/2:
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
        
        linksNum = 0
        totalBandwidth = 0
        assignedLinks = []
        for s in storages.keys():
            storages[s].append(storages[s][1])
        for c in comps.keys():
            comps[c].append(comps[c][1])
        for r,d in zip(res,requests):
            numEdges = int(len(r.vertices)*self.coupling)
            bandwidth = min(int(d[2]/2/numEdges), maxNet/(self.coupling*2))
            dstrgs = [v for v in r.vertices if isinstance(v, DemandStorage)]
            vms = [v for v in r.vertices if isinstance(v, VM)]
            requestBandwidth = 0
            for i in range(numEdges):
            # Number of links may be more then expected by coupling,
            # but total bandwidth is garantied to be exactly as expected
            #while requestBandwidth < (d[2]/2): 
                e = None
                path = None
                iter = 0
                while not e and bandwidth > 0:
                    st = random.choice(dstrgs)
                    edges = r.FindAllEdges(st)
                    if iter>50:
                        iter = 0
                        bandwidth /= 2
                    #to distribute edges uniformly
                    if len(edges) > self.coupling*2+1:
                        #iter+=1
                        continue
                    net = sum(e.capacity for e in edges)
                    if net+bandwidth>maxNet:
                        #iter+=1
                        continue
                    vm = random.choice(vms)
                    edges = r.FindAllEdges(vm)
                    if len(edges) > self.coupling*2+1:
                        #iter+=1
                        continue
                    net = sum(e.capacity for e in edges)
                    if net+bandwidth>maxNet:
                        #iter+=1
                        continue
                    s1 = None
                    for s in storages.keys():
                        for elem in storages[s][2:-1]:
                            if elem[0] == st:
                                if storages[s][-1] >= net:
                                    s1 = s
                                    break
                        if s1:
                            break
                    c1 = None
                    for c in comps.keys():
                        for elem in comps[c][2:-1]:
                            if elem[0] == vm:
                                if comps[c][-1] >= net:
                                    c1 = c
                                    break
                        if c1:
                            break

                    if not s1 or not c1:
                        continue

                    iter+=1
                    

                    e = DemandLink(st, vm, bandwidth)
                    paths = resources.FindPath(st.resource, vm.resource)
#                    path = paths.next()
#                    while path != None and path != []:
                    path = paths.next()

                    if not resources.checkPath(path, e, (0, 1)):
                         e = None
                    else:
                         r.AddLink(e)
                         resources.AssignLink(r, e, path, (0, 1))
                         linksNum += 1
                         requestBandwidth += bandwidth
                         totalBandwidth += bandwidth
                         iter = 0
                         assignedLinks.append((e, r))
                         comps[c1][-1] -= net
                         storages[s1][-1] -= net

                #if bandwidth == 0:
                #    raise Exception("Failed to generate test")

        # "trying to increase bandwidth of already assigned links (if capacity is not enough"
        
        for assignedLinkDemand in assignedLinks:
            assignedLink = assignedLinkDemand[0]
            assignedDemand = assignedLinkDemand[1]
            if (2 * totalBandwidth) >= netTotal:
                break
            
            bandwidth = assignedLink.capacity
            path = assignedLink.path
            
            # calculating the bandwidth to increase
            link1 = path[1]
            bwth1 = link1.capacity - link1.intervals[(0, 1)].usedCapacity
            
            link2 = path[len(path)-2]
            bwth2 = link2.capacity - link2.intervals[(0, 1)].usedCapacity
            
            #print "band1 = " + str(bwth1) + "; band2 = " + str(bwth2)
            bwth = min(min(bwth1, bwth2), netTotal - 2 * totalBandwidth)
            
            if bwth > 0:
                elem1 = assignedLink.e1
                elem2 = assignedLink.e2
                resources.DropLink(assignedDemand, assignedLink)
                e = DemandLink(elem1, elem2, bandwidth + bwth)

                if not resources.checkPath(path, e, (0, 1)):
                     resources.AssignLink(assignedDemand, assignedLink, path, (0, 1))
                else:
                     assignedDemand.DeleteEdge(assignedLink)
                     assignedDemand.AddLink(e)
                     resources.AssignLink(assignedDemand, e, path, (0, 1))
                     totalBandwidth += bwth
        print "   Requests generated. Network load gained: " + str((float)(2*totalBandwidth) / (float)(sumNet) )

        avgBandwidth = totalBandwidth/linksNum
        for d in res:
            for v in d.vertices:
                if isinstance(v, DemandStorage):
                    sumLinksCapacity = 0
                    for link in d.FindAllEdges(v):
                        sumLinksCapacity += link.capacity
                    v.replicationCapacity = int(self.replicationCapacityRatio * sumLinksCapacity)

                    #v.replicationCapacity = int(self.replicationCapacityRatio * avgBandwidth)
                    
        # Reseting demands
        for d in res:
            try:
                resources.DropDemand(d)
                resources.RemoveIntervals(d)
            except:
                pass   

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
                [self.tr("Consist.link bandwidth/Virt.link bandwidth"), self.parent.replicationCapacityRatio],
                [self.tr("Number of segments to guarantee assignment"), self.parent.numberOfSegments]
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
        self.replication_allowed = True if dict[10][1]=="True" else False
        self.replicationCapacityRatio = dict[8][1]
        self.assignInSegments = True if dict[11][1]=="True" else False
        self.numberOfSegments = dict[9][1]

def pluginMain():
    return TCGenerator
