import random, copy, math
from PyQt4.QtCore import QObject, pyqtSignal
from DCGUI.Project import Project


class SimulatedAnnealing(QObject):
    demand_assigned = pyqtSignal(str)

    def __init__(self, project):
        self.project = project
        self.iteration = 2
        self.temperature = 100
        self.start_temperature = 100
        self.prevProject = copy.deepcopy(project)
        self.prevProject.resources = copy.deepcopy(project.resources)
        self.prevProject.tenants = copy.deepcopy(project.tenants)
        
        self.curProject = copy.deepcopy(project)
        self.curProject.resources = copy.deepcopy(project.resources)
        self.curProject.tenants = copy.deepcopy(project.tenants)
        
        self.bestProject = copy.deepcopy(project)
        self.bestProject.resources = copy.deepcopy(project.resources)
        self.bestProject.tenants = copy.deepcopy(project.tenants)
        #need to save previuos assignments and current assignments
        #also best assignments
        super(SimulatedAnnealing, self).__init__()

    def Init(self):
        self.prevProject.resources.ClearAssignments()
        for t in self.prevProject.tenants:
            t.ClearAssignments()
            t.assigned = False
        for v in self.prevProject.resources.vertices:
            v.updateParams()
        for ten in self.prevProject.tenants:
            flag = False
            for ver in ten.vertices:
                nodes = [n for n in self.prevProject.resources.vertices if n.__class__.__name__ == ver.__class__.__name__]
                isRandAssigned = random.randint(0, len(nodes));
                if isRandAssigned == 0:
                    flag = False
                    ten.RemoveAssignment()
                    ten.assigned = False
                    break 
                randNode = random.choice(nodes)
                flag = ten.Assign(ver, randNode)
                i = 0
                while (flag == False) and (i < 1000):
                    randNode = random.choice(nodes)
                    flag = ten.Assign(ver, randNode)
                    i += 1
                    if flag == True:
                        break
                if flag == False:
                    ten.RemoveAssignment()
                    ten.assigned = False
                    break
                    #need to observe all previous vertices and leave them unassigned
            if flag == True:
                ten.assigned = True
                print "assigned"
            else:
                print "not assigned"
        #self.prevProject = copy.deepcopy(self.project)
        #self.bestProject = copy.deepcopy(self.prevProject)
        print "     self.project.AssignedTenantsNumber()", self.project.AssignedTenantsNumber()
        print "     self.prevProject.AssignedTenantsNumber()", self.prevProject.AssignedTenantsNumber()
    
    def GenerateCurProject(self):
        self.project.resources.ClearAssignments()
        for t in self.project.tenants:
            t.ClearAssignments()
            t.assigned = False
        for v in self.project.resources.vertices:
            v.updateParams()
        for ten in self.project.tenants:
            flag = False
            for ver in ten.vertices:
                nodes = [n for n in self.project.resources.vertices if n.__class__.__name__ == ver.__class__.__name__]
                isRandAssigned = random.randint(0, len(nodes));
                if isRandAssigned == 0:
                    flag = False
                    ten.RemoveAssignment()
                    ten.assigned = False
                    break 
                randNode = random.choice(nodes)
                flag = ten.Assign(ver, randNode)
                i = 0
                while (flag == False) and (i < 1000):
                    randNode = random.choice(nodes)
                    flag = ten.Assign(ver, randNode)
                    i += 1
                    if flag == True:
                        break
                if flag == False:
                    ten.RemoveAssignment()
                    ten.assigned = False
                    break
                    #need to observe all previous vertices and leave them unassigned
            if flag == True:
                ten.assigned = True
                print "assigned"
            else:
                print "not assigned"
        self.curProject = copy.deepcopy(self.project)
        if self.curProject.AssignedTenantsNumber() > self.bestProject.AssignedTenantsNumber():
            self.bestProject.resources.ClearAssignments()
            for t in self.bestProject.tenants:
                t.ClearAssignments()
                t.assigned = False
            self.bestProject = copy.deepcopy(self.curProject)
        print "     self.curproject.AssignedTenantsNumber()", self.curProject.AssignedTenantsNumber()
        
    def Finish(self):
        self.project.resources.ClearAssignments()
        for t in self.project.tenants:
            t.ClearAssignments()
            t.assigned = False
        self.project = copy.deepcopy(self.bestProject)
        print "The winner is..."
        print self.project.AssignedTenantsNumber(), self.bestProject.AssignedTenantsNumber()

    def StopCondition(self):
        if self.temperature < 20 or self.project.IsAssignmentFull():
            return True
        else:
            return False

    def Step(self):
        self.temperature = self.start_temperature * math.log(1 + self.iteration) / (1 + self.iteration)
        self.iteration += 1
        i = 0
        print self.iteration, self.temperature
        while i < 1:
            #mutation
            self.GenerateCurProject()
            #compare
            delta = self.prevProject.AssignedTenantsNumber() - self.curProject.AssignedTenantsNumber()
            #delta < 0 if the current situation is better
            p = math.exp(-delta / self.temperature)
            h = random.random()
            #save best assignments
            if (delta <= 0):
                self.prevProject = copy.deepcopy(self.curProject)
            elif h > p:
                self.prevProject = copy.deepcopy(self.curProject)
            
            if self.bestProject.AssignedTenantsNumber() < self.prevProject.AssignedTenantsNumber():
                self.bestProject = copy.deepcopy(self.prevProject)
            i += 1

    def Run(self):
        #self.project.Reset()
        self.project.resources.ClearAssignments()
        for t in self.project.tenants:
            t.ClearAssignments()
            t.assigned = False
        for v in self.project.resources.vertices:
            v.updateParams()
        # Code the data and create an initial approximation here
        self.Init()
        #self.GenerateCurProject()
        self.project = copy.deepcopy(self.prevProject)
        self.project.resources = copy.deepcopy(self.prevProject.resources)
        self.project.tenants = copy.deepcopy(self.prevProject.tenants)
        
        print self.project.AssignedTenantsNumber()
        #while not self.StopCondition():
            #self.Step()
        # Decode the results here
        #self.Finish()
