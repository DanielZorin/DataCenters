import xml.dom.minidom, copy, random
from Core.AbstractGraph import AbstractGraph, AbstractVertex
from Core.Tenant import *
from Core.ParamFactory import *


class ResourceGraph(AbstractGraph):
    ''' Graph of physical resources
    '''
    assignedTenants = set([])

    def __init__(self):
        AbstractGraph.__init__(self)

    def ExportToXml(self):
        '''
        :returns: string with XML representation
        '''
        dom = xml.dom.minidom.Document()
        root = self.CreateXml(dom)
        dom.appendChild(root)
        return dom.toprettyxml()

    def CreateXml(self, dom):
        root = dom.createElement("resources")
        for v in self.vertices:
            if isinstance(v, VM):
                tag = dom.createElement("server")
                tag.setAttribute("name", v.id)
                tag.setAttribute("image_id", v.image)
            elif isinstance(v, Storage):
                tag = dom.createElement("storage")
                tag.setAttribute("name", v.id)
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

            if v.x:
                tag.setAttribute("x", str(v.x))
                tag.setAttribute("y", str(v.y))

            tag.setAttribute("service", "1" if v.service else "0")
            conset = dom.createElement("connection_set")
            conset.setAttribute("number_of_ports", str(len(v.ports)))
            for s in v.ports:
                port = dom.createElement("port")
                port.setAttribute("port_name", s)
                conset.appendChild(port)
            tag.appendChild(conset)
            pset = dom.createElement("parameter_set")
            for p in v.params:
                param = dom.createElement("parameter")
                param.setAttribute("parameter_name", p.name)
                param.setAttribute("parameter_type", p.type)
                param.setAttribute("value_user", p.value)
                pset.appendChild(param)
            tag.appendChild(pset)
            root.appendChild(tag)
        for v in self.edges:
            tag = dom.createElement("link")
            tag.setAttribute("service", "1" if v.service else "0")
            tag.setAttribute("channel_capacity", str(v.capacity))
            tag.setAttribute("node1", v.e1.id)
            tag.setAttribute("port1", v.port1)
            tag.setAttribute("node2", v.e2.id)
            tag.setAttribute("port2", v.port2)
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
            if isinstance(node, xml.dom.minidom.Text) or isinstance(node, xml.dom.minidom.Comment):
                continue
            if node.tagName == "resources":
                self.LoadFromXmlNode(node)
        f.close()

    def ParseNodes(self, root, resources):
        for vertex in root.childNodes:
            if isinstance(vertex, xml.dom.minidom.Text) or isinstance(vertex, xml.dom.minidom.Comment) or (vertex.nodeName == "link"):
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
                        s = port.getAttribute("port_name")
                        ports.append(s)
                if v.nodeName == "parameter_set":
                    for param in v.childNodes:
                        if isinstance(param, xml.dom.minidom.Text) or isinstance(param, xml.dom.minidom.Comment):
                            continue
                        name = param.getAttribute("parameter_name")
                        type = param.getAttribute("parameter_type")
                        value = param.getAttribute("value_user")
                        params.append([name, type, value])
            if vertex.nodeName == "server":
                v = VM(vertex.getAttribute("name"))
                v.image = vertex.getAttribute("image_id")            
            elif vertex.nodeName == "storage":
                v = Storage(vertex.getAttribute("name"))
            elif vertex.nodeName == "netelement":
                tag = vertex
                v = NetElement(tag.getAttribute("name"))              
                v.type = tag.getAttribute("netelement_type")
                v.ip = tag.getAttribute("ip")
                v.router = tag.getAttribute("is_router") == 1
                v.isservice = tag.getAttribute("is_service") == 1
                v.servicename = tag.getAttribute("service_name")
                v.provider = tag.getAttribute("provider_name")
                v.port = tag.getAttribute("external_port")
            x = vertex.getAttribute("x")
            y = vertex.getAttribute("y")
            if (x != '') and (y != ''):
                v.x = float(x)
                v.y = float(y)
            else:
                self.GenCoords(v)
            v.service = service
            v.ports = ports
            for vp in v.params:
                for p in params:
                    if (p[0] == vp.name) and (p[1] == vp.type):
                        vp.value = p[2]
            self.vertices.append(v)

    def ParseLinks(self, root):
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
                try:
                    srcv = [v for v in self.vertices if v.id == src][0]
                    dstv = [v for v in self.vertices if v.id == dst][0]
                except:
                    print("Incorrect link:", src, dst)
                    continue
                e = Link(srcv, dstv, cap)
                e.port1 = port1
                e.port2 = port2
                e.service = service
                self.edges.append(e)

    def LoadFromXmlNode(self, node, resources=None):
        self.expiration = node.getAttribute("expiration_time")
        self.type = node.getAttribute("tenant_type")
        self.name = node.getAttribute("tenant_name")
        #Parse vertices
        self.ParseNodes(node, resources)
        self.ParseLinks(node)

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

    def GetAvailableVertices(self,v,time):
        availableVertices = set([])
        for v1 in self.vertices:
            if (isinstance(v, VM) and isinstance(v1, Computer)
                and (v.speed <= v1.speed - v1.intervals[time].usedResource) and (v.ram <= v1.ram - v1.intervals[time].usedRam)):
                availableVertices.add(v1)
            if isinstance(v, TenantStorage) and isinstance(v1, Storage) and (v.type == v1.type) and (v.volume <= v1.volume - v1.intervals[time].usedVolume):
                availableVertices.add(v1)
        return availableVertices

    def GetAvailableLinks(self, e, time):
        availableLinks = []
        g = self.FindPath(e.e1.resource, e.e2.resource)
        while True:
            try:
                p = g.next()
                if self.checkPath(p,e, time):
                    availableLinks.append(p)
            except StopIteration:
                return availableLinks

    def checkPath(self, path, link, time):
        for elem in path[1:len(path)-1]:
            if isinstance(elem, Router):
                if link.capacity > elem.capacity - elem.intervals[time].usedCapacity:
                    return False
            else:
                e = self.FindEdge(elem.e1, elem.e2)
                if link.capacity > e.capacity - e.intervals[time].usedCapacity:
                    return False
        return True

    def DropVertex(self,tenant,v):
        if v.resource == None:
            return
        for time in self.GetRanges(tenant):
            if isinstance(v,VM):
                v.resource.intervals[time].usedSpeed -= v.speed
                v.resource.intervals[time].usedRam -= v.ram
            elif isinstance(v,TenantStorage):
                v.resource.intervals[time].usedVolume -= v.volume
            v.resource.intervals[time].tenants[tenant.id].remove(v.number)
            if v.resource.intervals[time].tenants[tenant.id]==[]:
                del v.resource.intervals[time].tenants[tenant.id]
        v.resource = None

    def DropLink(self,tenant,link):
        if link.path == []:
            return
        path = link.path
        for elem in path[1:len(path)-1]:
            for time in self.GetRanges(tenant):
                if isinstance(elem, Router):
                    elem.intervals[time].usedCapacity -= link.capacity
                    elem.intervals[time].tenants[tenant.id].remove((link.e1.number,link.e2.number))
                    if elem.intervals[time].tenants[tenant.id]==[]:
                        del elem.intervals[time].tenants[tenant.id]
                else:
                    e = self.FindEdge(elem.e1, elem.e2)
                    e.intervals[time].usedCapacity -= link.capacity
                    e.intervals[time].tenants[tenant.id].remove((link.e1.number,link.e2.number))
                    if e.intervals[time].tenants[tenant.id]==[]:
                        del e.intervals[time].tenants[tenant.id]
        link.path = []

    def DropTenant(self,tenant):
        tenant.assigned = False
        for r in tenant.replications:
            self.DropReplica(tenant,r)
        for rl in tenant.replicalinks:
            self.DropReplicaLink(tenant,rl)
        tenant.replications = []
        tenant.replicalinks = []
        for v in tenant.vertices:
            self.DropVertex(tenant,v)
        for e in tenant.edges:
            self.DropLink(tenant,e)
        if tenant in self.assignedTenants:
            self.assignedTenants.remove(tenant) 
        #self.RemoveIntervals(tenant)

    def LoadAllTenants(self, tenants):
        for tenant in tenants:
            if tenant.assigned:
                for v in tenant.vertices:
                    self.AssignVertex(tenant,v,v.resource,t)
                for e in tenant.edges:
                    if e.e1.resource == e.e2.resource:
                        continue
                    self.AssignLink(tenant, e, e.path, t)

    def GenerateTree3(self, params):
        copyNum=params["copyNum"] + 1
        leafwidth = 25
        leafNumber = params["computersNum"]*params["computersNodes"] + params["storagesNum"]*params["storagesNodes"]
        width = leafNumber*leafwidth
        num0 = num1 = num2 = num3 = 0
        for i in range(params["routersNum0"]):
            w = width / params["routersNum0"]/copyNum
            routers0=[]
            for j in range(copyNum):
                r = Router("router_0_"+str(num0), params["routerBandwidth0"])
                r.x = 15 + 0.5*w + num0*w
                r.y = 15
                self.AddVertex(r)
                routers0.append(r)
                num0+=1
            for j in range(params["routerChilds0"]):
                w1 = width / params["routerChilds0"]/params["routersNum0"]/copyNum
                childs1 = []
                for k in range(copyNum):
                    child1 = Router("router_1_"+str(num1), params["routerBandwidth1"])
                    child1.x = 15 + 0.5*w1 + num1*w1
                    child1.y = 15 + 2*leafwidth
                    self.AddVertex(child1)
                    num1 += 1
                    for r in routers0:
                        channel1 = Link(r,child1,params["channelsBandwidth0"])
                        self.AddLink(channel1)
                    childs1.append(child1)
                for k in range(params["computersNodes"]/params["routerChilds0"]/params["routersNum0"]):
                    w2 = width / (params["routerChilds1"]*params["routerChilds0"]*params["routersNum0"])
                    child2 = Router("router_2_"+str(num2), params["routerBandwidth2"])
                    child2.x = 15 + 0.5*w2 + num2*w2
                    child2.y = 15 + 4*leafwidth
                    self.AddVertex(child2)
                    for child1 in childs1:
                        channel2 = Link(child1,child2,params["channelsBandwidth1"])
                        self.AddLink(channel2)
                    for l in range(params["computersNum"]):
                        computer = Computer("computer"+str(num3), params["performance"], params["ram"])
                        computer.x = 15 + 0.5*leafwidth + num3*leafwidth
                        computer.y = 15 + 6*leafwidth
                        self.AddVertex(computer)
                        channel3 = Link(child2,computer,params["computerChannelsBandwidth2"])
                        self.AddLink(channel3)
                        num3+=1
                    num2+=1
                for k in range(params["storagesNodes"]/params["routerChilds0"]/params["routersNum0"]):
                    w2 = width / (params["routerChilds1"]*params["routerChilds0"]*params["routersNum0"])
                    child2 = Router("router_2_"+str(num2), params["routerBandwidth2"])
                    child2.x = 15 + 0.5*w2 + num2*w2
                    child2.y = 15 + 4*leafwidth
                    self.AddVertex(child2)
                    for child1 in childs1:
                        channel2 = Link(child1,child2,params["channelsBandwidth1"])
                        self.AddLink(channel2)
                    for l in range(params["storagesNum"]):
                        storage = Storage("storage"+str(num3),params["capacity"],random.randint(0,params["numTypes"]-1))
                        storage.x = 15 + 0.5*leafwidth + num3*leafwidth
                        storage.y = 15 + 6*leafwidth
                        self.AddVertex(storage)
                        channel3 = Link(child2,storage,params["storageChannelsBandwidth2"])
                        self.AddLink(channel3)
                        num3+=1
                    num2+=1

    def GenerateTree2(self, params):
        copyNum = params["copyNum"] + 1
        leafwidth = 25
        leafNumber = params["computersNum"]*params["computersNodes"] + params["storagesNum"]*params["storagesNodes"]
        width = leafNumber*leafwidth
        num0 = num1 = num2 = 0
        for i in range(params["routersNum0"]):
            w = width / params["routersNum0"]/copyNum
            routers0 = []
            for j in range(copyNum):
                r = Router("router_0_"+str(num0), params["routerBandwidth0"])
                r.x = 15 + 0.5*w + num0*w
                r.y = 15
                self.AddVertex(r)
                routers0.append(r)
                num0+=1
            for k in range(params["computersNodes"]/params["routersNum0"]):
                w2 = width / (params["routerChilds0"]*params["routersNum0"])
                child1 = Router("router_1_"+str(num1), params["routerBandwidth2"])
                child1.x = 15 + 0.5*w2 + num1*w2
                child1.y = 15 + 2*leafwidth
                self.AddVertex(child1)
                for r in routers0:
                    channel1 = Link(child1,r,params["channelsBandwidth1"])
                    self.AddLink(channel1)
                for l in range(params["computersNum"]):
                    computer = Computer("computer"+str(num2), params["performance"], params["ram"])
                    computer.x = 15 + 0.5*leafwidth + num2*leafwidth
                    computer.y = 15 + 4*leafwidth
                    self.AddVertex(computer)
                    channel2 = Link(child1,computer,params["computerChannelsBandwidth2"])
                    self.AddLink(channel2)
                    num2+=1
                num1+=1
            for k in range(params["storagesNodes"]/params["routersNum0"]):
                w2 = width / (params["routerChilds0"]*params["routersNum0"])
                child1 = Router("router_1_"+str(num1), params["routerBandwidth2"])
                child1.x = 15 + 0.5*w2 + num1*w2
                child1.y = 15 + 2*leafwidth
                self.AddVertex(child1)
                for r in routers0:
                    channel1 = Link(child1,r,params["channelsBandwidth1"])
                    self.AddLink(channel1)
                for l in range(params["storagesNum"]):
                    storage = Storage("storage"+str(num2),params["capacity"],random.randint(0,params["numTypes"]-1))
                    storage.x = 15 + 0.5*leafwidth + num2*leafwidth
                    storage.y = 15 + 4*leafwidth
                    self.AddVertex(storage)
                    channel2 = Link(child1,storage,params["storageChannelsBandwidth2"])
                    self.AddLink(channel2)
                    num2+=1
                num1+=1

    def GenerateCommonStructure(self, params):
        copyNum=params["copyNum"] + 1
        leafwidth = 25
        leafNumber = params["computersNum"]*params["computersNodes"] + params["storagesNum"]*params["storagesNodes"]
        width = leafNumber*leafwidth
        rootRouters = []
        for i in range(params["routersNum0"]):
            w = width / params["routersNum0"]
            r = Router("router_0_"+str(i), params["routerBandwidth0"])
            r.x = 15 + 0.5*w + i*w
            r.y = 15
            self.AddVertex(r)
            rootRouters.append(r)
        num1 = num2 = num3 = 0
        for i in range(params["routersNum1"]):
            w1 = width / params["routersNum1"]/copyNum
            childs1 = []
            for j in range(copyNum):
                child1 = Router("router_1_"+str(num1), params["routerBandwidth1"])
                child1.x = 15 + 0.5*w1 + num1*w1
                child1.y = 15 + 2*leafwidth
                self.AddVertex(child1)
                childs1.append(child1)
                num1+=1
            for r in rootRouters:
                for child1 in childs1:
                    channel1 = Link(r,child1,params["channelsBandwidth0"])
                    self.AddLink(channel1)
            for k in range(params["computersNodes"]/params["routersNum1"]):
                w2 = width / (params["routerChilds1"]*params["routersNum1"])
                child2 = Router("router_2_"+str(num2), params["routerBandwidth2"])
                child2.x = 15 + 0.5*w2 + num2*w2
                child2.y = 15 + 4*leafwidth
                self.AddVertex(child2)
                for child1 in childs1:
                    channel2 = Link(child1,child2,params["channelsBandwidth1"])
                    self.AddLink(channel2)
                for l in range(params["computersNum"]):
                    computer = Computer("computer"+str(num3), params["performance"], params["ram"])
                    computer.x = 15 + 0.5*leafwidth + num3*leafwidth
                    computer.y = 15 + 6*leafwidth
                    self.AddVertex(computer)
                    channel3 = Link(child2,computer,params["computerChannelsBandwidth2"])
                    self.AddLink(channel3)
                    num3+=1
                num2+=1
            for k in range(params["storagesNodes"]/params["routersNum1"]):
                w2 = width / (params["routerChilds1"]*params["routersNum1"])
                child2 = Router("router_2_"+str(num2), params["routerBandwidth2"])
                child2.x = 15 + 0.5*w2 + num2*w2
                child2.y = 15 + 4*leafwidth
                self.AddVertex(child2)
                for child1 in childs1:
                    channel2 = Link(child1,child2,params["channelsBandwidth1"])
                    self.AddLink(channel2)
                for l in range(params["storagesNum"]):
                    storage = Storage("storage"+str(num3),params["capacity"],random.randint(0,params["numTypes"]-1))
                    storage.x = 15 + 0.5*leafwidth + num3*leafwidth
                    storage.y = 15 + 6*leafwidth
                    self.AddVertex(storage)
                    channel3 = Link(child2,storage,params["storageChannelsBandwidth2"])
                    self.AddLink(channel3)
                    num3+=1
                num2+=1
