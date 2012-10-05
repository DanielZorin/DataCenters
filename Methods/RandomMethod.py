import random
from Core.Resources import Storage, Computer, Router, Link, Range, State
from Core.Demands import VM, DemandStorage, DemandLink

class RandomMethod:
    def __init__(self, resources, demands):
        self.resources = resources
        self.demands = demands

    def GetAvailableVertices(self,v,time):
        availableVertices = []
        for v1 in self.resources.vertices:
            if isinstance(v, VM) and isinstance(v1, Computer) and (v.speed <= v1.speed - v1.intervals[time].usedResource):
                availableVertices.append(v1)
            if isinstance(v, DemandStorage) and isinstance(v1, Storage) and (v.type == v1.type) and (v.volume <= v1.volume - v1.intervals[time].usedResource):
                availableVertices.append(v1)
        return availableVertices

    def GetAvailableLinks(self, e, time):
        availableLinks = []
        g = self.resources.FindPath(e.e1.resource, e.e2.resource)
        while True:
            try:
                p = g.next()
                if self.checkPath(p,e, time):
                    availableLinks.append(p)
            except StopIteration:
                return availableLinks

    def checkPath(self,path,link,time):
        for elem in path[1:len(path)-1]:
            if isinstance(elem, Router):
                if (link.capacity > elem.capacity - elem.intervals[time].usedResource):
                    return False
            else:
                e = self.resources.FindEdge(elem.e1, elem.e2)
                if (link.capacity > e.capacity - e.intervals[time].usedResource):
                    return False
        return True

    def AssignVertex(self, demand, vdemand, vresource, time):
        vdemand.resource = vresource
        if not vresource.intervals[time].demands.has_key(demand):
            vresource.intervals[time].demands[demand] = []
        vresource.intervals[time].demands[demand].append(vdemand)
        if isinstance(vdemand,VM):
            vresource.intervals[time].usedResource += vdemand.speed
        elif isinstance(vdemand,DemandStorage):
            vresource.intervals[time].usedResource += vdemand.volume

    def AssingLink(self, demand, link, path, time):
        link.path = path
        for elem in path[1:len(path)-1]:
            if isinstance(elem, Router):
                elem.intervals[time].usedResource += link.capacity
                if not elem.intervals[time].demands.has_key(demand):
                    elem.intervals[time].demands[demand] = []
                elem.intervals[time].demands[demand].append(link)
            else:
                e = self.resources.FindEdge(elem.e1, elem.e2)
                e.intervals[time].usedResource += link.capacity
                if not e.intervals[time].demands.has_key(demand):
                    e.intervals[time].demands[demand] = []
                e.intervals[time].demands[demand].append(link)

    def AssignDemand(self,demand):
        ranges = self.GetRanges(demand)
        iter = 0
        while True and (iter<1000): #FIXME
            iter+=1
            success = True
            for v in demand.vertices:
                lists = []
                for time in ranges:
                    lists.append(self.GetAvailableVertices(v,time))
                v1 = []
                for elem in lists[0]:
                    fl = True
                    for i in range(1,len(lists)-1):
                        if lists[i].count(elem) == 0:
                            fl = False
                            break
                    if fl:
                        v1.append(elem)
                if v1==[]:
                    self.DropDemand(demand)
                    success = False
                    break;
                i = random.randint(0, len(v1)-1)
                for time in ranges:
                    self.AssignVertex(demand,v,v1[i],time)
            if not success:
                continue
            for e in demand.edges:
                if e.e1.resource == e.e2.resource:
                    continue
                lists = []
                for time in ranges:
                    lists.append(self.GetAvailableLinks(e,time))
                e1 = []
                for elem in lists[0]:
                    fl = True
                    for i in range(1,len(lists)-1):
                        if lists[i].count(elem) == 0:
                            fl = False
                            break
                    if fl:
                        e1.append(elem)
                if e1==[]:
                    self.DropDemand(demand)
                    success=False
                    break
                i = random.randint(0, len(e1)-1)
                for time in ranges:
                    self.AssingLink(demand,e,e1[i],time)
            if success:
                break
        if iter == 1000:
            print "Failed to assign demand " + demand.id
            return False
        return True

    def DropVertex(self,demand,v):
        if v.resource == None:
            return
        for time in self.GetRanges(demand):
            if isinstance(v,VM):
                v.resource.intervals[time].usedResource -= v.speed
            elif isinstance(v,DemandStorage):
                v.resource.intervals[time].usedResource -= v.volume
            v.resource.intervals[time].demands[demand].remove(v)
            if v.resource.intervals[time].demands[demand]==[]:
                del v.resource.intervals[time].demands[demand]
        v.resource = None

    def DropLink(self,demand,link):
        if link.path == []:
            return
        path = link.path
        for elem in path[1:len(path)-1]:
            for time in self.GetRanges(demand):
                if isinstance(elem, Router):
                    elem.intervals[time].usedResource -= link.capacity
                    elem.intervals[time].demands[demand].remove(link)
                    if elem.intervals[time].demands[demand]==[]:
                        del elem.intervals[time].demands[demand]
                else:
                    e = self.resources.FindEdge(elem.e1, elem.e2)
                    e.intervals[time].usedResource -= link.capacity
                    e.intervals[time].demands[demand].remove(link)
                    if e.intervals[time].demands[demand]==[]:
                        del e.intervals[time].demands[demand]
        link.path = []

    def DropDemand(self,demand):
        for v in demand.vertices:
            self.DropVertex(demand,v)
        for e in demand.edges:
            self.DropLink(demand,e)

    def Run(self):
        self.BuildIntervals()
        for d in self.demands:
            self.AssignDemand(d)
        '''for v1 in self.resources.vertices:
            for t in v1.intervals.keys():
                print str(t.t1) + " " + str(t.t2)
                print v1.intervals[t].usedResource
                for d in v1.intervals[t].demands:
                    for v in d.vertices:
                        print v.id'''


    def Clear(self):
        for d in self.demands:
            self.DropDemand(d)

    def BuildIntervals(self):
        l = []
        for d in self.demands:
            l.append(d.startTime)
            l.append(d.endTime)
        l = list(set(l))
        l.sort()
        for v in self.resources.vertices:
            v.intervals = {}
        for e in self.resources.edges:
            e.intervals = {}
        for i in range(len(l)-1):
            time = Range(l[i],l[i+1])
            for v in self.resources.vertices:
                v.intervals[time] = State()
            for e in self.resources.edges:
                e.intervals[time] = State()

    def GetRanges(self, demand):
        v = self.resources.vertices[0]
        ranges = []
        for t in v.intervals.keys():
            if (t.t1 >= demand.startTime) and (t.t2 <= demand.endTime):
                ranges.append(t)
        return ranges