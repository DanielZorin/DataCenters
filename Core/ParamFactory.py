import xml.dom.minidom, copy

class Param:
    def __init__(self, name, type, value, minv=0, maxv=0):
        self.name = name
        self.type = type
        self.default = value
        self.minv = minv
        self.maxv = maxv
        self.value = self.default

class ParamFactory(object):
    params = {}
    forbiddenlinks = []

    @staticmethod
    def Load(xmlfile):
        #try:
        f = open(xmlfile, "r")
        dom = xml.dom.minidom.parse(f)
        ParamFactory.params = {}
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
                        ParamFactory.forbiddenlinks.append([v1, v2])
                        continue
                    nodetype = node.tagName
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
                                value = param.getAttribute("value_default")
                                minv = param.getAttribute("min")
                                maxv = param.getAttribute("max")
                                ParamFactory.params[nodetype].append(Param(name, type, value, minv, maxv))
        f.close()

    @staticmethod
    def Create(type):
        if type in ParamFactory.params:
            return copy.deepcopy(ParamFactory.params[type])
        else:
            return []
