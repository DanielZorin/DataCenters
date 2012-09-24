class RandomMethod:
    def __init__(self, resources, demands):
        self.resources = resources
        self.demands = demands

    def Run(self):
        g = self.resources.FindPath(self.resources.vertices[5], self.resources.vertices[7])
        while True:
            try:
                p = g.next()
            except StopIteration:
                p = None
                break
            print p