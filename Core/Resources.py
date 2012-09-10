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
        self.vertices = []
        self.edges = []

    def AddLink(self, e):
        self.edges.append(e)

    def AddVertex(self, e):
        self.vertices.append(e)