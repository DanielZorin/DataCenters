class Storage:
    def __init__(self, id, volume):
        self.id = id
        self.volume = volume

class Computer:
    def __init__(self, id, speed):
        self.id = id
        self.speed = speed

class Router:
    def __init__(self, id):
        self.id = id

class Link:
    def __init__(self, e1, e2):
        self.e1 = e1
        self.e2 = e2

class ResourceGraph:
    def __init__(self):
        st1 = Storage("1",2)
        st2 = Storage("1",2)
        st3 = Storage("1",2)
        st4 = Storage("1",2)
        c1 = Computer("1",2)
        c2 = Computer("1",2)
        c3 = Computer("1",2)
        c4 = Computer("1",2)
        r1 = Router("1")
        r2 = Router("1")
        r3 = Router("1")
        r4 = Router("1")
        l1 = Link(r1,r2)
        l2 = Link(c1,r4)
        l3 = Link(st2,st3)
        l4 = Link(r3,c4)
        l5 = Link(st1,r3)
        self.vertices = [st1,c1,r1,st2,c2,r2,st3,st4,c3,c4,r3,r4]
        self.edges = [l1,l2,l3,l4,l5]

    def AddLink(self, e):
        self.edges.append(e)

    def AddVertex(self, e):
        self.vertices.append(e)