import random, xml.dom.minidom

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

class Demand:
    def __init__(self, id):
        self.id = id
        self.vertices = []
        self.edges = []

    def AddLink(self, e):
        self.edges.append(e)

    def AddVertex(self, v):
        self.vertices.append(v)
        v.number = len(self.vertices)

    def DeleteVertex(self, v):
        ind = self.vertices.index(v)
        new_edges = []
        for e in self.edges:
            if e.e1 != v and e.e2 != v:
                new_edges.append(e)
            else:
                del e
        self.edges = new_edges
        del self.vertices[ind]

    def DeleteEdge(self, ed):
        new_edges = []
        for e in self.edges:
            if e != ed:
                new_edges.append(e)
            else:
                del e
        self.edges = new_edges

    def FindEdge(self, v1, v2):
        '''Search for a specific edge from v1 to v2. Returns None if the edge doesn't exist'''
        for ver in self.edges:
            if (ver.e1 == v1):
                if (ver.e2 == v2):
                    return ver
        return None
    
    def FindAllEdges(self, v1 = None, v2 = None):
        '''Search for all edges where source is v1 and destination is v2. 
        If v1 or v2 is None, it doesn't set any restrictions.
        I.e. FindAllEdges(None, None) returns a list of all edges of the graph'''
        res = []
        for ver in self.edges:
            if (v1 is None) or (ver.e1 == v1):
                if (v2 is None) or (ver.e2 == v2):
                    res.append(ver)
        return res

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
