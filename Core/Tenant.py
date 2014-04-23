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
        self.providername = ""
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
                tag.appendChild(name)
            elif isinstance(v, Domain):
                tag = dom.createElement("domain")
                tag.setAttribute("domain_name", v.id)
                tag.setAttribute("commutation_type", v.type)
            elif isinstance(v, NetService):
                tag = dom.createElement("vnf")
                tag.setAttribute("vnf_name", v.id)
                tag.setAttribute("vnf_type", v.type)
                tag.setAttribute("profile_type", v.profile)
                tag.setAttribute("is_service", "1" if v.isservice else "0")
                tag.setAttribute("service_name", v.servicename)
                tag.setAttribute("user_name", v.provider)
                tag.setAttribute("external_port", v.port)
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
        for v in self.edges:
            tag = dom.createElement("link")
            tag.setAttribute("created_at", v.created)
            tag.setAttribute("updated_at", v.updated)
            tag.setAttribute("deleted_at", v.deleted)
            tag.setAttribute("deleted", "1" if v.deleteFlag else "0")
            tag.setAttribute("service", "1" if v.service else "0")
            lnk = dom.createElement("link_name")
            nd = dom.createElement("node_name")
            nd.setAttribute("port_name", v.e1[1])
            val = dom.createTextNode(str(self.vertices.index(v.e1[0])))
            nd.appendChild(val)
            lnk.appendChild(nd)
            nd = dom.createElement("node_name")
            nd.setAttribute("port_name", v.e2[1])
            val = dom.createTextNode(str(self.vertices.index(v.e2[0])))
            nd.appendChild(val)
            lnk.appendChild(nd)
            tag.appendChild(lnk)
            name = dom.createElement("channel_capacity")
            nd = dom.createTextNode(str(v.capacity))
            name.appendChild(nd)
            tag.appendChild(name)
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
            if node.tagName == "tenant":
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
                ram = int(vertex.getAttribute("ramcapacity")) if vertex.hasAttribute("ramcapacity") else 0
                v = Computer(name, speed, ram)
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

