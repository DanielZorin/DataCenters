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
        print "The winner is..."
        print self.project.AssignedTenantsNumber(), self.bestProject.AssignedTenantsNumber()

    def StopCondition(self):
        if self.temperature < 20 or self.bestProject.IsAssignmentFull():
            return True
        else:
            return False

    def Step(self):
        self.temperature = self.start_temperature * math.log(1 + self.iteration) / (1 + self.iteration)
        self.iteration += 1
        i = 0
        print self.iteration, self.temperature
        while i < 10:
            #mutation
            '''
            select_one_tenant_element()
            move_it_somewhere()
            '''
            self.GenerateCurProject()
            #compare
            '''
            target function here = 
                sum(difference between used resource and total resource in percent, if used <= total) + 
                sum(difference between used resource and total resource in percent * 10, if used > total)
            '''
            delta = self.prevProject.AssignedTenantsNumber() - self.curProject.AssignedTenantsNumber()
            #delta < 0 if the current situation is better
            p = math.exp(-delta / self.temperature)
            h = random.random()
            #save best assignments
            if (delta <= 0):
                self.CopyCurToPrev()
            elif h > p:
                self.CopyCurToPrev()            
            if self.bestProject.AssignedTenantsNumber() < self.prevProject.AssignedTenantsNumber():
                self.CopyPrevToBest()
            i += 1
    
    def CopyPrevToBest(self):
        self.prevProject.Save("1.xml")
        self.bestProject.Load("1.xml")
        for t in self.bestProject.tenants:
            t.UpdateAssignFlag()

    def CopyCurToBest(self):
        self.curProject.Save("1.xml")
        self.bestProject.Load("1.xml")
        for t in self.bestProject.tenants:
            t.UpdateAssignFlag()
            
    def CopyCurToPrev(self):
        self.curProject.Save("1.xml")
        self.prevProject.Load("1.xml")
        for t in self.prevProject.tenants:
            t.UpdateAssignFlag()
            
    def CopyPrevToCur(self):
        self.prevProject.Save("1.xml")
        self.curProject.Load("1.xml")
        for t in self.curProject.tenants:
            t.UpdateAssignFlag()
            
    def SaveBestProject(self):
        self.bestProject.Save("1.xml")
        self.project.Load("1.xml")
        for t in self.project.tenants:
            t.UpdateAssignFlag()

    def RemoveRandomTenantAssignment(self):
        self.CopyPrevToCur()
        assignedTenants = [t for t in self.curProject.tenants if t.assigned]
        if not assignedTenants:
            return False
        t = random.choice(assignedTenants)
        t.RemoveAssignment()
        t.assigned = False

    def ReassignRandomTenant(self):
        self.CopyPrevToCur()
        assignedTenants = [t for t in self.curProject.tenants if t.assigned]
        if not assignedTenants:
            return False
        t = random.choice(assignedTenants)
        t.RemoveAssignment()
        t.assigned = False
        for ver in t.vertices:
            nodes = [n for n in self.curProject.resources.vertices if n.name == ver.resource]
            if not nodes:
                t.RemoveAssignment()
                t.assigned = False
                return False
            randNode = random.choice(nodes)
            flag = t.Assign(ver, randNode)
            i = 0
            while (flag == False) and (i < 1000):
                randNode = random.choice(nodes)
                flag = t.Assign(ver, randNode)
                i += 1
                if flag == True:
                    break
            if flag == False:
                t.RemoveAssignment()
                t.assigned = False
                break
        if flag == True:
            t.assigned = True
            return True
        else:
            t.RemoveAssignment()
            t.assigned = False
            return False    

    def AddUnassigned(self):
        self.CopyPrevToCur()
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

        print self.AddUnassigned()
        print "     self.prevProject.AssignedTenantsNumber()", self.prevProject.AssignedTenantsNumber()
        self.prevProject.PrintTenantsAssignmentFlags()
        print "     self.curProject.AssignedTenantsNumber()", self.curProject.AssignedTenantsNumber()
        self.curProject.PrintTenantsAssignmentFlags()
        self.CopyCurToBest()
        
        '''
        while time.time() < TIME_LIMIT:
            while not self.StopCondition():
                self.Step()
            if success:
                add_one_more_tenant()
            else:
                remove_last_tenant()
                add_one_more_tenant()
        '''
        ''' 
        notassigned = [t for t in self.project.tenants if not t.assigned]
        ## Replace with time limit
        for i in range(50):
            backup = copy.deepcopy(self.project)
            t = notassigned[random.randint(0, len(notassigned))]
            t.AssignRandomly(self.project.resources)
            number = self.project.AssignedTenantsNumber()
            while not self.StopCondition():
                self.Step()
            number2 = self.project.AssignedTenantsNumber()
            if number2 > number:
                pass
         '''   
       
        #while not self.StopCondition():
            #self.Step()
        # Decode the results here
        self.Finish()
