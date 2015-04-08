import random, copy, math, time
from PyQt4.QtCore import QObject, pyqtSignal
from DCGUI.Project import Project

class SimulatedAnnealing(QObject):
    demand_assigned = pyqtSignal(str)

    def __init__(self, project):
        self.project = project
        self.iteration = 2
        self.temperature = 100
        self.start_temperature = 100
        self.prevProject = Project()
        self.curProject = Project()
        self.bestProject = Project()
        project.Save("1.xml")
        self.prevProject.Load("1.xml")
        self.curProject.Load("1.xml") 
        self.bestProject.Load("1.xml")
   
        #need to save previuos assignments and current assignments
        #also best assignments
        super(SimulatedAnnealing, self).__init__()

    def Init(self):
    #generate PrevProject
        kol = 0
        self.prevProject.resources.ClearAssignments()
        for t in self.prevProject.tenants:
            t.ClearAssignments()
            t.assigned = False
        for v in self.prevProject.resources.vertices:
            v.updateParams()
        for ten in self.prevProject.tenants:
            flag = False
            for ver in ten.vertices:
                nodes = [n for n in self.prevProject.resources.vertices if n.name == ver.resource]
                randNode = random.choice(nodes)
                flag = ten.Assign(ver, randNode)
                i = 0
                while (flag == False) and (i < 100):
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
        kol = self.prevProject.AssignedTenantsNumber()
        return kol
    
    def GenerateCurProject(self):
        kol = 0
        self.curProject.resources.ClearAssignments()
        for t in self.curProject.tenants:
            t.ClearAssignments()
            t.assigned = False
        for v in self.curProject.resources.vertices:
            v.updateParams()
        for ten in self.curProject.tenants:
            flag = False
            for ver in ten.vertices:
                nodes = [n for n in self.curProject.resources.vertices if n.name == ver.resource]
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
        kol = self.curProject.AssignedTenantsNumber()
        return kol
        
    def Finish(self):
        self.project.resources.ClearAssignments()
        for t in self.project.tenants:
            t.ClearAssignments()
            t.assigned = False
        self.SaveBestProject()
        print "Total tenants...", self.project.AssignedTenantsNumber()

    def StopCondition(self):
        if self.temperature < 30 or self.bestProject.IsAssignmentFull():
            return True
        else:
            return False

    def Step(self):
        self.temperature = self.start_temperature * math.log(1 + self.iteration) / (1 + self.iteration)
        self.iteration += 1
        i = 0
        print self.iteration, self.temperature
        while i < 1:
            print "=====", i, "========="
            #mutation
            self.ReassignRandomTenant()
            print "REASSIGNED"
            #target = self.curProject.TargetFunction()
            #print "TARGET", target
            #compare
            '''
            target function here = 
                sum(difference between used resource and total resource in percent, if used <= total) + 
                sum(difference between used resource and total resource in percent * 10, if used > total)
            '''
            #print "Characters1:", self.curProject.BusyNodesNumber(), self.curProject.FreeSpace()
            #print "Characters2:", self.prevProject.BusyNodesNumber(), self.prevProject.FreeSpace()
            #target1 = self.curProject.BusyNodesNumber() + self.curProject.FreeSpace()
            #target2 = self.prevProject.BusyNodesNumber() + self.prevProject.FreeSpace()
            target1 = self.curProject.TargetFunction()
            target2 = self.prevProject.TargetFunction()
            print "target cur, target prev", target1, target2
            delta = target1 - target2
            #delta = self.prevProject.AssignedTenantsNumber() - self.curProject.AssignedTenantsNumber()
            #delta < 0 if the current situation is better
            p = math.exp(-delta / self.temperature)
            h = random.random()
            #save best assignments
            if (delta <= 0):
                self.CopyCurToPrev()
            elif h > p:
                self.CopyCurToPrev()            
            if self.prevProject.CheckAssignments() == True and self.bestProject.AssignedTenantsNumber() < self.prevProject.AssignedTenantsNumber():
                print "COPPPPPPPPPPY"
                print self.bestProject.CheckAssignments()
                self.CopyPrevToBest()
                print self.bestProject.CheckAssignments()
            i += 1
            
    def CopyBestToPrev(self):
        self.prevProject = Project()
        self.bestProject.Save("1.xml")
        self.prevProject.Load("1.xml")
        for t in self.prevProject.tenants:
            t.UpdateAssignFlag()
            
    def CopyBestToCur(self):
        self.curProject = copy.deepcopy(self.bestProject)
        '''
        self.curProject = Project()
        self.bestProject.Save("1.xml")
        self.curProject.Load("1.xml")
        for t in self.curProject.tenants:
            t.UpdateAssignFlag()
        '''
    
    def CopyPrevToBest(self):
        self.bestProject = copy.deepcopy(self.prevProject)
        '''
        self.bestProject = Project()
        self.prevProject.Save("1.xml")
        self.bestProject.Load("1.xml")
        for t in self.bestProject.tenants:
            t.UpdateAssignFlag()
        '''

    def CopyCurToBest(self):
        self.bestProject = copy.deepcopy(self.curProject)
        '''
        self.bestProject = Project()
        self.curProject.Save("1.xml")
        self.bestProject.Load("1.xml")
        for t in self.bestProject.tenants:
            t.UpdateAssignFlag()
        '''
                    
    def CopyCurToPrev(self):
        self.prevProject = copy.deepcopy(self.curProject)
        '''
        self.prevProject = Project()
        self.curProject.Save("1.xml")
        self.prevProject.Load("1.xml")
        for t in self.prevProject.tenants:
            t.UpdateAssignFlag()
        '''
            
    def CopyPrevToCur(self):
        self.curProject = copy.deepcopy(self.prevProject)
        '''
        self.curProject = Project()
        self.prevProject.Save("1.xml")
        self.curProject.Load("1.xml")
        for t in self.curProject.tenants:
            t.UpdateAssignFlag()
        '''
            
    def SaveBestProject(self):
        self.bestProject.Save("1.xml")
        self.project.Load("1.xml")
        for t in self.project.tenants:
            t.UpdateAssignFlag()

    def RemoveRandomTenantAssignment(self):
        self.CopyCurToPrev()
        assignedTenants = [t for t in self.curProject.tenants if t.assigned]
        if not assignedTenants:
            return False
        t = random.choice(assignedTenants)
        t.RemoveAssignment()
        t.assigned = False

    def ReassignRandomTenant(self):
        print "reassign....."
        assignedTenants = [t for t in self.curProject.tenants if t.assigned == True]
        if not assignedTenants:
            print "not assigned Tenants"
            return True
        t = random.choice(assignedTenants)
        t.RemoveAssignment()
        t.assigned = False
        for ver in t.vertices:
            nodes = [n for n in self.curProject.resources.vertices if n.name == ver.resource]
            if not nodes:
                print "no such nodes"
                t.RemoveAssignment()
                t.assigned = False
                return True
            randNode = random.choice(nodes)
            t.NoCheckAssign(ver, randNode)
        t.assigned = True
        return t.CheckAssignments()

    def AddUnassigned(self):
        print "Add unassigned..."
        self.CopyCurToPrev()
        unassignedTenants = [t for t in self.curProject.tenants if not t.assigned]
        if not unassignedTenants:
            return False
        ten = random.choice(unassignedTenants)
        for ver in ten.vertices:
            nodes = [n for n in self.curProject.resources.vertices if n.name == ver.resource]
            if not nodes:
                ten.RemoveAssignment()
                ten.assigned = False
                return False
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
        if flag == True:
            ten.assigned = True
            return True
        else:
            ten.RemoveAssignment()
            ten.assigned = False
            return False    
    
    def PrintState(self):
        print "\n======\nPREVPROJECT"
        self.prevProject.PrintAssignments()
        print len(self.prevProject.resources.vertices), len(self.prevProject.tenants)
        print "======\nCURPROJECT"
        self.curProject.PrintAssignments()
        print len(self.curProject.resources.vertices), len(self.curProject.tenants)        
        
    def Run(self):
        self.project.Reset()
        self.project.resources.ClearAssignments()
        for t in self.project.tenants:
            t.ClearAssignments()
            t.assigned = False
        for v in self.project.resources.vertices:
            v.updateParams()
        # Code the data and create an initial approximation here
        #self.Init()
        self.AddUnassigned()        
        self.CopyCurToBest()
        

        
        #while time.time() < TIME_LIMIT:
        
        for i in range(3):
            self.iteration = 2
            self.temperature = 100
            self.start_temperature = 100
            self.CopyCurToPrev()
            print "cur Project -------------------------"
            self.curProject.PrintAssignments()
            print "prev Project -------------------------"
            self.prevProject.PrintAssignments()
            
            while not self.StopCondition():
                self.Step() #correctly reassign tenants to clear more space for new one
            if self.bestProject.IsAssignmentFull():
                break
            self.CopyBestToCur()
            if self.AddUnassigned() == False:
                print "REMOVE AND ADD"
                self.RemoveRandomTenantAssignment()
                self.AddUnassigned()
            if self.curProject.AssignedTenantsNumber() > self.bestProject.AssignedTenantsNumber():
                self.CopyCurToBest()
            print "                         *****************",self.bestProject.CheckAssignments(), self.bestProject.AssignedTenantsNumber()
        
        ## Replace with time limit
        #while not self.StopCondition():
            #self.Step()
        self.Finish()
