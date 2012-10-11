import random, copy
from Core.Resources import Storage, Computer, Router, Link, State
from Core.Demands import VM, DemandStorage, DemandLink
from PyQt4.QtCore import QObject, pyqtSignal

class RandomMethod(QObject):
    demand_assigned = pyqtSignal(str)

    def __init__(self, resources, demands):
        self.resources = resources
        self.demands = demands
        super(RandomMethod, self).__init__()

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

    def AssignDemand(self,demand):
        if demand.assigned:
            return
        self.resources.PrepareIntervals(demand)
        ranges = self.resources.GetRanges(demand)
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
                    self.resources.AssignVertex(demand,v,v1[i],time)
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
                    self.resources.AssignLink(demand, e, e1[i], time)
            if success:
                demand.assigned = True
                print "Successfully assigned demand ", demand.id
                self.demand_assigned.emit(demand.id)
                break
        if iter == 1000:
            print "Failed to assign demand " + demand.id
            self.UpdateIntervals(demand)
            return False
        return True

    def DropVertex(self,demand,v):
        if v.resource == None:
            return
        for time in self.resources.GetRanges(demand):
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
            for time in self.resources.GetRanges(demand):
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

    def GetCurrentTimePoints(self):
        l = set([])
        for d in self.demands:
            if d.assigned:
                l.add(d.startTime)
                l.add(d.endTime)
        l = list(l)
        l.sort()
        return l

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
        
