import xml.dom.minidom, copy, random
from Core.AbstractGraph import AbstractGraph, AbstractVertex
from Core.ParamFactory import *

class Storage(AbstractVertex):
    resource="storage"
    name="storage"
    ''' Storage element

    :param id: name
    :param volume: storage size
    :param type: storage type (enum/int)
    '''
    def __init__(self, id):
        AbstractVertex.__init__(self, id)
        self.params = ParamFactory.Create("st")

class VM(AbstractVertex):
    resource="vm"
    name="vm"
    ''' Computer element

    :param id: name
    :param speed: computer performance
    :param ram: RAM capacity
    '''
    def __init__(self, id):
        AbstractVertex.__init__(self, id)
        self.image = ""
        self.external = False
        self.params = ParamFactory.Create("vm")

class Vnf(AbstractVertex):
    resource="vm"
    name="vnf"
    def __init__(self, id=""):
        AbstractVertex.__init__(self, id)
        self.type = ""
        self.profile = ""
        self.isservice = False
        self.servicename = ""
        self.username = ""
        self.image = ""
        self.connectionset = []
        self.params = ParamFactory.Create("vnf")

class Domain(AbstractVertex):
    resource="vm"
    name="domain"
    def __init__(self, id, type=""):
        AbstractVertex.__init__(self, id)
        self.type = type
        self.params = ParamFactory.Create("domain")

class NetElement(AbstractVertex):
    resource="netelement"
    name="netelement"
    def __init__(self, id, type=""):
        AbstractVertex.__init__(self, id)
        self.type = type
        self.ip = ""
        self.router = False
        self.isservice = False
        self.servicename = ""
        self.provider = ""
        self.port = ""
        self.prefix = "1"
        self.params = ParamFactory.Create("netelement")

class Link:
    ''' Channel;

    :param e1: first node
    :param e2: second node
    :param capacity: bandwidth
    '''
    def __init__(self, e1, e2, capacity, port1=None, port2=None):
        self.e1 = e1
        self.e2 = e2
        if not port1:
            self.port1 = e1.addPort()
        else:
            self.port1 = port1
        if not port2:
            self.port2 = e2.addPort()
        else:
            self.port2 = port2
        self.capacity = capacity
        self.service = False
        self.assigned = []
        self.assignments = []

    def usedCapacity(self):
        s = sum([e[0].capacity for e in self.assignments])
        return s
        
