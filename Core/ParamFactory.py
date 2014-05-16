import xml.dom.minidom, copy

class Param:
    def __init__(self, name, type, value, minv, maxv):
        self.name = name
        self.type = type
        self.default = value
        self.minv = minv
        self.maxv = maxv
        self.value = self.default

class ParamFactory(object):
    params = {}

    @staticmethod
    def Load(xmlfile):
        f = open(xmlfile, "r")
        dom = xml.dom.minidom.parse(f)
        for top in dom.childNodes:
            if isinstance(top, xml.dom.minidom.Text):
                continue
            if top.tagName == "params":
                for node in top.childNodes:
                    if isinstance(node, xml.dom.minidom.Text):
                        continue
                    nodetype = node.tagName
                    ParamFactory.params[nodetype] = []
                    for vertex in node.childNodes:
                        if isinstance(vertex, xml.dom.minidom.Text):
                            continue
                        if vertex.nodeName == "parameter_set":
                            for param in vertex.childNodes:
                                if isinstance(param, xml.dom.minidom.Text):
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
