'''
Created on 06.01.2011

@author: juan
'''

from PyQt4.QtGui import QDialog, QColorDialog, QFileDialog
from DCGUI.Windows.ui_SettingsDialog import Ui_SettingsDialog
     
class SettingsDialog(QDialog):
    vis = {}
    
    def __init__(self, vi):
        QDialog.__init__(self)
        self.ui = Ui_SettingsDialog()
        self.ui.setupUi(self)
        self.vis = vi
        self.ui.selection.setStyleSheet("background-color: " + self.vis["selected"].name())
        self.ui.highlight.setStyleSheet("background-color: " + self.vis["selected_tenant"].name())
        self.ui.names.setChecked(self.vis["node"])
        self.ui.computer.setChecked(self.vis["computer"])
        self.ui.storage.setChecked(self.vis["storage"])
        self.ui.router.setChecked(self.vis["router"])
        self.ui.channel.setChecked(self.vis["channel"])
     
    def SelectionColor(self):
        color = QColorDialog.getColor()
        if (color.isValid()):
            self.ui.selection.setStyleSheet("background-color: " + color.name())
            self.vis["selected"] = color
    
    def HighlightColor(self):
        color = QColorDialog.getColor()
        if (color.isValid()):
            self.ui.highlight.setStyleSheet("background-color: " + color.name())
            self.vis["selected_tenant"] = color
                
    def exec_(self):
        self.Backup()  
        QDialog.exec_(self)

    def OpenParams(self):
        name = unicode(QFileDialog.getOpenFileName(filter="*.xml"))
        if name == None or name == '':
            return
        self.ui.params.setText(name)
        
    def OK(self):
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
        for k in self.vis.keys():
            self.backup[1][k] = self.vis[k]

    def LoadBackup(self):
        for k in self.vis.keys():
            self.vis[k] = self.backup[1][k]
        self.backup = [{}, {}]