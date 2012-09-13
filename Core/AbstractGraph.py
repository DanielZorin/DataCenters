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
            if (ver.e1 == v1):
                if (ver.e2 == v2):
                    return ver
        return None
    
    def FindAllEdges(self, v1 = None, v2 = None):
        '''Search for all edges where source is v1 and destination is v2. 
        If v1 or v2 is None, it doesn't set any restrictions.
        I.e. FindAllEdges(None, None) returns a list of all edges of the graph'''
        res = []
        for ver in self.edges:
            if (v1 is None) or (ver.e1 == v1):
                if (v2 is None) or (ver.e2 == v2):
                    res.append(ver)
        return res