'''
Created on 06.01.2011

@author: juan
'''

from PyQt4.QtGui import QDialog, QColorDialog
from DCGUI.Windows.ui_SettingsDialog import Ui_SettingsDialog
     
class SettingsDialog(QDialog):
    
    graphVis = {}
    vis = {}
    
    def __init__(self, vi, gv):
        QDialog.__init__(self)
        self.ui = Ui_SettingsDialog()
        self.ui.setupUi(self)
        self.graphVis = gv
        self.vis = vi
        self.ui.axis.setStyleSheet("background-color: " + self.graphVis["axis"].name())
        self.ui.graph.setStyleSheet("background-color: " + self.graphVis["graph"].name())
        self.ui.selection.setStyleSheet("background-color: " + self.vis["selected"].name())
        self.ui.highlight.setStyleSheet("background-color: " + self.vis["selected_demand"].name())
        self.ui.net.setChecked(self.graphVis["net"])
        self.ui.names.setChecked(self.vis["node"])
        self.ui.computer.setChecked(self.vis["computer"])
        self.ui.storage.setChecked(self.vis["storage"])
        self.ui.router.setChecked(self.vis["router"])
        self.ui.channel.setChecked(self.vis["channel"])
                
    def AxisColor(self):
        color = QColorDialog.getColor()
        if (color.isValid()):
            self.ui.axis.setStyleSheet("background-color: " + color.name())
            self.graphVis["axis"] = color
    
    def GraphColor(self):
        color = QColorDialog.getColor()
        if (color.isValid()):
            self.ui.graph.setStyleSheet("background-color: " + color.name())
            self.graphVis["graph"] = color
    
    def SelectionColor(self):
        color = QColorDialog.getColor()
        if (color.isValid()):
            self.ui.selection.setStyleSheet("background-color: " + color.name())
            self.vis["selected"] = color
    
    def HighlightColor(self):
        color = QColorDialog.getColor()
        if (color.isValid()):
            self.ui.highlight.setStyleSheet("background-color: " + color.name())
            self.vis["selected_demand"] = color
                
    def exec_(self):
        self.Backup()  
        QDialog.exec_(self)
        
    def OK(self):
        self.graphVis["net"] = self.ui.net.isChecked()
        self.vis["node"] = self.ui.names.isChecked()
        self.vis["computer"] = self.ui.computer.isChecked()
        self.vis["storage"] = self.ui.storage.isChecked()
        self.vis["router"] = self.ui.router.isChecked()
        self.vis["channel"] = self.ui.channel.isChecked()
        self.accept()
        
    def Cancel(self):
        self.LoadBackup()
        self.reject()

    def Backup(self):
        self.backup = [{}, {}]
        for k in self.graphVis.keys():
            self.backup[0][k] = self.graphVis[k]
        for k in self.vis.keys():
            self.backup[1][k] = self.vis[k]

    def LoadBackup(self):
        for k in self.graphVis.keys():
            self.graphVis[k] = self.backup[0][k]
        for k in self.vis.keys():
            self.vis[k] = self.backup[1][k]
        self.backup = [{}, {}]
