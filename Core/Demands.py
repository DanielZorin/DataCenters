class VM:
    number = -1
    def __init__(self, id, speed):
        self.id = id
        self.speed = speed

class DemandStorage:
    number = -1
    def __init__(self, id, volume, type):
        self.id = id
        self.volume = volume
        self.type = type

class DemandLink:
    def __init__(self, e1, e2):
        self.e1 = e1
        self.e2 = e2

class Demand:
    def __init__(self, id):
        self.id = id
        self.vertices = []
        self.edges = []

    def AddLink(self, e):
        self.edges.append(e)

    def AddVertex(self, v):
        self.vertices.append(v)
        v.number = len(self.vertices)

    def ExportToXml(self):
        dom = xml.dom.minidom.Document()
        root = dom.createElement("demand")
        root.setAttribute("id", "0")
        dom.appendChild(root)
        for v in self.vertices:
            if isinstance(v, VM):
                tag = dom.createElement("vm")
                tag.setAttribute("speed", v.speed)
            elif isinstance(v, DemandStorage):
                tag = dom.createElement("storage")
                tag.setAttribute("volume", v.volume)
            tag.setAttribute("number", v.number)
            tag.setAttribute("name", v.name)
            root.appendChild(tag)
        for v in self.edges:
            tag = dom.createElement("link")
            tag.setAttribute("from", v.e1.number)
            tag.setAttribute("to", v.e2.number)
            root.appendChild(tag)
        return dom.toprettyxml()

    def GenerateRandom(self):
        pass


