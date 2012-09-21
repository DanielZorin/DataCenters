class RandomMethod:
    def __init__(self, resources, demands):
        self.resources = resources
        self.demands = demands

    def Run(self):
        g = self.resources.FindPath(self.resources.vertices[0], self.resources.vertices[1])
        p = g.next()
        x = 9