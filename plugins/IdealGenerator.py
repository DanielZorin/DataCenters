import random

class IdealGenerator:
    storagePercent = 50
    number = 10

    def __init__(self):
        pass

    @staticmethod
    def GetName():
        return "Ideal"

    def Generate(self, resources):
        median = self.storagePercent / self.number

    def GetSettings(self):
        # importing here to allow using the class without Qt
        from PyQt4.QtCore import QObject
        class Translator(QObject):
            def __init__(self, parent):
                QObject.__init__(self)
                self.parent = parent
            def getTranslatedSettings(self):
                return [
                [self.tr("Request count"), self.parent.number],
                [self.tr("Storage"), self.parent.storagePercent]
                        ]
        t = Translator(self)
        return t.getTranslatedSettings()

    def UpdateSettings(self, dict):
        # importing here to allow using the class without Qt
        self.number = dict[0][1]
        self.storagePercent = dict[1][1]

def pluginMain():
    return IdealGenerator