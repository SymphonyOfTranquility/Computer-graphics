import heapq
import enum
import numpy as np

from BeachLineTree import BeachLineTree
from EdgeAndLocus import Locus, Edge
from Point import *


class _EventType(enum.Enum):
    Empty = 1
    NewLocus = 3
    DeleteLocus = 2


class _SweepEvent:

    def __init__(self, event_type=_EventType.Empty, locus=None, time=-1):
        self.type = event_type
        self.locus = locus
        self.loci = None
        self.time = time

    def __lt__(self, other):
        if abs(self.locus.point.y - other.locus.point.y) < EPS_CONST:
            if abs(self.locus.point.x - other.locus.point.x) < EPS_CONST:
                if self.type == _EventType.NewLocus:
                    return True
                else:
                    return False
            elif self.locus.point.x < other.locus.point.x:
                return True
            elif self.locus.point.x < other.locus.point.x:
                return False
        elif self.locus.point.y > other.locus.point.y:
            return True
        else:
            return False


class _PriorityQueue:
    def __init__(self):
        self.elements = []

    def empty(self):
        return len(self.elements) == 0

    def push(self, item):
        heapq.heappush(self.elements, item)

    def top(self):
        return self.elements[0]

    def pop(self):
        heapq.heappop(self.elements)


class Voroniy:

    def __init__(self):
        self.points = np.empty(0, Point)
        self.loci = np.empty(0, Locus)
        self.points_number = 0

    def set_points(self, points):
        self.points = points
        self.points_number = len(points)

    def _check_on_1_line(self, loci, i):
        f_loci, s_loci, t_loci = loci[i][1].point, loci[i+1][1].point, loci[i+2][1].point
        return abs((f_loci.x - t_loci.x)*(s_loci.y - t_loci.y) - (f_loci.y - t_loci.y)*(s_loci.x - t_loci.x)) < EPS_CONST

    def _get_bottom_circle_dot(self, loci, i):
        f_loci, s_loci, t_loci = loci[i][1].point, loci[i + 1][1].point, loci[i + 2][1].point
        a1, a2 = -2*(f_loci.x - s_loci.x), -2*(f_loci.x - t_loci.x)
        b1, b2 = -2*(f_loci.y - s_loci.y), -2*(f_loci.y - t_loci.y)
        c1 = f_loci.x**2 - s_loci.x**2 + f_loci.y**2 - s_loci.y**2
        c2 = f_loci.x**2 - t_loci.x**2 + f_loci.y**2 - t_loci.y**2

        p = (c2*b1 - c1*b2)/(b2*a1 - a2*b1)
        q = (a2*c1 - a1*c2)/(b2*a1 - a2*b1)
        r = ((s_loci.x - p)**2 + (s_loci.y - q)**2)**0.5
        return Point(p, q-r)

    def _dictance(self, x1, y1, x2, y2):
        return ((x1-x2)**2 + (y1-y2)**2)**0.5

    def _check_order(self, loci, i):
        f_loci, s_loci, t_loci = loci[i][1].point, loci[i + 1][1].point, loci[i + 2][1].point
        if f_loci < s_loci:
            f_loci, s_loci = s_loci, f_loci
        if f_loci < t_loci:
            f_loci, t_loci = t_loci, f_loci
        if s_loci < t_loci:
            t_loci, s_loci = s_loci, t_loci

        if abs(t_loci.y - s_loci.y) < EPS_CONST:
            if t_loci.x < s_loci.x:
                return loci[i][1].point == t_loci and loci[i+1][1].point == f_loci and loci[i+2][1].point == s_loci
            else:
                return loci[i][1].point == s_loci and loci[i + 1][1].point == f_loci and loci[i + 2][1].point == t_loci

        p_f, p_s = (f_loci.y - t_loci.y)/2, (s_loci.y - t_loci.y)/2
        k_f, k_s = (f_loci.y + t_loci.y)/2, (s_loci.y + t_loci.y)/2
        h_f, h_s = f_loci.x, s_loci.x

        y_f = (t_loci.x - h_f)**2/(4*p_f) + k_f
        y_s = (t_loci.x - h_s)**2/(4*p_s) + k_s

        if abs(y_f - y_s) < EPS_CONST:
            return loci[i+1][1].point != t_loci
        elif y_f < y_s:
            if t_loci.x < s_loci.x:
                return loci[i][1].point == t_loci and loci[i + 1][1].point == f_loci and loci[i + 2][1].point == s_loci
            else:
                return loci[i][1].point == s_loci and loci[i + 1][1].point == f_loci and loci[i + 2][1].point == t_loci
        else:
            a = p_f - p_s
            b = p_f * h_s - p_s * h_f
            c = p_f * h_s**2 - p_s * h_f**2 + 4 * p_s * p_f * (k_s - k_f)

            if abs(a) < EPS_CONST:
                if f_loci.x < s_loci.x:
                    return loci[i][1].point == f_loci and loci[i+1][1].point == s_loci and loci[i+2][1].point == t_loci
                else:
                    return loci[i][1].point == t_loci and loci[i+1][1].point == s_loci and loci[i+2][1].point == f_loci

            x1 = (b - (b * b - a * c) ** 0.5) / a
            x2 = (b + (b * b - a * c) ** 0.5) / a
            y1 = (x1 - h_f)**2/(4*p_f) + k_f
            y2 = (x2 - h_f)**2/(4*p_f) + k_f
            y_div = (y1 - y2)/(x1-x2)*(t_loci.x - x1) + y1

            dict_to_low = self._dictance(x1, y1, t_loci.x, y_div)
            dict_to_high = self._dictance(x2, y2, t_loci.x, y_div)
            if abs(dict_to_low - dict_to_high) < EPS_CONST:
                return loci[i][1] == s_loci
            elif dict_to_low < dict_to_high:
                return loci[i][1].point == f_loci and loci[i+1][1].point == s_loci and loci[i+2][1].point == t_loci
            else:
                return loci[i][1].point == t_loci and loci[i + 1][1].point == s_loci and loci[i + 2][1].point == f_loci

    def fortune_algorithm(self):
        self.loci = np.empty(0, Locus)
        queue = _PriorityQueue()
        for i in range(self.points_number):
            new_locus = Locus(self.points[i])
            self.loci = np.append(self.loci, [new_locus])
            event = _SweepEvent(_EventType.NewLocus, new_locus, 0)
            queue.push(event)

        tree = BeachLineTree()
        time = 1
        prev_line = queue.top().locus.point.y + 10
        while not queue.empty():
            top = queue.top()
            queue.pop()
            print(top.locus.point.x, top.locus.point.y, top.type)
            if top.type == _EventType.NewLocus:
                tree.add(top.locus)
            else:
                tree.delete(top.loci, Point(top.locus.point.x, (top.locus.point.y+prev_line)/2.0))

            loci_to_check = tree.get_crossed()
            if len(loci_to_check) >= 3:
                for i in range(len(loci_to_check)-2):
                    if not self._check_on_1_line(loci_to_check, i) and self._check_order(loci_to_check, i):
                        circle_dot = self._get_bottom_circle_dot(loci_to_check, i)
                        locus = Locus(circle_dot)
                        event = _SweepEvent(_EventType.DeleteLocus, locus, time)
                        event.loci = loci_to_check[i:i+3]
                        queue.push(event)
            self.output()
            time += 1
            if abs(prev_line - top.locus.point.y) > EPS_CONST:
                prev_line = top.locus.point.y
        x_min, y_min, x_max, y_max = -1, -1, 8, 8
        tree.set_ends_to_all(x_min, y_min, x_max, y_max)
        for locus in self.loci:
            for edge in locus.edges:
                if edge.end is None:
                    new_x = -edge.line.c/edge.line.a
                    edge.end = Point(new_x, y_max)

    def output(self):
        for locus in self.loci:
            print(locus.point.x, locus.point.y)
            for edge in locus.edges:
                print("\te:")
                if edge.start is not None:
                    print('\t\tStart is', edge.start.x, edge.start.y)

                if edge.end is not None:
                    print('\t\tEnd is', edge.end.x, edge.end.y)

                print('\t\tLine:', edge.line.a, edge.line.b, edge.line.c)
        print()


vor = Voroniy()
points = [Point(1, 2), Point(3, 2), Point(3, 0), Point(3, 3), Point(5, 3)]
vor.set_points(points)
vor.fortune_algorithm()
vor.output()
