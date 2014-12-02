import random, copy
from PyQt4.QtCore import QObject, pyqtSignal


class SimulatedAnnealing(QObject):
    demand_assigned = pyqtSignal(str)

    def __init__(self, project):
        self.project = project
        self.iteration = 0
        super(SimulatedAnnealing, self).__init__()

    def Init(self):
		for t in self.project.tenants:
			for v in t.vertices:
				nodes = [v0 for v0 in self.project.resources.vertices if type(v0) == type(v)]
				print nodes, type(v), self.project.resources.vertices
				randNode = random.choice(nodes)
				t.Assign(v, randNode)
		        

    def Finish(self):
        pass

    def StopCondition(self):
        if self.iteration > 1000:
            return True
        else:
            return False

    def Step(self):
        self.iteration += 1

    def Run(self):
        # Code the data and create an initial approximation here
        self.Init()
        while not self.StopCondition():
            self.Step()
        # Decode the results here
        self.Finish()
