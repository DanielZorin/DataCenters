import xml.dom.minidom

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
    def Load(xml):
        f = open(xml, "r")
        dom = xml.dom.minidom.parse(f)
        for node in dom.childNodes:
            if isinstance(node, xml.dom.minidom.Text):
                continue
            nodetype = node.tagName
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
                        params.append(Param(name, type, value, minv, maxv))
        f.close()

    @staticmethod
    def Create(type):
        pass
