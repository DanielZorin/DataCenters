import random, copy
from PyQt4.QtCore import QObject, pyqtSignal


class SimulatedAnnealing(QObject):
    demand_assigned = pyqtSignal(str)

    def __init__(self, project):
        self.project = project
        self.iteration = 0
        super(SimulatedAnnealing, self).__init__()

    def Init(self):
        for ten in self.project.tenants:
            for ver in ten.vertices:
                nodes = [n for n in self.project.resources.vertices if n.__class__.__name__ == ver.__class__.__name__]
                #print "_______", nodes, type(v), self.project.resources.vertices
                randNode = random.choice(nodes)
                flag = ten.Assign(ver, randNode)
                while flag == False:
					randNode = random.choice(nodes)
					flag = ten.Assign(ver, randNode)
                if flag == True:
					print "Assigned!"

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
