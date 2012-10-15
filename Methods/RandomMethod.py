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
            self.resources.RemoveIntervals(demand)
            return False
        return True   

    def Run(self):
        for d in self.demands:
            self.AssignDemand(d)

    def Run(self, d):
        self.AssignDemand(d)