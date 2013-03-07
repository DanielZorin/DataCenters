class IdealGenerator:


    bandwidth = 1
    delay = 0

    def __init__(self):
        pass

    @staticmethod
    def GetName():
        return "Ideal"

    def Generate(self, resources):
        pass

    def GetSettings(self):
        # importing here to allow using the class without Qt
        from PyQt4.QtCore import QObject
        class Translator(QObject):
            def __init__(self, parent):
                QObject.__init__(self)
                self.parent = parent
            def getTranslatedSettings(self):
                return [
                [self.tr("Channel bandwidth"), self.parent.bandwidth],
                [self.tr("Delay"), self.parent.delay]
                        ]
        t = Translator(self)
        return t.getTranslatedSettings()

    def UpdateSettings(self, dict):
        # importing here to allow using the class without Qt
        self.bandwidth = dict[0][1]
        self.delay = dict[1][1]

def pluginMain():
    return IdealGenerator