class AbstractVertex:
    ''' Represents a graph vertex.
        
    :param id: name of the vertex
    '''
    
    x = 50
    ''' X coordinate on the canvas'''

    y = 50
    ''' Y coordinate on the canvas'''

    number = -1

    def __init__(self, id, created="", updated="", deleted="", deleteFlag=False, service=False):
        self.id = id
        self.created = created
        self.updated = updated
        self.deleted = deleted
        self.deleteFlag = deleteFlag
        self.service = service
        self.params = []
        self.ports = []

    def addPort(self):
        s = self.id + "_port_" + str(len(self.ports))
        self.ports.append(s)
        return s

class Param:
    def __init__(self, name, type, value):
        self.name = name
        self.type = type
        self.value = value
        
class AbstractGraph:
    ''' Represents a graph with vertices and edges'''
    def __init__(self):
        self.vertices = []
        self.edges = []

    def AddLink(self, e):
        ''' Add edge'''
        self.edges.append(e)

    def AddVertex(self, v):
        ''' Add vertex'''
        self.vertices.append(v)
        v.number = len(self.vertices)

    def DeleteVertex(self, v):
        ''' Delete vertex and all edges incident to it'''
        ind = self.vertices.index(v)
        new_edges = []
        for e in self.edges:
            if e.e1 != v and e.e2 != v:
                new_edges.append(e)
            else:
                del e
        self.edges = new_edges
        for v in self.vertices[ind:]:
            v.number -= 1
        del self.vertices[ind]

    def DeleteEdge(self, ed):
        ''' Delete edge'''
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
        ''' Searches all linked components of the graph (self.components)
            and constructs a dictionary (self.compdict) vertex -> component
        '''
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
                        if e.e2 != v:
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
        ''' Check that there is a path between v1 and v2'''
        return self.compdict[v1] == self.compdict[v2]

