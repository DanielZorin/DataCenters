class AbstractVertex:
    x = 50
    y = 50
    number = -1

    def __init__(self, id):
        self.id = id

class AbstractGraph:
    def __init__(self):
        self.vertices = []
        self.edges = []

    def AddLink(self, e):
        self.edges.append(e)

    def AddVertex(self, v):
        self.vertices.append(v)
        v.number = len(self.vertices)

    def DeleteVertex(self, v):
        ind = self.vertices.index(v)
        new_edges = []
        for e in self.edges:
            if e.e1 != v and e.e2 != v:
                new_edges.append(e)
            else:
                del e
        self.edges = new_edges
        del self.vertices[ind]

    def DeleteEdge(self, ed):
        new_edges = []
        for e in self.edges:
            if e != ed:
                new_edges.append(e)
            else:
                del e
        self.edges = new_edges

    def FindEdge(self, v1, v2):
        '''Search for a specific edge from v1 to v2. Returns None if the edge doesn't exist'''
        for ver in self.edges:
            if ((ver.e1 == v1) and (ver.e2 == v2)) or ((ver.e1 == v2) and (ver.e2 == v1)):
                    return ver
        return None
    
    def FindAllEdges(self, v1 = None, v2 = None):
        '''Search for all edges where source is v1 and destination is v2. 
        If v1 or v2 is None, it doesn't set any restrictions.
        I.e. FindAllEdges(None, None) returns a list of all edges of the graph'''
        res = []
        for ver in self.edges:
            if (v1 is None) or (ver.e1 == v1) or (ver.e2 == v1):
                if (v2 is None) or (ver.e2 == v2) or (ver.e1 == v2):
                    res.append(ver)
        return res

    def _buildPaths(self):
        toParse = list(self.vertices)
        components = []
        while True:
            if len(toParse) == 0:
                break
            comp = [toParse[0]]
            toParse = toParse[1:]
            while True:
                newcomp = set(comp)
                for v in comp:
                    links = self.FindAllEdges(v1=v)
                    for e in links:
                        if e.e1 != v:
                            newcomp.add(e.e1)
                        if e.e2 != v2:
                            newcomp.add(e.e2)
                if len(newcomp) == len(comp):
                    break
                comp = newcomp
            components.append(list(comp))
            for v in comp:
                if v in toParse:
                    toParse.remove(v)
        self.components = components
        self.compdict = {}
        for v in self.vertices:
            for c in self.components:
                if v in c:
                    self.compdict[v] = c
                    break

    def PathExists(self, v1, v2):
        return self.compdict[v1] == self.compdict[v2]

