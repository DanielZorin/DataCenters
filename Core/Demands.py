import random, xml.dom.minidom, math
from Core.AbstractGraph import AbstractGraph, AbstractVertex

class VM(AbstractVertex):
    def __init__(self, id, speed, ram=0):
        AbstractVertex.__init__(self, id)
        self.id = id
        self.speed = speed
        self.ram = ram
        self.resource = None

class DemandStorage(AbstractVertex):
    def __init__(self, id, volume, type, replicationCapacity):
        AbstractVertex.__init__(self, id)
        self.volume = volume
        self.type = type
        self.resource = None
        self.replicationCapacity = replicationCapacity

class DemandLink:
    def __init__(self, e1, e2, capacity, fromreplica=False, toreplica=False):
        self.e1 = e1
        self.e2 = e2
        self.capacity = capacity
        self.path = []
        self.fromreplica = fromreplica
        self.toreplica = toreplica

class Replication:
    def __init__(self, src, assignedto):
        self.replica = src
        self.assignedto = assignedto

class Demand(AbstractGraph):
    def __init__(self, id):
        AbstractGraph.__init__(self)
        self.id = id
        self.startTime = 0
        self.endTime = 1
        self.replications = []
        self.replicalinks = []
        self.assigned = False
        self.critical = True

    def GenerateRandom(self, params):
        self.startTime = random.randint(params["start"], params["end"] - 1)
        self.endTime = random.randint(self.startTime, params["end"])
        x = 50
        y = 50
        maxi = 40 * int(math.sqrt(params["vms"] + params["storages"]))
        maxx = 0
        for i in range(params["vms"]):
            v = VM("vm_" + str(i), random.randint(params["vm_min"], params["vm_max"]), random.randint(params["ram_min"], params["ram_max"]))
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
            v = DemandStorage("storage_" + str(i), random.randint(params["st_min"], params["st_max"]), t, random.randint(params["cons_cap_min"], params["cons_cap_max"]))
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
                if not params["vmvm"] and isinstance(src, VM) and isinstance(dest, VM):
                    continue
                if not params["stst"] and isinstance(src, DemandStorage) and isinstance(dest, DemandStorage):
                    continue
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
        root.setAttribute("critical", str(self.critical))
        for v in self.vertices:
            if isinstance(v, VM):
                tag = dom.createElement("vm")
                tag.setAttribute("speed", str(v.speed))
                tag.setAttribute("ramcapacity", str(v.ram))
            elif isinstance(v, DemandStorage):
                tag = dom.createElement("storage")
                tag.setAttribute("volume", str(v.volume))
                tag.setAttribute("type", str(v.type))
                tag.setAttribute("replicationcapacity", str(v.replicationCapacity))
            if v.x:
                tag.setAttribute("x", str(v.x))
                tag.setAttribute("y", str(v.y))
            tag.setAttribute("number", str(v.number))
            tag.setAttribute("name", str(v.id))
            if self.assigned and v.resource:
                tag.setAttribute("assignedto", str(v.resource.number))
            root.appendChild(tag)
        for r in self.replications:
            tag = dom.createElement("replica")
            tag.setAttribute("storage_number", str(r.replica.number))
            tag.setAttribute("assignedto", str(r.assignedto.number))
            root.appendChild(tag)
        links = self.edges + self.replicalinks
        for v in links:
            if self.assigned and v.path==[]:
                continue
            tag = dom.createElement("link")
            tag.setAttribute("from", str(v.e1.number))
            tag.setAttribute("to", str(v.e2.number))
            tag.setAttribute("capacity", str(v.capacity))
            if v.fromreplica:
                tag.setAttribute("fromtype", "replica")
            if v.toreplica:
                tag.setAttribute("totype", "replica")
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
        self.replications = []
        self.replicalinks = []
        for node in dom.childNodes:
            if node.tagName == "demand":
                self.LoadFromXmlNode(node)
        f.close()

    def LoadFromXmlNode(self, node, resources):
        self.id = node.getAttribute("id")
        self.startTime = int(node.getAttribute("start"))
        self.endTime = int(node.getAttribute("end"))
        self.assigned = True if node.getAttribute("assigned") == "True" else False
        self.critical = True if node.getAttribute("critical") == "True" else False
        #Parse vertices
        for vertex in node.childNodes:
            if isinstance(vertex, xml.dom.minidom.Text):
                continue
            if vertex.nodeName == "link" or vertex.nodeName == "replica":
                continue
            name = vertex.getAttribute("name")
            number = int(vertex.getAttribute("number"))
            if vertex.nodeName == "vm":
                speed = int(vertex.getAttribute("speed"))
                ram = int(vertex.getAttribute("ramcapacity")) if vertex.hasAttribute("ramcapacity") else 0
                v = VM(name, speed, ram)
            elif vertex.nodeName == "storage":
                volume = int(vertex.getAttribute("volume"))
                type = int(vertex.getAttribute("type"))
                replicationCapacity = int(vertex.getAttribute("replicationcapacity"))
                v = DemandStorage(name, volume, type, replicationCapacity)
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

        for vertex in node.childNodes:
            if isinstance(vertex, xml.dom.minidom.Text):
                continue
            if vertex.nodeName == "replica":
                storage = int(vertex.getAttribute("storage_number"))
                v = (v for v in self.vertices if v.number == storage).next()  
                num = int(vertex.getAttribute("assignedto"))
                assign = (v for v in resources.vertices if v.number == num).next()
                if all(r.replica != v and r.assignedto != assign for r in self.replications):
                    self.replications.append(Replication(v, assign))                    
        #Parse edges
        for edge in node.childNodes:
            if edge.nodeName == "link":
                source = int(edge.getAttribute("from"))
                destination = int(edge.getAttribute("to"))
                cap = int(edge.getAttribute("capacity"))
                e = DemandLink(self.vertices[source-1], self.vertices[destination-1], cap)
                if edge.getAttribute("fromtype") == "replica":
                    e.fromreplica = True
                    self.replicalinks.append(e)
                    if self.vertices[source-1] != self.vertices[destination-1]:
                        self.edges.append(DemandLink(self.vertices[source-1], self.vertices[destination-1], cap))
                elif edge.getAttribute("totype") == "replica":
                    e.toreplica = True
                    self.replicalinks.append(e)
                    if self.vertices[source-1] != self.vertices[destination-1]:
                        self.edges.append(DemandLink(self.vertices[source-1], self.vertices[destination-1], cap))
                else:
                    self.edges.append(e)
                if self.assigned:
                    verts = edge.getAttribute("assignedto")
                    if verts == "none":
                        e.path = []
                    else:
                        nums = [int(s) for s in verts.split(";") if s != ""]
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
