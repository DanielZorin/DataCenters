import sys

from DCGUI.Project import *
from Methods.SimulatedAnnealing import *

if __name__ == "__main__":
    if len(sys.argv) == 1:
        print("Usage: python ConsoleRun.py <input> [<output>]")
        sys.exit(1)
    filename = sys.argv[1]
    if len(sys.argv) > 2:
        outfile = sys.argv[2]
    else:
        outfile = filename
    proj = Project()
    proj.Load(filename)
    alg = SimulatedAnnealing(proj)
    alg.Run()
    proj.resources.print_all()
    proj.Save(outfile)


