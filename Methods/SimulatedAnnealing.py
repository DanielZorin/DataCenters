import random, copy
from PyQt4.QtCore import QObject, pyqtSignal


class SimulatedAnnealing(QObject):
    demand_assigned = pyqtSignal(str)

    def __init__(self, project):
        self.project = project
        self.iteration = 0
        super(SimulatedAnnealing, self).__init__()

    def Init(self):
        pass

    def Finish(self):
        pass

    def StopCondition(self):
        if self.iteration > 1000:
            return True
        else:
            return False

    def Step(self):
        pass

    def Run(self):
        # Code the data and create an initial approximation here
        self.Init()
        while not self.StopCondition():
            self.Step()
        # Decode the results here
        self.Finish()