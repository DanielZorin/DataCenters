import random, copy
from Core.Resources import Storage, Computer, Router, Link, State
from Core.Demands import VM, DemandStorage, DemandLink
from PyQt4.QtCore import QObject, pyqtSignal

def list_intersect(l1,l2):
    res = []
    for i in l1:
        for j in l2:
            if (i==j):
                res.append(i)
    return res

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
                v1 = list(reduce(lambda x, y: x & y, lists))
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
                e1 = reduce(list_intersect, lists)
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
            self.resources.RemoveIntervals(demand)
            return False
        return True   

    def Run(self):
        for d in self.demands:
            self.AssignDemand(d)