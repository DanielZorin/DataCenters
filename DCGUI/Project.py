from Core.Resources import ResourceGraph, Computer, Storage, Router
from Core.Demands import Demand
from Methods.RandomMethod import RandomMethod
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
    demands = []
    name = "test project"

    def __init__(self):
        self.resources = ResourceGraph()
        self.demands = []
        self.method = RandomMethod(self.resources, self.demands)

    def CreateDemand(self):
        d = Demand("")
        self.demands.append(d)
        return d

    def RemoveDemand(self,d):
        self.demands.remove(d)

    def CreateRandomDemand(self, dict):
        d = self.CreateDemand()
        d.id = "Random_" + str(numbers.next())
        d.GenerateRandom(dict)
        return d

    def Save(self, filename):
        dom = xml.dom.minidom.Document()
        root = dom.createElement("dcxml")
        root.setAttribute("name", self.name)
        resgraph = self.resources.CreateXml(dom)
        root.appendChild(resgraph)
        for d in self.demands:
            dem = d.CreateXml(dom)
            root.appendChild(dem)
        dom.appendChild(root)
        f = open(filename, "w")
        f.write(dom.toprettyxml())
        f.close()

    def Load(self, filename, light = False):
        f = open(filename, "r")
        dom = xml.dom.minidom.parse(f)
        self.demands = []
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
                    elif node.tagName == "demand":
                        d = self.CreateDemand()
                        d.LoadFromXmlNode(node, self.resources)
        if not light:
            #get time intervals and assigne demands
            self.resources.LoadAllDemands(self.demands)
        f.close()
        self.method = RandomMethod(self.resources, self.demands)    
        
    def Reset(self):
        for d in self.demands:
            if d.assigned:
                try:
                    self.resources.DropDemand(d)
                    self.resources.RemoveIntervals(d)
                except:
                    pass   
                
    def GetStats(self):
        r = self.resources.GetTimeBounds()
        time = r[1] - r[0]
        stats = {"demands":0}
        for d in self.demands:
            if d.assigned:  
                stats["demands"] += 1
        ranges = self.resources.vertices[0].intervals.keys()
        totalSpeed = 0.0
        totalVolume = 0.0
        totalCapacity = 0.0
        totalLeafCapacity = 0.0
        for v in self.resources.vertices:
            if isinstance(v, Computer):
                totalSpeed += v.speed
            elif isinstance(v, Storage):
                totalVolume += v.volume
            elif isinstance(v, Router):
                totalCapacity += v.capacity
        for e in self.resources.edges:
            totalCapacity += e.capacity
            if isinstance(e.e1, Computer) or isinstance(e.e1, Storage) or isinstance(e.e2, Computer) or isinstance(e.e2, Storage):
                totalLeafCapacity += e.capacity
        maxUsedSpeed = 0.0
        maxUsedVolume = 0.0
        maxUsedCapacity = 0.0
        maxUsedLeafCapacity = 0.0
        avgUsedSpeed = 0.0
        avgUsedVolume = 0.0
        avgUsedCapacity = 0.0
        avgUsedLeafCapacity = 0.0
        for r in ranges:
            usedSpeed = 0.0
            usedVolume = 0.0
            usedCapacity = 0.0
            usedLeafCapacity = 0.0
            for v in self.resources.vertices:
                if isinstance(v, Computer):
                    usedSpeed += v.intervals[r].usedResource
                elif isinstance(v, Storage):
                    usedVolume += v.intervals[r].usedResource
                elif isinstance(v, Router):
                    usedCapacity += v.intervals[r].usedResource
            for e in self.resources.edges:
                usedCapacity += e.intervals[r].usedResource
                if isinstance(e.e1, Computer) or isinstance(e.e1, Storage) or isinstance(e.e2, Computer) or isinstance(e.e2, Storage):
                    usedLeafCapacity += e.intervals[r].usedResource
            avgUsedSpeed += usedSpeed * (float(r[1]-r[0])/time)
            avgUsedVolume += usedVolume * (float(r[1]-r[0])/time)
            avgUsedCapacity += usedCapacity * (float(r[1]-r[0])/time)
            avgUsedLeafCapacity += usedLeafCapacity * (float(r[1]-r[0])/time)
            if usedSpeed > maxUsedSpeed:
                maxUsedSpeed = usedSpeed
            if usedVolume > maxUsedVolume:
                maxUsedVolume = usedVolume
            if usedCapacity > maxUsedCapacity:
                maxUsedCapacity = usedCapacity
            if usedLeafCapacity > maxUsedLeafCapacity:
                maxUsedLeafCapacity = usedLeafCapacity

        stats["vmavg"] = 0.0 if totalSpeed==0 else round(avgUsedSpeed/totalSpeed*100,2)
        stats["stavg"] = 0.0 if totalVolume==0 else round(avgUsedVolume/totalVolume*100,2)
        stats["netavg"] = 0.0 if totalCapacity==0 else round(avgUsedCapacity/totalCapacity*100,2)
        stats["vmmax"] = 0.0 if totalSpeed==0 else round(maxUsedSpeed/totalSpeed*100,2)
        stats["stmax"] = 0.0 if totalVolume==0 else round(maxUsedVolume/totalVolume*100,2)
        stats["netmax"] = 0.0 if totalCapacity==0 else round(maxUsedCapacity/totalCapacity*100,2)
        stats["leafmax"] = 0.0 if totalLeafCapacity==0 else round(maxUsedLeafCapacity/totalLeafCapacity*100,2)
        stats["leafavg"] = 0.0 if totalLeafCapacity==0 else round(avgUsedLeafCapacity/totalLeafCapacity*100,2)
        return stats      

    def FindDemand(self, id):
        for d in self.demands:
            if d.id == id:
                return d
                
    def Run(self):
        self.Save("tmp_dump.dcxml")
        os.system("..\\Algorithm\\main.exe tmp_dump.dcxml result.dcxml")
        return