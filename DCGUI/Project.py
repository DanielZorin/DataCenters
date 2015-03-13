from Core.Resources import ResourceGraph
from Core.Tenant import Tenant, NetElement
import xml.dom.minidom
import os

def GetNumber():
    i = 0
    while True:
        i += 1
        yield i

numbers = GetNumber()

class Project:
    resources = None
    tenants = []
    name = "test project"

    def __init__(self):
        self.resources = ResourceGraph()
        self.tenants = []
        #self.method = RandomMethod(self.resources, self.tenants)

    def CreateTenant(self):
        d = Tenant()
        self.tenants.append(d)
        return d

    def RemoveTenant(self,d):
        self.tenants.remove(d)

    def CreateRandomTenant(self, dict):
        d = self.CreateTenant()
        d.id = "Random_" + str(numbers.next())
        d.GenerateRandom(dict)
        return d

    def Save(self, filename):
        dom = xml.dom.minidom.Document()
        dcxml = dom.createElement("dcxml")
        dcxml.setAttribute("name", self.name)
        resgraph = self.resources.CreateXml(dom)
        dcxml.appendChild(resgraph)
        tenants = dom.createElement("tenants")
        for d in self.tenants:
            dem = d.CreateXml(dom)
            tenants.appendChild(dem)
        dcxml.appendChild(tenants)
        dom.appendChild(dcxml)
        f = open(filename, "w")
        f.write(dom.toprettyxml())
        f.close()

    def Load(self, filename, light = False):
        f = open(filename, "r")
        dom = xml.dom.minidom.parse(f)
        self.tenants = []
        for root in dom.childNodes:
            if root.tagName == "dcxml":
                self.name = root.getAttribute("name")
            for node in root.childNodes:
                if isinstance(node, xml.dom.minidom.Text):
                    continue
                if node.tagName == "resources":
                    self.resources.LoadFromXmlNode(node)
            for node in root.childNodes:
                if isinstance(node, xml.dom.minidom.Text):
                    continue
                elif node.tagName == "tenants":
                    for t in node.childNodes:
                        if isinstance(t, xml.dom.minidom.Text):
                            continue
                        if t.tagName == "tenant":
                            d = self.CreateTenant()
                            d.LoadFromXmlNode(t, self.resources)
        if not light:
            #get time intervals and assigne tenants
            self.resources.LoadAllTenants(self.tenants)
        f.close()
        #self.method = RandomMethod(self.resources, self.tenants)    
        
    def Reset(self):
        for d in self.tenants:
            if d.assigned:
                try:
                    self.resources.DropTenant(d)
                    self.resources.RemoveIntervals(d)
                except:
                    pass   
                
    def GetStats(self):
        stats = {"tenants":0}
        for d in self.tenants:
            if d.assigned:  
                stats["tenants"] += 1
        stats["ratio"] = float(stats["tenants"]) / float(len(self.tenants)) * 100  
        totalCapacity = 0.0
        totalLeafCapacity = 0.0
        for e in self.resources.edges:
            totalCapacity += e.capacity
            if not isinstance(e.e1, NetElement) or not isinstance(e.e2, NetElement):
                totalLeafCapacity += e.capacity
        usedCapacity = 0.0
        usedLeafCapacity = 0.0
        for e in self.resources.edges:
            usedCapacity += e.usedCapacity()
            if not isinstance(e.e1, NetElement) or not isinstance(e.e2, NetElement):
                usedLeafCapacity += e.usedCapacity()

        stats["netavg"] = 0.0 if totalCapacity==0 else round(usedCapacity/totalCapacity*100,2)
        stats["leafavg"] = 0.0 if totalLeafCapacity==0 else round(usedLeafCapacity/totalLeafCapacity*100,2)
        return stats      

    def FindTenant(self, id):
        for d in self.tenants:
            if d.name == id:
                return d
    
    def IsAssignmentFull(self):
        for ten in self.tenants:
            if ten.assigned == False:
                return False
        return True
        
    def AssignedTenantsNumber(self):
        kol = 0
        for ten in self.tenants:
            if ten.assigned == True:
                kol += 1
        return kol
    
    def TargetFunction(self):
        summ = 0
        for ten in self.tenants:
            if ten.assigned == False:
                continue
            for v in ten.vertices:
                    lists = v.assigned.assignments
                    print lists
                    nodes = lists[0]
                    node = nodes[0]
                    print "node", node
                    for p in v.params.values():
                        percent = (node.params[p.name].value - p.value - node.paramvalues[p.name]) / node.params[p.name].value
                        if percent >= 0:
                            summ += percent
                        else:
                            summ += (percent * 10)
        return summ
    
    def CheckAssignments(self):
        dif = 0
        for ten in self.tenants:
            if ten.assigned == False:
                continue
            for v in ten.vertices:
                    node = v.assigned.assignments
                    for p in vertex.param.values():
                        dif = (node.params[p.name].value - p.value - node.paramvalues[p.name])
                        if dif < 0:
                            return False
        return True
        
    def PrintTenantsAssignmentFlags(self):
        for ten in self.tenants:
            if ten.assigned == True:
                print "assigned"
            else:
                print "unassigned"
               
