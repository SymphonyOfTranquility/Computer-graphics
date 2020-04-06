from Point import *
import numpy as np


class Locus:

    def __init__(self, p=Point()):
        self.point = p
        self.edges = np.empty(0, Edge)


class Edge:

    class Line:

        def __init__(self):
            self.a = 0
            self.b = 0
            self.c = 0

        def set_vals_by_points(self, q, p):
            self.a = p.y - q.y
            self.b = q.x - p.x
            self.c = p.x*q.y - q.x*p.y
            if abs(self.a) >= EPS_CONST and self.a < 0:
                self.a, self.b, self.c = -self.a, -self.b, -self.c

    def __init__(self, center_point=None, locus_a=None, locus_b=None):
        self.left_locus = locus_a
        self.right_locus = locus_b
        self.start = None
        self.end = None
        if locus_a is not None:
            self.direction = Point(locus_b.point.y - locus_a.point.y, locus_a.point.x - locus_b.point.x)
            self.line = None
            if center_point is not None:
                self.line = Edge.Line()
                self.line.set_vals_by_points(center_point, Point(center_point.x + self.direction.x, center_point.y+self.direction.y))
        else:
            self.direction = None
            self.line = None
