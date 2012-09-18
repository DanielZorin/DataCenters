import random, xml.dom.minidom
from Core.AbstractGraph import AbstractGraph

class VM:
    number = -1
    def __init__(self, id, speed):
        self.id = id
        self.speed = speed

class DemandStorage:
    number = -1
    def __init__(self, id, volume, type):
        self.id = id
        self.volume = volume
        self.type = type

class DemandLink:
    def __init__(self, e1, e2, capacity):
        self.e1 = e1
        self.e2 = e2
        self.capacity = capacity

class Demand(AbstractGraph):
    def __init__(self, id):
        AbstractGraph.__init__(self)
        self.id = id

    def ExportToXml(self):
        dom = xml.dom.minidom.Document()
        root = dom.createElement("demand")
        root.setAttribute("id", self.id)
        dom.appendChild(root)
        for v in self.vertices:
            if isinstance(v, VM):
                tag = dom.createElement("vm")
                tag.setAttribute("speed", str(v.speed))
            elif isinstance(v, DemandStorage):
                tag = dom.createElement("storage")
                tag.setAttribute("volume", str(v.volume))
                tag.setAttribute("type", str(v.type))
            tag.setAttribute("number", str(v.number))
            tag.setAttribute("name", str(v.id))
            root.appendChild(tag)
        for v in self.edges:
            tag = dom.createElement("link")
            tag.setAttribute("from", str(v.e1.number))
            tag.setAttribute("to", str(v.e2.number))
            tag.setAttribute("capacity", str(v.capacity))
            root.appendChild(tag)
        return dom.toprettyxml()

    def GenerateRandom(self, params):
        for i in range(params["vms"]):
            v = VM("vm_" + str(i), random.randint(params["vm_min"], params["vm_max"]))
            self.AddVertex(v)
        for i in range(params["storages"]):
            v = DemandStorage("storage_" + str(i), random.randint(params["st_min"], params["st_max"]), 1)
            self.AddVertex(v)
        for i in range(len(self.vertices) - 1):
            for j in range(random.randint(1, 2)):
                src = self.vertices[i]
                dest = self.vertices[random.randint(i+1, len(self.vertices) - 1)]
                capacity = random.randint(params["cap_min"], params["cap_max"])
                e = DemandLink(src, dest, capacity)
                if self.FindEdge(src, dest) == None:
                    self.edges.append(e)

    def LoadFromXml(self, filename):
        f = open(filename, "r")
        dom = xml.dom.minidom.parse(f)
        self.vertices = []
        self.edges = []
        for node in dom.childNodes:
            if node.tagName == "demand":
                self.id = node.getAttribute("id")
                #Parse vertices
                for vertex in node.childNodes:
                    if isinstance(vertex, xml.dom.minidom.Text):
                        continue
                    if vertex.nodeName == "link":
                        continue
                    name = vertex.getAttribute("name")
                    number = int(vertex.getAttribute("number"))
                    if vertex.nodeName == "vm":
                        speed = int(vertex.getAttribute("speed"))
                        v = VM(name, speed)
                    elif vertex.nodeName == "storage":
                        volume = int(vertex.getAttribute("volume"))
                        type = int(vertex.getAttribute("type"))
                        v = DemandStorage(name, volume, type)
                    v.number = number
                    self.vertices.append(v)

                self.vertices.sort(key=lambda x: x.number)
                    
                #Parse edges
                for edge in node.childNodes:
                    if edge.nodeName == "link":
                        source = int(edge.getAttribute("from"))
                        destination = int(edge.getAttribute("to"))
                        cap = int(edge.getAttribute("capacity"))
                        e = DemandLink(self.vertices[source-1], self.vertices[destination-1], cap)
                        self.edges.append(e)
        f.close()
'''
d = Demand("qwerty")
d.GenerateRandom(
                 {"vms": 5,
                  "vm_max": 10,
                  "vm_min": 2,
                  "storages": 5,
                  "st_min": 3,
                  "st_max": 6,
                  "cap_min": 11,
                  "cap_max": 15
                  })

print d.ExportToXml()
x = 99
'''
