import xml.dom.minidom, copy, os

class Param:
    def __init__(self, name, type, value, minv=0, maxv=0, unit=""):
        self.name = name
        self.type = type
        self.default = value
        self.minv = minv
        self.maxv = maxv
        self.value = self.default
        self.unit = unit

class ParamFactory(object):
    params = {}
    forbiddenlinks = []
    images = {}
    vnfimages = {}

    @staticmethod
    def LoadDir(url):
        ParamFactory.params = {}
        ParamFactory.forbiddenlinks = []
        ParamFactory.images = {}
        ParamFactory.vnfimages = {}
        if os.path.isdir(url):
            for s in os.listdir(url):
                xml = os.path.join(url, s)
                if os.path.isfile(xml):
                    try:
                        ParamFactory.Load(xml)
                    except:
                        print("Can't parse file: ", xml)

    @staticmethod
    def Load(xmlfile):
        #try:
        f = open(xmlfile, "r")
        dom = xml.dom.minidom.parse(f)
        #ParamFactory.params = {}
        for top in dom.childNodes:
            if isinstance(top, xml.dom.minidom.Text) or isinstance(top, xml.dom.minidom.Comment):
                continue
            if top.tagName == "params":
                for node in top.childNodes:
                    if isinstance(node, xml.dom.minidom.Text) or isinstance(node, xml.dom.minidom.Comment):
                        continue
                    if node.tagName == "forbid_link":
                        v1 = node.getAttribute("from")
                        v2 = node.getAttribute("to")
                        type = node.getAttribute("type")
                        if not type:
                            type = "both"
                        ParamFactory.forbiddenlinks.append([v1, v2, type])
                        continue
                    if node.tagName == "image":
                        img = node.getAttribute("id")
                        type = node.getAttribute("type")
                        name = node.getAttribute("name")
                        if type == "vm":
                            ParamFactory.images[name] = img
                        elif type == "vnf":
                            ParamFactory.vnfimages[name] = img
                        else:
                            ParamFactory.images[name] = img
                            ParamFactory.vnfimages[name] = img
                        continue
                    nodetype = node.tagName
                    if not nodetype in ParamFactory.params:
                        ParamFactory.params[nodetype] = []
                    for vertex in node.childNodes:
                        if isinstance(vertex, xml.dom.minidom.Text) or isinstance(vertex, xml.dom.minidom.Comment):
                            continue
                        if vertex.nodeName == "parameter_set":
                            for param in vertex.childNodes:
                                if isinstance(param, xml.dom.minidom.Text) or isinstance(param, xml.dom.minidom.Comment):
                                    continue
                                name = param.getAttribute("parameter_name")
                                type = param.getAttribute("parameter_type")
                                unit = param.getAttribute("parameter_unit")
                                value = param.getAttribute("value_default")
                                minv = param.getAttribute("min")
                                maxv = param.getAttribute("max")
                                if type == "integer":
                                    minv = int(minv)
                                    maxv = int(maxv)
                                if type == "real":
                                    minv = float(minv)
                                    maxv = float(maxv)
                                ParamFactory.params[nodetype].append(Param(name, type, value, minv, maxv, unit))
        f.close()

    @staticmethod
    def Create(type):
        if type in ParamFactory.params:
            lst = copy.deepcopy(ParamFactory.params[type])
            params = {}
            for p in lst:
                params[p.name] = p
            return params
        else:
            return {}
