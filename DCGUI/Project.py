from Core.Resources import ResourceGraph
from Core.Demands import Demand
import xml.dom.minidom

def GetNumber():
    i = 0
    while True:
        i += 1
        yield i

numbers = GetNumber()

class Project:
    resources = None
    demands = []
    name = "test project"

    def __init__(self):
        self.resources = ResourceGraph()
        self.demands = []

    def CreateDemand(self):
        d = Demand("")
        self.demands.append(d)
        return d

    def RemoveDemand(self,d):
        self.demands.remove(d)

    def CreateRandomDemand(self, dict):
        d = self.CreateDemand()
        d.id = "Random_" + str(numbers.next())
        d.GenerateRandom(dict)
        return d

    def Save(self, filename):
        dom = xml.dom.minidom.Document()
        root = dom.createElement("dcxml")
        resgraph = self.resources.CreateXml(dom)
        root.appendChild(resgraph)
        for d in self.demands:
            dem = d.CreateXml(dom)
            root.appendChild(dem)
        dom.appendChild(root)
        f = open(filename, "w")
        f.write(dom.toprettyxml())
        f.close()

    def Load(self, filename):
        f = open(filename, "r")
        dom = xml.dom.minidom.parse(f)
        self.demands = []
        for root in dom.childNodes:
            if root.tagName == "dcxml":
                for node in root.childNodes:
                    if isinstance(node, xml.dom.minidom.Text):
                        continue
                    if node.tagName == "resources":
                        self.resources.LoadFromXmlNode(node)
                    elif node.tagName == "demand":
                        d = self.CreateDemand()
                        d.LoadFromXmlNode(node)
        f.close()