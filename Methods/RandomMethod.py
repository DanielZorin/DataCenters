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
                    lists.append(self.resources.GetAvailableVertices(v, time))
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
                    self.resources.DropDemand(demand)
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
                    lists.append(self.resources.GetAvailableLinks(e,time))
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
                    self.resources.DropDemand(demand)
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
            self.RemoveIntervals(demand)
            return False
        return True   

    def Run(self):
        for d in self.demands:
            self.AssignDemand(d)

    def Clear(self):
        for d in self.demands:
            if d.assigned:
                self.resources.DropDemand(d)
                self.RemoveIntervals(d)

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

    def RemoveIntervals(self, demand):
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
        
