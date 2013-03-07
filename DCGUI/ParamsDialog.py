from PyQt4.QtGui import QDialog, QVBoxLayout, QHBoxLayout, QGroupBox, QLabel, QLineEdit, QScrollArea, QWidget, QPushButton, QIntValidator, QDoubleValidator, QComboBox
from PyQt4.QtCore import Qt, QObject, SIGNAL
     
class ParamsDialog(QDialog):
    
    data = {}
    ui = {}
    
    def __init__(self, config, parent):
        QDialog.__init__(self)
        self.setStyleSheet(parent.styleSheet())
        self.setWindowTitle("Method Settings")
        self.data = config             
        self.ui = {}
        self.area = QScrollArea()
        self.widget = QWidget()
        l = self._parseConfig(self.data, self.ui)
        self.widget.setLayout(l)
        self.area.setWidget(self.widget)
        self.area.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOn)
        self.layout = QVBoxLayout()
        self.layout.addWidget(self.area)
        self.buttons = QHBoxLayout()
        self.ok = QPushButton("OK")
        self.cancel = QPushButton("Cancel")
        self.buttons.addWidget(self.ok)
        self.buttons.addWidget(self.cancel)
        self.layout.addLayout(self.buttons)
        self.setLayout(self.layout)
        QObject.connect(self.ok, SIGNAL("clicked()"), self.OK)
        QObject.connect(self.cancel, SIGNAL("clicked()"), self.Cancel)

    def _parseDict(self, config, ui):
        l = QVBoxLayout()
        lst = list(config.keys())
        for k in lst:
            t = type(config[k])
            if t == type(int()):
                hbox = QHBoxLayout()
                label = QLabel(k)
                edit = QLineEdit(str(config[k]))
                val = QIntValidator(self)
                edit.setValidator(val)
                hbox.addWidget(label)
                hbox.addWidget(edit)
                l.addLayout(hbox)
                ui[k] = (edit, label, hbox, val)
            elif t == type(float()):
                hbox = QHBoxLayout()
                label = QLabel(k)
                edit = QLineEdit(str(config[k]))
                val = QDoubleValidator(self)
                edit.setValidator(val)
                hbox.addWidget(label)
                hbox.addWidget(edit)
                l.addLayout(hbox)
                ui[k] = (edit, label, hbox, val)
            elif t == type(list()):
                hbox = QHBoxLayout()
                label = QLabel(k)
                edit = QComboBox()
                i = 0
                for s in config[k][0]:
                    edit.addItem(s)
                    if i == config[k][1]:
                        edit.setCurrentIndex(i)
                    i += 1
                hbox.addWidget(label)
                hbox.addWidget(edit)
                l.addLayout(hbox)
                ui[k] = (edit, label, hbox)
            elif t == type(dict()):
                gbox = QGroupBox()
                gbox.setTitle(k)
                ui[k] = {"gbox":gbox}
                l2 = self._parseDict(config[k], ui[k])
                ui[k]["layout"] = l2
                gbox.setLayout(l2)
                l.addWidget(gbox)
        return l
        
    def _parseConfig(self, config, ui):
        l = QVBoxLayout()
        for k in config:
            t = type(k[1])
            if t == type(int()):
                hbox = QHBoxLayout()
                label = QLabel(k[0])
                edit = QLineEdit(str(k[1]))
                val = QIntValidator(self)
                edit.setValidator(val)
                hbox.addWidget(label)
                hbox.addWidget(edit)
                l.addLayout(hbox)
                ui[k[0]] = (edit, label, hbox, val)
            elif t == type(float()):
                hbox = QHBoxLayout()
                label = QLabel(k[0])
                edit = QLineEdit(str(k[1]))
                val = QDoubleValidator(self)
                edit.setValidator(val)
                hbox.addWidget(label)
                hbox.addWidget(edit)
                l.addLayout(hbox)
                ui[k[0]] = (edit, label, hbox, val)
            elif t == type(list()):
                hbox = QHBoxLayout()
                label = QLabel(k[0])
                edit = QComboBox()
                i = 0
                for s in k[1][0]:
                    edit.addItem(s)
                    if i == k[1][1]:
                        edit.setCurrentIndex(i)
                    i += 1
                hbox.addWidget(label)
                hbox.addWidget(edit)
                l.addLayout(hbox)
                ui[k[0]] = (edit, label, hbox)
            elif t == type(dict()):
                gbox = QGroupBox()
                gbox.setTitle(k[0])
                ui[k[0]] = {"gbox":gbox}
                l2 = self._parseDict(k[1], ui[k[0]])
                ui[k[0]]["layout"] = l2
                gbox.setLayout(l2)
                l.addWidget(gbox)
        return l

    def _getDict(self, config, ui):
        res = {}
        for k in config.keys():
            t = type(config[k])
            if t == type(int()):
                res[k] = int(ui[k][0].text())
            elif t == type(float()):
                res[k] = float(ui[k][0].text())
            elif t == type(list()):
                lst = []
                i = 0
                cur = 0
                while i < ui[k][0].count():
                    s = ui[k][0].itemText(i)
                    if s == ui[k][0].currentText():
                        cur = i
                    lst.append(s)
                    i += 1
                res[k] = [lst, cur]
            elif t == type(dict()):
                res[k] = self._getDict(config[k], ui[k])
        return res
    
    def _getData(self, config, ui):
        res = []
        for k in config:
            t = type(k[1])
            if t == type(int()):
                res.append([k[0], int(ui[k[0]][0].text())])
            elif t == type(float()):
                res.append([k[0], float(ui[k[0]][0].text())])
            elif t == type(list()):
                lst = []
                i = 0
                cur = 0
                while i < ui[k[0]][0].count():
                    s = ui[k[0]][0].itemText(i)
                    if s == ui[k[0]][0].currentText():
                        cur = i
                    lst.append(s)
                    i += 1
                res.append([k[0], [lst, cur]])
            elif t == type(dict()):
                res.append([k[0], self._getDict(k[1], ui[k[0]])])
        return res
    
    def OK(self):
        self.data = self._getData(self.data, self.ui)
        self.accept()
    
    def Cancel(self):
        self.reject()
