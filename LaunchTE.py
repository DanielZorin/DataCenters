from PyQt4 import QtGui
import sys

from DCGUI.TenantEditor import TenantEditor

if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    window = TenantEditor()
    window.show()
    sys.exit(app.exec_())

