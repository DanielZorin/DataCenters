from PyQt4.QtGui import QMainWindow, QFileDialog, QTreeWidgetItem
from DCGUI.Windows.ui_TestsWindow import Ui_TestsWindow
import os

class TestsWindow(QMainWindow):
    def __init__(self):
        QMainWindow.__init__(self)
        self.ui = Ui_TestsWindow()
        self.ui.setupUi(self)
        self.projects = {}
        self.ui.tests_tabs.setTabEnabled(1, False)

    def Add(self):
        name = str(QFileDialog.getOpenFileName(filter="*.dcxml"))
        if name == None or name == '':
            return
        it = QTreeWidgetItem(self.ui.projects, [os.path.splitext(os.path.split(name)[1])[0]])
        self.projects[it]=name

    def Remove(self):
        item = self.ui.projects.currentItem()
        if (item == None):
            return
        del self.projects[item]
        self.ui.projects.takeTopLevelItem(self.ui.projects.indexOfTopLevelItem(item))
        del item

    def Run(self):
        if self.ui.algorithm.currentIndex() == 0:
            alg = "a"
        elif self.ui.algorithm.currentIndex() == 1:
            alg = "c"
        else:
            alg = "d"
        for p in self.projects.keys():
            os.system("Algorithm\\main.exe " + os.path.relpath(self.projects[p]) + " " + os.path.relpath(self.projects[p]) + " " + alg)
        self.ui.tests_tabs.setTabEnabled(1, True)