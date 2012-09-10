from PyQt4.QtGui import QMainWindow
from DCGUI.Windows.ui_MainWindow import Ui_MainWindow

class MainWindow(QMainWindow):
    
    def __init__(self):
        QMainWindow.__init__(self)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)


    def NewProject(self):
        pass
    
    def OpenProject(self):
        pass
        
    def OpenProjectFromFile(self, name):
        pass
    
    def SaveProject(self):
        pass
    
    def SaveProjectAs(self):
        pass

    def Run(self):
        pass

    def Settings(self):
        pass

    def EditProgram(self):
        pass

    def About(self):
        pass

    def Exit(self):
        pass

