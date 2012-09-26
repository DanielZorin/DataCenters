import random
from Core.Resources import Storage, Computer, Router, Link
from Core.Demands import VM, DemandStorage, DemandLink

class RandomMethod:
    def __init__(self, resources, demands):
        self.resources = resources
        self.demands = demands

    def GetAvailableVertices(self,v):
        availableVertices = []
        for v1 in self.resources.vertices:
            if isinstance(v, VM) and isinstance(v1, Computer) and (v.speed <= v1.speed - v1.usedSpeed):
                availableVertices.append(v1)
            if isinstance(v, DemandStorage) and isinstance(v1, Storage) and (v.type == v1.type) and (v.volume <= v1.volume - v1.usedVolume):
                availableVertices.append(v1)
        return availableVertices

    def GetAvailableLinks(self, e):
        availableLinks = []
        g = self.resources.FindPath(e.e1.resource, e.e2.resource)
        while True:
            try:
                p = g.next()
                if self.checkPath(p,e):
                    availableLinks.append(p)
            except StopIteration:
                return availableLinks

    def checkPath(self,path,link):
        for elem in path[1:len(path)-1]:
            if isinstance(elem, Router):
                if (link.capacity > elem.capacity - elem.usedCapacity):
                    return False
            else:
                e = self.resources.FindEdge(elem.e1, elem.e2)
                if link.capacity > e.capacity - e.usedCapacity:
                    return False
        return True

    def AssignVertex(self, demand, vdemand, vresource):
        vdemand.resource = vresource
        vresource.assignedDemands.append([demand, vdemand])
        if isinstance(vdemand,VM):
            vresource.usedSpeed += vdemand.speed
        elif isinstance(vdemand,DemandStorage):
            vresource.usedVolume += vdemand.volume

    def AssingLink(self, demand, link, path):
        link.path = path
        for elem in path[1:len(path)-1]:
            if isinstance(elem, Router):
                elem.usedCapacity += link.capacity
                elem.assignedDemands.append([demand,link])
            else:
                e = self.resources.FindEdge(elem.e1, elem.e2)
                e.usedCapacity += link.capacity
                e.assignedDemands.append([demand,link])

    def AssignDemand(self,demand):
        iter = 0
        while True and (iter<1000): #FIXME
            iter+=1
            success = True
            for v in demand.vertices:
                v1 = self.GetAvailableVertices(v)
                if v1==[]:
                    self.DropDemand(demand)
                    success = False
                    break;
                i = random.randint(0, len(v1)-1)
                self.AssignVertex(demand,v,v1[i])
            if not success:
                continue
            for e in demand.edges:
                if e.e1.resource == e.e2.resource:
                    continue
                e1 = self.GetAvailableLinks(e)
                if e1==[]:
                    self.DropDemand(demand)
                    success=False
                    break
                i = random.randint(0, len(e1)-1)
                self.AssingLink(demand,e,e1[i])
            if success:
                break
        if iter == 1000:
            print "Failed to assign demand " + demand.id
            return False
        return True

    def DropVertex(self,demand,v):
        if v.resource == None:
            return
        if isinstance(v,VM):
            v.resource.usedSpeed -= v.speed
        elif isinstance(v,DemandStorage):
            v.resource.usedVolume -= v.volume
        v.resource.assignedDemands.remove([demand,v])
        v.resource = None

    def DropLink(self,demand,link):
        if link.path == []:
            return
        path = link.path
        for elem in path[1:len(path)-1]:
            if isinstance(elem, Router):
                elem.usedCapacity -= link.capacity
                elem.assignedDemands.remove([demand,link])
            else:
                e = self.resources.FindEdge(elem.e1, elem.e2)
                e.usedCapacity -= link.capacity
                e.assignedDemands.remove([demand,link])
        link.path = None

    def DropDemand(self,demand):
        for v in demand.vertices:
            self.DropVertex(demand,v)
        for e in demand.edges:
            self.DropLink(demand,e)

    def Run(self):
        for d in self.demands:
            self.AssignDemand(d)
        for d in self.demands:
            print "\n"+d.id
            for v in d.vertices:
                print v.resource
            for e in d.edges:
                print e.path
        #for d in self.demands:
            #self.DropDemand(d)
        