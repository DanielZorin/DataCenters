import random, copy
from Core.Resources import Storage, Computer, Router, Link, State
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

    def checkPath(self, path, link, time):
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
        if not vresource.intervals[time].demands.has_key(demand.id):
            vresource.intervals[time].demands[demand.id] = []
        vresource.intervals[time].demands[demand.id].append(vdemand.number)
        if isinstance(vdemand,VM):
            vresource.intervals[time].usedResource += vdemand.speed
        elif isinstance(vdemand,DemandStorage):
            vresource.intervals[time].usedResource += vdemand.volume

    def AssignLink(self, demand, link, path, time):
        link.path = path
        for elem in path[1:len(path)-1]:
            if isinstance(elem, Router):
                elem.intervals[time].usedResource += link.capacity
                if not elem.intervals[time].demands.has_key(demand.id):
                    elem.intervals[time].demands[demand.id] = []
                elem.intervals[time].demands[demand.id].append((link.e1.number, link.e2.number))
            else:
                e = self.resources.FindEdge(elem.e1, elem.e2)
                e.intervals[time].usedResource += link.capacity
                if not e.intervals[time].demands.has_key(demand.id):
                    e.intervals[time].demands[demand.id] = []
                e.intervals[time].demands[demand.id].append((link.e1.number, link.e2.number))

    def AssignDemand(self,demand):
        if demand.assigned:
            return
        self.PrepareIntervals(demand)
        ranges = self.GetRanges(demand)
        iter = 0
        while True and (iter<1000): #FIXME
            iter+=1
            success = True
            for v in demand.vertices:
                lists = []
                for time in ranges:
                    lists.append(self.GetAvailableVertices(v, time))
                v1 = []
                if len(lists) > 0:
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
                if len(lists) > 0:
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
                    self.AssignLink(demand, e, e1[i], time)
            if success:
                demand.assigned = True
                print "Successfully assigned demand ", demand.id
                break
        if iter == 1000:
            print "Failed to assign demand " + demand.id
            self.UpdateIntervals(demand)
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
            v.resource.intervals[time].demands[demand.id].remove(v.number)
            if v.resource.intervals[time].demands[demand.id]==[]:
                del v.resource.intervals[time].demands[demand.id]
        v.resource = None

    def DropLink(self,demand,link):
        if link.path == []:
            return
        path = link.path
        for elem in path[1:len(path)-1]:
            for time in self.GetRanges(demand):
                if isinstance(elem, Router):
                    elem.intervals[time].usedResource -= link.capacity
                    elem.intervals[time].demands[demand.id].remove((link.e1.number,link.e2.number))
                    if elem.intervals[time].demands[demand.id]==[]:
                        del elem.intervals[time].demands[demand.id]
                else:
                    e = self.resources.FindEdge(elem.e1, elem.e2)
                    e.intervals[time].usedResource -= link.capacity
                    e.intervals[time].demands[demand.id].remove((link.e1.number,link.e2.number))
                    if e.intervals[time].demands[demand.id]==[]:
                        del e.intervals[time].demands[demand.id]
        link.path = []

    def DropDemand(self,demand):
        demand.assigned = False
        for v in demand.vertices:
            self.DropVertex(demand,v)
        for e in demand.edges:
            self.DropLink(demand,e)
        

    def Run(self):
        for d in self.demands:
            self.AssignDemand(d)

    def Clear(self):
        for d in self.demands:
            if d.assigned:
                self.DropDemand(d)
                self.UpdateIntervals(d)

    def GetRanges(self, demand):
        v = self.resources.vertices[0]
        ranges = []
        for t in v.intervals.keys():
            if (t[0] >= demand.startTime) and (t[1] <= demand.endTime):
                ranges.append(t)
        return ranges

    def GetCurrentTimePoints(self):
        l = []
        for d in self.demands:
            if d.assigned:
                l.append(d.startTime)
                l.append(d.endTime)
        l = list(set(l))
        l.sort()
        return l

    def AddTimePoint(self, intervals, point):
        points = []
        for k in intervals.keys():
            points.extend([k[0],k[1]])
        points = list(set(points))
        points.sort()        
        if points.count(point) != 0:
            return
        if point > max(points):
            intervals[(max(points),point)] = State()
            return
        if point < min(points):
            intervals[(point,min(points))] = State()
            return
        i = 1
        while points[i] < point:
            i+=1
        intervals[(points[i-1],point)] = copy.deepcopy(intervals[(points[i-1],points[i])])
        intervals[(point,points[i])] = copy.deepcopy(intervals[(points[i-1],points[i])])
        del intervals[(points[i-1],points[i])]

    def RemoveTimePoint(self, intervals, point):
        points = self.GetCurrentTimePoints()
        if points.count(point) != 0:
            return
        points = []
        for k in intervals.keys():
            points.extend([k[0],k[1]])
        points = list(set(points))
        points.remove(point)
        points.sort()
        if point > max(points):
            del intervals[(max(points),point)]
            return
        if point < min(points):
            del intervals[(point,min(points))]
            return
        i = 1
        while points[i] < point:
            i+=1
        intervals[(points[i-1],points[i])] = copy.deepcopy(intervals[(points[i-1],point)])
        del intervals[(points[i-1],point)]
        del intervals[(point,points[i])]

    def PrepareIntervals(self, demand):
        for v in self.resources.vertices:
            if v.intervals == {}:
                v.intervals[(demand.startTime,demand.endTime)] = State()
            else:
                self.AddTimePoint(v.intervals, demand.startTime)
                self.AddTimePoint(v.intervals, demand.endTime)
        for e in self.resources.edges:
            if e.intervals == {}:
                e.intervals[(demand.startTime,demand.endTime)] = State()
            else:
                self.AddTimePoint(e.intervals, demand.startTime)
                self.AddTimePoint(e.intervals, demand.endTime)

    def UpdateIntervals(self, demand):
        for v in self.resources.vertices:
            if len(v.intervals.keys())==1:
                del v.intervals[(demand.startTime,demand.endTime)]
            else:
                self.RemoveTimePoint(v.intervals, demand.startTime)
                self.RemoveTimePoint(v.intervals, demand.endTime)
        for e in self.resources.edges:
            if len(e.intervals.keys())==1:
                del e.intervals[(demand.startTime,demand.endTime)]
            else:
                self.RemoveTimePoint(e.intervals, demand.startTime)
                self.RemoveTimePoint(e.intervals, demand.endTime)
        