import xml.dom.minidom
from Core.AbstractGraph import AbstractGraph, AbstractVertex

class State:
    def __init__(self):
        self.usedResource = 0
        self.demands = {}

class Storage(AbstractVertex):
    def __init__(self, id, volume, type):
        AbstractVertex.__init__(self, id)
        self.volume = volume
        self.type = type
        self.intervals = {}

    def getUsedVolumePercent(self, t):
        return 0 if self.volume == 0 else self.intervals[t].usedResource*100.0/self.volume

class Computer(AbstractVertex):
    def __init__(self, id, speed):
        AbstractVertex.__init__(self, id)
        self.speed = speed
        self.intervals = {}

    def getUsedSpeedPercent(self, t):
        return 0 if self.speed == 0 else self.intervals[t].usedResource*100.0/self.speed

class Router(AbstractVertex):
    def __init__(self, id, capacity):
        AbstractVertex.__init__(self, id)
        self.capacity = capacity
        self.intervals = {}

    def getUsedCapacityPercent(self, t):
        return 0 if self.capacity == 0 else self.intervals[t].usedResource*100.0/self.capacity

class Link:
    def __init__(self, e1, e2, capacity):
        self.e1 = e1
        self.e2 = e2
        self.capacity = capacity
        self.intervals = {}

    def getUsedCapacityPercent(self,t):
        return 0 if self.capacity == 0 else self.intervals[t].usedResource*100.0/self.capacity

class ResourceGraph(AbstractGraph):
    def __init__(self):
        AbstractGraph.__init__(self)

    def ExportToXml(self):
        dom = xml.dom.minidom.Document()
        root = self.CreateXml(dom)
        dom.appendChild(root)
        return dom.toprettyxml()

    def CreateXml(self, dom):
        root = dom.createElement("resources")
        for v in self.vertices:
            if isinstance(v, Computer):
                tag = dom.createElement("computer")
                tag.setAttribute("speed", str(v.speed))
            elif isinstance(v, Storage):
                tag = dom.createElement("storage")
                tag.setAttribute("volume", str(v.volume))
                tag.setAttribute("type", str(v.type))
            elif isinstance(v, Router):
                tag = dom.createElement("router")
                tag.setAttribute("capacity", str(v.capacity))
            if v.x:
                tag.setAttribute("x", str(v.x))
                tag.setAttribute("y", str(v.y))
            tag.setAttribute("number", str(v.number))
            tag.setAttribute("name", str(v.id))
            root.appendChild(tag)
        for v in self.edges:
            tag = dom.createElement("link")
            tag.setAttribute("from", str(v.e1.number))
            tag.setAttribute("to", str(v.e2.number))
            tag.setAttribute("capacity", str(v.capacity))
            root.appendChild(tag)
        return root

    def LoadFromXML(self, filename):
        ''' Load edges and vertices from XML
        
        .. warning:: Describe XML format here'''
        f = open(filename, "r")
        dom = xml.dom.minidom.parse(f)
        self.vertices = []
        self.edges = []
        for node in dom.childNodes:
            if node.tagName == "resources":
                self.LoadFromXmlNode(node)
        f.close()

    def LoadFromXmlNode(self, node):
        #Parse vertices
        for vertex in node.childNodes:
            if isinstance(vertex, xml.dom.minidom.Text):
                continue
            if vertex.nodeName == "link":
                continue
            name = vertex.getAttribute("name")
            number = int(vertex.getAttribute("number"))
            if vertex.nodeName == "computer":
                speed = int(vertex.getAttribute("speed"))
                v = Computer(name, speed)
            elif vertex.nodeName == "storage":
                volume = int(vertex.getAttribute("volume"))
                type = int(vertex.getAttribute("type"))
                v = Storage(name, volume, type)
            elif vertex.nodeName == "router":
                capacity = int(vertex.getAttribute("capacity"))
                v = Router(name,capacity)
            x = vertex.getAttribute("x")
            y = vertex.getAttribute("y")
            if x != '':
                v.x = float(x)
            if y != '':
                v.y = float(y)
            v.number = number
            self.vertices.append(v)

        self.vertices.sort(key=lambda x: x.number)
                    
        #Parse edges
        for edge in node.childNodes:
            if edge.nodeName == "link":
                source = int(edge.getAttribute("from"))
                destination = int(edge.getAttribute("to"))
                cap = int(edge.getAttribute("capacity"))
                e = Link(self.vertices[source-1], self.vertices[destination-1], cap)
                self.edges.append(e)

        self._buildPaths()

    def FindPath(self, v1, v2):
        if not self.PathExists(v1, v2):
            return
        comp = self.compdict[v1]
        paths = [[v1]]
        while True:
            newpaths = []
            for p in paths:
                last = p[len(p)-1]
                links = self.FindAllEdges(v1=last)
                for e in links:
                    other = e.e2 if e.e1 == last else e.e1
                    if v2 == other:
                        yield p + [e, v2]
                    if isinstance(other, Router):
                        if len(p)== 1 or ((len(p) >= 3) and (p[-3] != other) and (p.count(other) == 0)):
                            newpaths.append(p + [e, other])
            if newpaths == []:
                break
            paths = newpaths

    def GetTimeBounds(self):
        t1 = 0
        t0 = 0
        if not (self.vertices == []) and  not (self.vertices[0].intervals.keys() == []):
            t0 = self.vertices[0].intervals.keys()[0][0]
        for v in self.vertices:
            for t in v.intervals.keys():
                if t[1] > t1:
                    t1 = t[1]
                if t[0] < t0:
                    t0 = t[0]
        return (t0, t1)

    def GetTimeInterval(self, time):
        for v in self.vertices:
            for t in v.intervals.keys():
                if (time >= t[0]) and (time <= t[1]):
                    return t