class Tenant(AbstractGraph):
    ''' Tenant
    '''

    def __init__(self):
        AbstractGraph.__init__(self)
        self.name = ""
        self.type = ""
        self.expiration = ""
        self.assigned = False
        self.critical = False
        
        self.nocheckassigned = False

    def WithoutCheckAssign(self, vertex, node):
        '''
        vertex - tenant vertex
        node - resource vertex where it is assigned
        '''
        vertex.assigned = node
        node.assignments.append([vertex, self])
        node.updateParams()
        return True
        
        
    def NoCheckAssign(self, vertex, node):
        #vertex - tenant, node - resource
        vertex.assigned = node
        node.assignments.append([vertex, self])
        node.updateParams()
        s = 0
        percent = 0
        for p in vertex.params.values():
            percent = (node.paramvalues[p.name]) / node.params[p.name].value
            print node.paramvalues[p.name], node.params[p.name].value
            print "percent", percent
            if (percent <= 1 and percent >= 0):
                s += percent
            else:
                s += (percent * 10)
        return s
        
    def CheckAssignments(self):
        for v in self.vertices:
            node = v.assigned  
            if node: 
                for p in v.params.values():
                    if node.paramvalues[p.name] > node.params[p.name].value:
                        return False
        return True
        
    def PrintAssignments(self):
        print "PRINTING ASSIGNMENTS..."
        for v in self.vertices:
            node = v.assigned  
            if node: 
                for p in v.params.values():
                    print node.paramvalues[p.name], node.params[p.name].value
                    if node.paramvalues[p.name] > node.params[p.name].value:
                        return False
        return True

    def Assign(self, vertex, node):
        '''
        vertex - tenant vertex
        node - resource vertex where it is assigned
        '''
        for p in vertex.params.values():
            if p.value + node.paramvalues[p.name] > node.params[p.name].value:
                return False
        vertex.assigned = node
        node.assignments.append([vertex, self])
        node.updateParams()
        return True
        
    def PrintValues(self):
        for v in self.vertices:
            print "\n========NEW VERTICE OF TENANT", v
            node = v.assigned  
            print "appropriate node", node
            for p in v.params.values():
                print "--------------------"
                print "tenant.v.params.values.value, p.name", p.value, p.name
                if node:
                    print "node.paramvalues[p.name]", node.paramvalues[p.name]
                    print "node.params[p.name].value", node.params[p.name].value

    def LoadAssign(self, vt, id, resources):
        if not resources:
            return
        # TODO: error handling
        nodes = [v for v in resources.vertices if v.id == id]
        if not nodes:
            print "Can't find resource ", id
            return
        node = nodes[0]
        vt.assigned = node
        node.assignments.append([vt, self])

    def AssignLink(self, e, id, resources):
        if not resources:
            return
        # TODO: error handling!!
        nodes = id.replace(" ", "").split(";")
        verts = []
        for n in nodes:
            ss = n.split(":")
            name = ss[0]
            port = ss[1]
            node = [v for v in resources.vertices if v.id == name][0]
            verts.append([node, port])
        for i in range(len(verts) / 2):
            v1 = verts[i][0]
            v2 = verts[i+1][0]
            edge = resources.FindEdge(v1, v2)
            if not edge:
                print "Assignment ", id, " is incorrect: can't find edge from ", v1.id, " to ", v2.id
                return
            #if ((edge.port1 == prev[1]) and (edge.port2 == v[1])) or ((edge.port1 == v[1]) and (edge.port2 == prev[1])) :
            edge.assignments.append([e, self])
            e.assigned.append(edge)

    def AssignRandomly(self, resources):
        pass
            
    def RemoveAssignment(self):
        for v in self.vertices:
            if v.assigned:
                newl = []
                for s in v.assigned.assignments:
                    if s[0] != v:
                        newl.append(s)
                v.assigned.assignments = newl
                v.assigned = None
        for v in self.edges:
            if v.assigned:
                for e in v.assigned:
                    newl = []
                    for s in e.assignments:
                        if s[0] != v:
                            newl.append(s)
                    e.assignments = newl
                v.assigned = None

    def ExportToXml(self):
        '''
        :returns: string with XML representation
        '''
        dom = xml.dom.minidom.Document()
        root = self.CreateXml(dom)
        dom.appendChild(root)
        return dom.toprettyxml()

    def CreateXml(self, dom):
        root = dom.createElement("tenant")
        root.setAttribute("expiration_time", self.expiration)
        root.setAttribute("tenant_type", self.type)
        root.setAttribute("name", self.name)
        nodes = dom.createElement("list_of_nodes")
        for v in self.vertices:
            if isinstance(v, VM):
                tag = dom.createElement("vm")
                tag.setAttribute("name", v.id)
                tag.setAttribute("image_id", v.image)
                tag.setAttribute("floating_ip", str(v.external))
                if v.assigned:
                    tag.setAttribute("assignedTo", v.assigned.id)
            elif isinstance(v, Storage):
                tag = dom.createElement("st")
                tag.setAttribute("name", v.id)
                if v.assigned:
                    tag.setAttribute("assignedTo", v.assigned.id)
            elif isinstance(v, NetElement):
                tag = dom.createElement("netelement")
                tag.setAttribute("name", v.id)
                tag.setAttribute("netelement_type", v.type)
                tag.setAttribute("ip", v.ip)
                tag.setAttribute("is_router", "1" if v.router else "0")
                tag.setAttribute("is_service", "1" if v.isservice else "0")
                tag.setAttribute("service_name", v.servicename)
                tag.setAttribute("provider_name", v.provider)
                tag.setAttribute("external_port", v.port)
                if v.assigned:
                    tag.setAttribute("assignedTo", v.assigned.id)
            elif isinstance(v, Domain):
                tag = dom.createElement("domain")
                tag.setAttribute("name", v.id)
                tag.setAttribute("commutation_type", v.type)
                if v.assigned:
                    tag.setAttribute("assignedTo", v.assigned.id)
            elif isinstance(v, Vnf):
                tag = dom.createElement("vnf")
                tag.setAttribute("name", v.id)
                tag.setAttribute("vnf_type", v.type)
                tag.setAttribute("profile_type", v.profile)
                tag.setAttribute("image_id", v.image)
                tag.setAttribute("is_service", "1" if v.isservice else "0")
                tag.setAttribute("service_name", v.servicename)
                tag.setAttribute("user_name", v.username)
                if v.assigned:
                    tag.setAttribute("assignedTo", v.assigned.id)
                conset = dom.createElement("external_connection_set")
                conset.setAttribute("number_of_ports", str(len(v.connectionset)))
                ports = [s for s in v.connectionset]
                if not ports:
                    ports = ["default_port"]
                for s in ports:
                    port = dom.createElement("external_port")
                    port.setAttribute("name", s)
                    conset.appendChild(port)
                tag.appendChild(conset)
            if v.x:
                tag.setAttribute("x", str(v.x))
                tag.setAttribute("y", str(v.y))

            tag.setAttribute("service", "1" if v.service else "0")
            conset = dom.createElement("connection_set")
            conset.setAttribute("number_of_ports", str(len(v.ports)))
            for s in v.ports:
                port = dom.createElement("port")
                port.setAttribute("name", s)
                conset.appendChild(port)
            tag.appendChild(conset)
            pset = dom.createElement("parameter_set")
            for p in v.params.values():
                param = dom.createElement("parameter")
                param.setAttribute("parameter_name", p.name)
                param.setAttribute("parameter_type", p.type)
                param.setAttribute("parameter_value", str(p.value))
                pset.appendChild(param)
            tag.appendChild(pset)
            nodes.appendChild(tag)
        root.appendChild(nodes)
        links = dom.createElement("list_of_links")
        for v in self.edges:
            tag = dom.createElement("link")
            tag.setAttribute("service", "1" if v.service else "0")
            tag.setAttribute("channel_capacity", str(v.capacity))
            tag.setAttribute("node1", v.e1.id)
            tag.setAttribute("port1", v.port1)
            tag.setAttribute("node2", v.e2.id)
            tag.setAttribute("port2", v.port2)
            if v.assigned:
                res = ""
                for e in v.assigned:
                    res += e.e1.id + ":" + e.port1 + ";" + e.e2.id + ":" + e.port2 + ";"
                tag.setAttribute("assignedTo", res[:-1])
            links.appendChild(tag)
        root.appendChild(links)
        return root

    def LoadFromXML(self, filename):
        ''' Load edges and vertices from XML
        
        .. warning:: Describe XML format here'''
        f = open(filename, "r")
        dom = xml.dom.minidom.parse(f)
        self.vertices = []
        self.edges = []
        for node in dom.childNodes:
            if isinstance(node, xml.dom.minidom.Text) or isinstance(node, xml.dom.minidom.Comment):
                continue
            if node.tagName == "tenant":
                self.LoadFromXmlNode(node)
        f.close()

    def ParseNodes(self, root, resources):
        for vertex in root.childNodes:
            if isinstance(vertex, xml.dom.minidom.Text) or isinstance(vertex, xml.dom.minidom.Comment):
                continue
            service = True if vertex.getAttribute("service") == "1" else False
            ports = []
            params = []
            conset = []
            for v in vertex.childNodes:
                if isinstance(v, xml.dom.minidom.Text) or isinstance(v, xml.dom.minidom.Comment):
                    continue
                if v.nodeName == "connection_set":
                    for port in v.childNodes:
                        if isinstance(port, xml.dom.minidom.Text) or isinstance(port, xml.dom.minidom.Comment):
                            continue
                        s = port.getAttribute("name")
                        ports.append(s)
                if v.nodeName == "external_connection_set":
                    for port in v.childNodes:
                        if isinstance(port, xml.dom.minidom.Text) or isinstance(port, xml.dom.minidom.Comment):
                            continue
                        s = port.getAttribute("name")
                        conset.append(s)
                if v.nodeName == "parameter_set":
                    for param in v.childNodes:
                        if isinstance(param, xml.dom.minidom.Text) or isinstance(param, xml.dom.minidom.Comment):
                            continue
                        name = param.getAttribute("parameter_name")
                        type = param.getAttribute("parameter_type")
                        value = param.getAttribute("parameter_value")
                        params.append([name, type, value])
            if vertex.nodeName == "vm":
                v = VM(vertex.getAttribute("name"))
                v.image = vertex.getAttribute("image_id") 
                v.external = vertex.getAttribute("floating_ip") == "True"           
            elif vertex.nodeName == "st":
                v = Storage(vertex.getAttribute("name"))
            elif vertex.nodeName == "netelement":
                tag = vertex
                v = NetElement(tag.getAttribute("name"))              
                v.type = tag.getAttribute("netelement_type")
                v.ip = tag.getAttribute("ip")
                v.router = tag.getAttribute("is_router") == "1"
                v.isservice = tag.getAttribute("is_service") == "1"
                v.servicename = tag.getAttribute("service_name")
                v.provider = tag.getAttribute("provider_name")
                v.port = tag.getAttribute("external_port") 
            elif vertex.nodeName == "vnf":
                v = Vnf()
                tag = vertex
                v.id = tag.getAttribute("name")
                v.type = tag.getAttribute("vnf_type")
                v.image = tag.getAttribute("image_id")
                v.profile = tag.getAttribute("profile_type")
                v.isservice = tag.getAttribute("is_service") == "1"
                v.servicename = tag.getAttribute("service_name")
                v.username = tag.getAttribute("user_name")
                v.connectionset = conset
            elif vertex.nodeName == "domain":
                v = Domain(vertex.getAttribute("name"))
                v.type = vertex.getAttribute("commutation_type")
            assigned = vertex.getAttribute("assignedTo") 
            x = vertex.getAttribute("x")
            y = vertex.getAttribute("y")
            if (x != '') and (y != ''):
                v.x = float(x)
                v.y = float(y)
            else:
                self.GenCoords(v)
            v.service = service
            v.ports = ports
            for vp in v.params.values():
                for p in params:
                    if (p[0] == vp.name) and (p[1] == vp.type):
                        if (vp.type == "integer"):
                            p[2] = int(p[2])
                        if (vp.type == "real"):
                            p[2] = float(p[2])
                        vp.value = p[2]
            if assigned:
                self.LoadAssign(v, assigned, resources)
            self.vertices.append(v)

    def ParseLinks(self, root, resources):
        for edge in root.childNodes:
            if isinstance(edge, xml.dom.minidom.Text) or isinstance(edge, xml.dom.minidom.Comment):
                continue
            if edge.nodeName == "link":
                src = edge.getAttribute("node1")
                port1 = edge.getAttribute("port1")
                dst = edge.getAttribute("node2")
                port2 = edge.getAttribute("port2")
                try:
                    cap = int(edge.getAttribute("channel_capacity"))
                except:
                    cap = 0
                service = edge.getAttribute("service") == "1"
                assigned = edge.getAttribute("assignedTo")
                try:
                    srcv = [v for v in self.vertices if v.id == src][0]
                    dstv = [v for v in self.vertices if v.id == dst][0]
                except:
                    print("Incorrect link:", src, dst)
                    continue
                e = Link(srcv, dstv, cap, port1, port2)
                e.service = service
                if assigned:
                    self.AssignLink(e, assigned, resources)
                self.edges.append(e)

    def LoadFromXmlNode(self, node, resources=None):
        self.expiration = node.getAttribute("expiration_time")
        self.type = node.getAttribute("tenant_type")
        self.name = node.getAttribute("name")
        #Parse vertices
        for vertex in node.childNodes:
            if isinstance(vertex, xml.dom.minidom.Text):
                continue
            if vertex.nodeName == "list_of_links":
                continue
            if vertex.nodeName == "list_of_nodes":
                self.ParseNodes(vertex, resources)
        for vertex in node.childNodes:
            if isinstance(vertex, xml.dom.minidom.Text):
                continue
            if vertex.nodeName == "list_of_links":
                self.ParseLinks(vertex, resources)

        self._buildPaths()

    def LoadFromXmlNodeOld(self, node, resources=None):                
        self.name = node.getAttribute("id")

        #Parse vertices
        for vertex in node.childNodes:
            if isinstance(vertex, xml.dom.minidom.Text):
                continue
            if vertex.nodeName == "link" or vertex.nodeName == "replica":
                continue
            name = vertex.getAttribute("name")
            if vertex.nodeName == "vm":
                speed = int(vertex.getAttribute("speed"))
                ram = int(vertex.getAttribute("ramcapacity")) if vertex.hasAttribute("ramcapacity") else 0
                v = VM(name)
                v.params["RAM"].value = ram
                v.params["RootDisk"].value = speed
            elif vertex.nodeName == "storage":
                volume = int(vertex.getAttribute("volume"))
                v = Storage(name)
                v.params["size"].value = volume
            x = vertex.getAttribute("x")
            y = vertex.getAttribute("y")
            if (x != '') and (y != ''):
                v.x = float(x)
                v.y = float(y)
            else:
                self.GenCoords(v)
            self.vertices.append(v)

                   
        #Parse edges
        '''for edge in node.childNodes:
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

        self._buildPaths()'''
        
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
            
    def UpdateAssignFlag(self):
        flag = False
        for v in self.vertices:
            if v.assigned:
                flag = True
                continue
            flag = False
            self.assigned = False
            return flag
        self.assigned = flag
        return flag

