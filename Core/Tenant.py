import xml.dom.minidom, copy, random
from Core.AbstractGraph import AbstractGraph, AbstractVertex

class Storage(AbstractVertex):
    ''' Storage element

    :param id: name
    :param volume: storage size
    :param type: storage type (enum/int)
    '''
    def __init__(self, id):
        AbstractVertex.__init__(self, id)

class VM(AbstractVertex):
    ''' Computer element

    :param id: name
    :param speed: computer performance
    :param ram: RAM capacity
    '''
    def __init__(self, id):
        AbstractVertex.__init__(self, id)
        self.image = ""

class Vnf(AbstractVertex):
    def __init__(self):
        AbstractVertex.__init__(self, "")
        self.type = ""
        self.profile = ""
        self.isservice = False
        self.servicename = ""
        self.username = ""
        self.connectionset = []

class Domain(AbstractVertex):
    ''' Computer element

    :param id: name
    :param speed: computer performance
    :param ram: RAM capacity
    '''
    def __init__(self, id, type):
        AbstractVertex.__init__(self, id)
        self.type = type

class NetElement(AbstractVertex):
    ''' Router/switch element

    :param id: name
    :param capcity: total channel bandwidth
    '''
    def __init__(self, id, type):
        AbstractVertex.__init__(self, id)
        self.type = type
        self.ip = ""
        self.router = False
        self.isservice = False
        self.servicename = ""
        self.provider = ""
        self.port = ""

class Link:
    ''' Channel;

    :param e1: first node
    :param e2: second node
    :param capacity: bandwidth
    '''
    def __init__(self, e1, e2, capacity):
        self.e1 = e1
        self.e2 = e2
        self.port1 = e1.addPort()
        self.port2 = e2.addPort()
        self.capacity = capacity
        self.service = False
        
class Tenant(AbstractGraph):
    ''' Tenant
    '''

    def __init__(self):
        AbstractGraph.__init__(self)
        self.name = ""
        self.type = ""
        self.expiration = ""
        self.assigned = False

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
        nodes = dom.createElement("list_of_nodes")
        for v in self.vertices:
            if isinstance(v, VM):
                tag = dom.createElement("vm")
                tag.setAttribute("vm_name", v.id)
                tag.setAttribute("image_id", v.image)
            elif isinstance(v, Storage):
                tag = dom.createElement("st")
                tag.setAttribute("st_name", v.id)
            elif isinstance(v, NetElement):
                tag = dom.createElement("netelement")
                tag.setAttribute("netelement_name", v.id)
                tag.setAttribute("netelement_type", v.type)
                tag.setAttribute("ip", v.ip)
                tag.setAttribute("is_router", "1" if v.router else "0")
                tag.setAttribute("is_service", "1" if v.isservice else "0")
                tag.setAttribute("service_name", v.servicename)
                tag.setAttribute("provider_name", v.provider)
                tag.setAttribute("external_port", v.port)
            elif isinstance(v, Domain):
                tag = dom.createElement("domain")
                tag.setAttribute("domain_name", v.id)
                tag.setAttribute("commutation_type", v.type)
            elif isinstance(v, Vnf):
                tag = dom.createElement("vnf")
                tag.setAttribute("vnf_name", v.id)
                tag.setAttribute("vnf_type", v.type)
                tag.setAttribute("profile_type", v.profile)
                tag.setAttribute("is_service", "1" if v.isservice else "0")
                tag.setAttribute("service_name", v.servicename)
                tag.setAttribute("user_name", v.username)
                conset = dom.createElement("exported_connection_set")
                conset.setAttribute("number_of_ports", str(len(v.connectionset)))
                for s in v.connectionset:
                    port = dom.createElement("port")
                    port.setAttribute("port_name", s)
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
            nodes.appendChild(tag)
        root.appendChild(nodes)
        links = dom.createElement("list_of_links")
        for v in self.edges:
            tag = dom.createElement("link")
            tag.setAttribute("service", "1" if v.service else "0")
            tag.setAttribute("channel_capacity", str(v.capacity))
            nd = dom.createElement("node1")
            nd.setAttribute("node_name", v.e1.id)
            nd.setAttribute("port_name", v.port1)
            tag.appendChild(nd)
            nd = dom.createElement("node2")
            nd.setAttribute("node_name", v.e2.id)
            nd.setAttribute("port_name", v.port2)
            tag.appendChild(nd)
            tag.appendChild(nd)
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
            if node.tagName == "tenant":
                self.LoadFromXmlNode(node)
        f.close()

    def ParseNodes(root):
        for vertex in root.childNodes:
            service = True if vertex.getAttribute("service") == "1" else False
            ports = []
            params = []
            for v in vertex.childNodes:
                if v.nodeName == "connection_set":
                    for port in v.childNodes:
                        s = port.getAttribute("port_name")
                        ports.append(p)
                if v.nodeName == "parameter_set":
                    pass
            if vertex.nodeName == "vm":
                pass
            elif vertex.nodeName == "st":
                pass
            elif vertex.nodeName == "netelement":
                pass
            elif vertex.nodeName == "vnf":
                pass
            elif vertex.nodeName == "domain":
                pass
            x = vertex.getAttribute("x")
            y = vertex.getAttribute("y")
            if x != '':
                v.x = float(x)
            if y != '':
                v.y = float(y)
            self.vertices.append(v)

    def ParseLinks(root):
        for edge in node.childNodes:
            if edge.nodeName == "link":
                source = int(edge.getAttribute("from"))
                destination = int(edge.getAttribute("to"))
                cap = int(edge.getAttribute("capacity"))
                e = Link(self.vertices[source-1], self.vertices[destination-1], cap)
                self.edges.append(e)

    def LoadFromXmlNode(self, node):
        #Parse vertices
        for vertex in node.childNodes:
            if isinstance(vertex, xml.dom.minidom.Text):
                continue
            if vertex.nodeName == "list_of_links":
                continue
            if vertex.nodeName == "list_of_nodes":
                self.ParseNodes(vertex)
        for vertex in node.childNodes:
            if isinstance(vertex, xml.dom.minidom.Text):
                continue
            if vertex.nodeName == "list_of_links":
                self.ParseLinks(vertex)

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

