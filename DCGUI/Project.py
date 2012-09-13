from Core.Resources import ResourceGraph
from Core.Demands import Demand

class Project:
    resources = None
    demands = []

    def __init__(self):
        self.resources = ResourceGraph()
        self.demands = []

    def CreateDemand(self):
        d = Demand("")
        self.demands.append(d)
        return d

    def CreateRandomDemand(self, dict):
        d = self.CreateDemand()
        d.id = "Random"
        d.GenerateRandom(dict)
        return d