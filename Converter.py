import sys

from DCGUI.Project import *
from Methods.SimulatedAnnealing import *
from Core.ParamFactory import ParamFactory

if __name__ == "__main__":
    if len(sys.argv) == 1:
        print("Usage: python ConsoleRun.py <input> [<output>]")
        sys.exit(1)
    filename = sys.argv[1]
    if len(sys.argv) > 2:
        outfile = sys.argv[2]
    else:
        outfile = filename
    ParamFactory.LoadDir("params")
    proj = Project()
    proj.LoadOld(filename)
    proj.Save(outfile)


