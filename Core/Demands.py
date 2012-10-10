import random, xml.dom.minidom, math
from Core.AbstractGraph import AbstractGraph, AbstractVertex

class VM(AbstractVertex):
    def __init__(self, id, speed):
        AbstractVertex.__init__(self, id)
        self.id = id
        self.speed = speed
        self.resource = None

class DemandStorage(AbstractVertex):
    def __init__(self, id, volume, type):
        AbstractVertex.__init__(self, id)
        self.volume = volume
        self.type = type
        self.resource = None

class DemandLink:
    def __init__(self, e1, e2, capacity):
        self.e1 = e1
        self.e2 = e2
        self.capacity = capacity
        self.path = []

class Demand(AbstractGraph):
    def __init__(self, id):
        AbstractGraph.__init__(self)
        self.id = id
        self.startTime = 0
        self.endTime = 0
        self.assigned = False

    def GenerateRandom(self, params):
        #TODO: get ranges from the dialog
        self.startTime = random.randint(0, 90)
        self.endTime = random.randint(self.startTime, 100)
        x = 50
        y = 50
        maxi = 40 * int(math.sqrt(params["vms"] + params["storages"]))
        maxx = 0
        for i in range(params["vms"]):
            v = VM("vm_" + str(i), random.randint(params["vm_min"], params["vm_max"]))
            v.x = x
            v.y = y
            self.AddVertex(v)
            if x < maxi:
                x += 40
                maxx = x
            else:
                y += 40
                x = 50
        for i in range(params["storages"]):
            t=0 if params["types"]==[] else params["types"][random.randint(0,len(params["types"])-1)]
            v = DemandStorage("storage_" + str(i), random.randint(params["st_min"], params["st_max"]), t)
            v.x = x
            v.y = y
            self.AddVertex(v)
            if x < maxi:
                x += 40
                maxx = x
            else:
                y += 40
                x = 50
        for i in range(len(self.vertices) - 1):
            for j in range(random.randint(1, 2)):
                src = self.vertices[i]
                dest = self.vertices[random.randint(i+1, len(self.vertices) - 1)]
                capacity = random.randint(params["cap_min"], params["cap_max"])
                e = DemandLink(src, dest, capacity)
                if self.FindEdge(src, dest) == None:
                    self.edges.append(e)

    def ExportToXml(self):
        dom = xml.dom.minidom.Document()
        root = self.CreateXml(dom)
        dom.appendChild(root)
        return dom.toprettyxml()

    def CreateXml(self, dom):
        root = dom.createElement("demand")
        root.setAttribute("id", self.id)
        root.setAttribute("start", str(self.startTime))
        root.setAttribute("end", str(self.endTime))
        root.setAttribute("assigned", str(self.assigned))
        for v in self.vertices:
            if isinstance(v, VM):
                tag = dom.createElement("vm")
                tag.setAttribute("speed", str(v.speed))
            elif isinstance(v, DemandStorage):
                tag = dom.createElement("storage")
                tag.setAttribute("volume", str(v.volume))
                tag.setAttribute("type", str(v.type))
            if v.x:
                tag.setAttribute("x", str(v.x))
                tag.setAttribute("y", str(v.y))
            tag.setAttribute("number", str(v.number))
            tag.setAttribute("name", str(v.id))
            if self.assigned and v.resource:
                tag.setAttribute("assignedto", str(v.resource.number))
            root.appendChild(tag)
        for v in self.edges:
            tag = dom.createElement("link")
            tag.setAttribute("from", str(v.e1.number))
            tag.setAttribute("to", str(v.e2.number))
            tag.setAttribute("capacity", str(v.capacity))
            if self.assigned:
                if v.path != []:
                    pathstr = ""
                    i = 0
                    while i <= len(v.path):
                        pathstr += str(v.path[i].number) + ";"
                        i += 2
                else:
                    pathstr = "none"
                tag.setAttribute("assignedto", pathstr)

            root.appendChild(tag)
        return root

    def LoadFromXml(self, filename):
        f = open(filename, "r")
        dom = xml.dom.minidom.parse(f)
        self.vertices = []
        self.edges = []
        for node in dom.childNodes:
            if node.tagName == "demand":
                self.LoadFromXmlNode(node)
        f.close()

    def LoadFromXmlNode(self, node, resources):
        self.id = node.getAttribute("id")
        self.startTime = int(node.getAttribute("start"))
        self.endTime = int(node.getAttribute("end"))
        self.assigned = True if node.getAttribute("assigned") == "True" else False
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
            x = vertex.getAttribute("x")
            y = vertex.getAttribute("y")
            if x != '':
                v.x = float(x)
            if y != '':
                v.y = float(y)
            v.number = number
            if self.assigned:
                num = int(vertex.getAttribute("assignedto"))
                v.resource = (v for v in resources.vertices if v.number == num).next()
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
                if self.assigned:
                    verts = edge.getAttribute("assignedto")
                    if verts == "none":
                        e.path = []
                    else:
                        nums = [int(s) for s in verts.split(";") if s != ""]
                        print nums
                        path = [(v for v in resources.vertices if v.number == nums[0]).next()]
                        
                        for n in nums[1:]:
                            vert = (v for v in resources.vertices if v.number == n).next()
                            edge = resources.FindEdge(path[-1], vert)
                            path += [edge, vert]
                        e.path = path
                    

    def FindVertex(self, number):
        for v in self.vertices:
            if v.number == number:
                return v