import random, copy
from Core.Resources import Storage, Computer, Router, Link, State
from Core.Demands import VM, DemandStorage, DemandLink, Replication
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
                if e.e1.resource == e.e2.resource or e.path != []:
                    continue
                lists = []
                for time in ranges:
                    lists.append(self.resources.GetAvailableLinks(e,time))
                e1 = reduce(list_intersect, lists)
                if e1==[]:
                    if not self.Replicate(e,demand,ranges):
                        self.resources.DropDemand(demand)
                        success=False
                        break
                    else:
                        continue
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

    def Replicate(self, link, demand, ranges):
        if isinstance(link.e1,VM) and isinstance(link.e2,VM) or isinstance(link.e1,DemandStorage) and isinstance(link.e2,DemandStorage):
            return False
        if isinstance(link.e1,DemandStorage):
            st = link.e1
            vm = link.e2
        else:
            st = link.e2
            vm = link.e1
        replica = DemandStorage(st.id+"_replica",st.volume,st.type)
        consistencyLink = DemandLink(st,replica,demand.replicationCapacity)
        link = DemandLink(vm,replica,link.capacity)
        r = Replication(replica, consistencyLink, link)
        demand.AddReplication(r)
        lists = []
        for time in ranges:
            lists.append(self.resources.GetAvailableVertices(replica, time))
        storages = list(reduce(lambda x, y: x & y, lists))
        if storages == []:
            demand.DeleteReplication(r)
            return False
        success = False
        for s in storages:
            for time in ranges:
                self.resources.AssignVertex(demand, replica, s, time)
            lists = []
            for time in ranges:
                lists.append(self.resources.GetAvailableLinks(link,time))
            available_links = reduce(list_intersect, lists)
            if available_links==[]:
                self.resources.DropVertex(demand, replica)
                continue
            i = random.randint(0, len(available_links)-1)
            for time in ranges:
                self.resources.AssignLink(demand, link, available_links[i], time)

            lists = []
            for time in ranges:
                lists.append(self.resources.GetAvailableLinks(consistencyLink,time))
            available_links = reduce(list_intersect, lists)
            if available_links==[]:
                self.resources.DropVertex(demand, replica)
                self.resources.DropLink(demand,link)
                continue
            i = random.randint(0, len(available_links)-1)
            for time in ranges:
                self.resources.AssignLink(demand, consistencyLink, available_links[i], time)
            success = True
            break
        if not success:
            demand.DeleteReplication(r)
            return False
        return True
            

    def Run(self):
        for d in self.demands:
            self.AssignDemand(d)