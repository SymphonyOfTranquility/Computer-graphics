import enum
import math
from math import atan2

import numpy as np

from EdgeAndLocus import Locus, Edge
from Point import Point, EPS_CONST


class _BeachType(enum.Enum):
    NONE = 1
    EDGE = 2
    ARC = 3


class _EdgeType(enum.Enum):
    NONE = 1
    LEFT = 2
    RIGHT = 3
    VERTICAL = 4


class _BeachLineEdge:

    def __init__(self, edge=None, edge_type=_EdgeType.NONE):
        self.edge = edge
        self.type = edge_type

    def __del__(self):
        self.edge = None
        del self.type


class _TreeItem:

    def __init__(self):
        self.parent = None
        self.children = np.empty(0, _TreeItem)
        self.left = None
        self.right = None
        self.type = _BeachType.NONE
        self.locus = None
        self.edges = None
        self.id = -1

    def set_to_arc(self, locus, locus_id):
        self.type = _BeachType.ARC
        self.locus = locus
        self.id = locus_id

    def set_to_edge(self):
        self.type = _BeachType.EDGE
        self.edges = np.empty(0, _BeachLineEdge)

    def __del__(self):
        self.parent = None
        if self.type == _BeachType.EDGE:
            for edge in self.edges:
                del edge
        self.children = None
        self.locus = None
        self.right = None
        self.left = None
        self.type = None
        self.id = None


class BeachLineTree:

    def __init__(self):
        self._root = None
        self._can_be_crossed = np.empty(0, np.object)
        self._max_id = 0

    # add new arc
    def add(self, locus):
        if self._root is None:
            self._root = _TreeItem()
            self._root.set_to_arc(locus, self._max_id)
            self._max_id += 1
        else:
            self._root = self._rec_add(self._root, locus)

    def _rec_add(self, root, locus):
        if root.type == _BeachType.ARC:  # check if Arc - it's leaf
            parent = root.parent
            if parent is None:  # it's only on the start when self._root is leaf.
                parent = _TreeItem()
                parent.set_to_edge()
                parent.children = np.append(parent.children, root)
                root.parent = parent

            new_edge = self._create_edge(root.locus, locus)
            if len(parent.edges) == 0:  # comment higher
                parent.edges = np.append(parent.edges, new_edge)
            else:  # seek for place to insert new edges
                i = 0
                while i < len(parent.children):
                    if parent.children[i] == root:
                        break
                    i += 1
                parent.edges = np.insert(parent.edges, i, new_edge)

            root_pos = 0
            while root_pos < len(parent.children):  # seek for child in parent
                if parent.children[root_pos] == root:
                    break
                root_pos += 1

            root_pos += 1
            if len(new_edge) == 2:  # create 2 new arcs
                new_center_leaf = _TreeItem()
                new_center_leaf.set_to_arc(locus, self._max_id)
                self._max_id += 1
                new_right_leaf = _TreeItem()
                new_right_leaf.set_to_arc(root.locus, self._max_id)
                self._max_id += 1
                parent.children = np.insert(parent.children, root_pos, [new_center_leaf, new_right_leaf])
                new_center_leaf.parent = parent
                new_right_leaf.parent = parent

                if root.right is not None:
                    root.right.left = new_right_leaf

                new_right_leaf.right = root.right
                new_center_leaf.right = new_right_leaf
                root.right = new_center_leaf
                new_right_leaf.left = new_center_leaf
                new_center_leaf.left = root

                # it's for adding to sweep line queue
                self._can_be_crossed = np.empty(0, np.object)
                if root.left is not None:
                    self._can_be_crossed = np.append(self._can_be_crossed, [(root.left.id, root.left.locus)])
                self._can_be_crossed = np.append(self._can_be_crossed, [(root.id, root.locus),
                                                                        (new_center_leaf.id, new_center_leaf.locus),
                                                                        (new_right_leaf.id, new_right_leaf.locus),
                                                                        ])
                if new_right_leaf.right is not None:
                    self._can_be_crossed = np.append(self._can_be_crossed, [(new_right_leaf.right.id,
                                                                             new_right_leaf.right.locus)])
                    if new_right_leaf.right.right is not None:
                        self._can_be_crossed = np.append(self._can_be_crossed, [(new_right_leaf.right.right.id,
                                                                                 new_right_leaf.right.right.locus)])
                self._can_be_crossed = self._can_be_crossed.reshape(self._can_be_crossed.shape[0] // 2, 2)
            else:  # create one new arc
                new_leaf = _TreeItem()
                new_leaf.set_to_arc(locus, self._max_id)
                self._max_id += 1
                parent.children = np.insert(parent.children, root_pos, [new_leaf])

                if root.right is not None:
                    root.right.left = new_leaf

                new_leaf.right = root.right
                new_leaf.left = root
                root.right = new_leaf

                # it's for adding to sweep line queue
                self._can_be_crossed = np.empty(0, np.object)
                if root.left is not None:
                    self._can_be_crossed = np.append(self._can_be_crossed, [(root.left.id, root.left.locus)])
                self._can_be_crossed = np.append(self._can_be_crossed, [(root.id, root.locus),
                                                                        (new_leaf.id, new_leaf.locus)])
                if new_leaf.right is not None:
                    self._can_be_crossed = np.append(self._can_be_crossed, [(new_leaf.right.id,
                                                                             new_leaf.right.locus)])
                self._can_be_crossed = self._can_be_crossed.reshape(self._can_be_crossed.shape[0] // 2, 2)
            return parent

        else:  # It have to be an edge -> find that child that locus intersects
            update = False
            last_vertical = -1
            count_vert = 0
            ind = -1
            for i in range(len(root.edges)):
                position = self._get_position_depend_on_edge(root.edges[i], locus.point)
                if position == _EdgeType.LEFT:  # on the left of edge
                    if last_vertical == -1:
                        root = self._rec_add(root.children[i], locus)
                    update = True
                    break
                elif position == _EdgeType.VERTICAL:  # is on the edge
                    if root.edges[i].type == _EdgeType.RIGHT:
                        last_vertical = i
                    else:
                        last_vertical = i+1
                    count_vert += 1
                    ind = i
                    update = True

            if count_vert == 1:
                root = self._rec_add(root.children[last_vertical], locus)
            elif count_vert > 1:
                def _get_distance(x, y, point):
                    line = Edge.Line()
                    line.set_vals_by_points(Point(x[0], y[0]), Point(x[1], y[1]))
                    return abs(line.a*point.x + line.b*point.y + line.c)/(line.a**2 + line.b**2)**0.5

                ans = last_vertical
                dist = 10000000000000000
                for i in range(count_vert):
                    x_point = self._get_parabolas_intersection_on_axis_x(root.edges[ind-i], locus.point)
                    if x_point is Point:
                        ans = ind-1
                        break
                    y_point = self._get_parabolas_intersection_on_axis_y(root.edges[ind-i], x_point)
                    if dist < _get_distance(x_point, y_point, locus.point):
                        dist = _get_distance(x_point, y_point, locus.point)
                        ans = ind-i
                root = self._rec_add(root.children[ans], locus)
            elif not update:  # the most right edge
                root = self._rec_add(root.children[-1], locus)
            if len(root.children) > 3:  # size more than 3 -> create new vertex in tree split current
                middle = len(root.edges) // 2
                parent = root.parent
                if parent is None:  # create new root
                    parent = _TreeItem()
                    parent.set_to_edge()
                    parent.children = np.append(parent.children, [root])
                    root.parent = parent

                root_pos = 0
                while root_pos < len(parent.children):
                    if parent.children[root_pos] == root:
                        break
                    root_pos += 1

                # create new sibling
                sibling = _TreeItem()
                sibling.parent = parent
                parent.children = np.insert(parent.children, root_pos + 1, [sibling])
                parent.edges = np.insert(parent.edges, root_pos, [root.edges[middle]])

                sibling.set_to_edge()
                sibling.edges = np.append(sibling.edges, root.edges[(middle + 1):])
                sibling.children = np.append(sibling.children, root.children[(middle + 1):])
                for child in sibling.children:
                    child.parent = sibling
                root.edges = root.edges[:middle]
                root.children = root.children[:(middle + 1)]

                return parent
            else:
                if root.parent is None:
                    return root
                else:
                    return root.parent

    def _create_edge(self, root_locus, new_locus):  # create edge for 2 loci, leaf locus and added locus
        edge_of_beach = np.empty(0, _BeachLineEdge)
        if abs(root_locus.point.y - new_locus.point.y) >= EPS_CONST:
            intersection_point = self._get_line_parabola_intersection(root_locus.point, new_locus.point)
            edge = Edge(intersection_point, root_locus, new_locus)
            edge_of_beach = np.append(edge_of_beach, [_BeachLineEdge(edge, _EdgeType.LEFT),
                                                      _BeachLineEdge(edge, _EdgeType.RIGHT)])
        else:
            intersection_point = Point((root_locus.point.x + new_locus.point.x) / 2,
                                       1.0 / EPS_CONST * abs(root_locus.point.y))
            edge = Edge(intersection_point, root_locus, new_locus)
            edge_of_beach = np.append(edge_of_beach, [_BeachLineEdge(edge, _EdgeType.VERTICAL)])

        root_locus.edges = np.append(root_locus.edges, [edge])
        new_locus.edges = np.append(new_locus.edges, [edge])
        return edge_of_beach

    def _get_line_parabola_intersection(self, focus, locus):
        point = Point()
        point.x = locus.x
        point.y = (locus.x - focus.x)**2/(2.0*(focus.y-locus.y)) + (locus.y + focus.y)/2.0
        return point

    def _get_position_depend_on_edge(self, edge, locus_point):
        x_point = self._get_parabolas_intersection_on_axis_x(edge, locus_point)
        if edge.type == _EdgeType.VERTICAL:
            if locus_point.x > x_point[0]:
                return _EdgeType.RIGHT
            else:
                return _EdgeType.LEFT
        elif isinstance(x_point, Point):
            if abs(locus_point.x-x_point.x) < EPS_CONST:
                return _EdgeType.VERTICAL
            if locus_point.x < x_point.x:
                return _EdgeType.LEFT
            else:
                return _EdgeType.RIGHT
        else:
            x1 = x_point[0]
            x2 = x_point[1]
            if edge.edge.start is not None:
                if edge.type == _EdgeType.LEFT:
                    x2 = min(x2, edge.edge.start.x)
                else:
                    x1 = max(x1, edge.edge.start.x)
            if locus_point.x < x1:
                return _EdgeType.LEFT
            elif x2 < locus_point.x:
                return _EdgeType.RIGHT
            else:
                return _EdgeType.VERTICAL

    def _get_parabolas_intersection_on_axis_x(self, edge, sweep_line):
        if edge.type == _EdgeType.VERTICAL:
            x_point = - edge.edge.line.c / edge.edge.line.a
            return np.array([x_point])
        else:
            point_a = edge.edge.right_locus.point
            point_b = edge.edge.left_locus.point
            if abs(point_a.y - sweep_line.y) < EPS_CONST:
                return point_a
            elif abs(point_b.y - sweep_line.y) < EPS_CONST:
                return point_b

            p1, p2 = (point_a.y - sweep_line.y) / 2.0, (point_b.y - sweep_line.y) / 2.0
            k1, k2 = (point_a.y + sweep_line.y) / 2.0, (point_b.y + sweep_line.y) / 2.0
            h1, h2 = point_a.x, point_b.x

            a = p2 - p1
            b = p2 * h1 - p1 * h2
            c = p2 * h1 * h1 - p1 * h2 * h2 + 4 * p1 * p2 * (k1 - k2)

            x1 = (b - (b * b - a * c) ** 0.5) / a
            x2 = (b + (b * b - a * c) ** 0.5) / a

            try:
                if x1 > x2:
                    x1, x2 = x2, x1
            except:
                print(x1, x2)
                x1 = abs(x1)
                x2 = abs(x2)
            return np.array([x1, x2])

    # delete arc from tree
    def delete(self, loci, sweep_line):
        self._rec_del(self._root, loci, sweep_line)

    def _rec_del(self, root, loci, sweep_line):
        if root.type == _BeachType.ARC:
            temp = root
            upd = False
            for i in range(20):
                if temp is None:
                    break
                if (temp.left is not None) and (temp.right is not None) and \
                        temp.left.id == loci[0][0] and temp.id == loci[1][0] and temp.right.id == loci[2][0]:
                    root = temp
                    upd = True
                    break
                temp = temp.left
            if not upd:
                temp = root
                for i in range(20):
                    if temp is None:
                        break
                    if (temp.left is not None) and (temp.right is not None) and \
                            temp.left.id == loci[0][0] and temp.id == loci[1][0] and temp.right.id == loci[2][0]:
                        root = temp
                        upd = True
                        break
                    temp = temp.right

            if not upd:
                return
            print('success')
            locus = loci[1][1]
            parent = root.parent
            near_left, height_left, pos_left = self._get_near(root, 1)  # move on the left
            near_right, height_right, pos_right = self._get_near(root, -1)  # move on the right
            near_left_locus = root.left.locus
            near_right_locus = root.right.locus

            intersection_point = self._get_parabolas_intersection(near_left_locus, near_right_locus, locus)

            if near_left.edges[pos_left].edge.start is None:
                near_left.edges[pos_left].edge.start = intersection_point
            else:
                near_left.edges[pos_left].edge.end = intersection_point

            if near_right.edges[pos_right].edge.start is None:
                near_right.edges[pos_right].edge.start = intersection_point
            else:
                near_right.edges[pos_right].edge.end = intersection_point

            new_edge = self._create_edge(near_left_locus, near_right_locus)
            new_edge[0].edge.start = intersection_point
            if height_left > height_right:
                top_item = near_left
                top_pos = pos_left
            else:
                top_item = near_right
                top_pos = pos_right

            if len(new_edge) == 1:
                self._insert_edge(top_item, top_pos, new_edge[0])
            else:
                x_point = self._get_parabolas_intersection_on_axis_x(new_edge[0], Point(locus.point.x, locus.point.y - 1000))
                y_point = self._get_parabolas_intersection_on_axis_y(new_edge[0], x_point)
                if self._check_angel(x_point, y_point, intersection_point,
                                     near_left.edges[pos_left], near_right.edges[pos_right]):
                    self._insert_edge(top_item, top_pos, new_edge[0])
                else:
                    self._insert_edge(top_item, top_pos, new_edge[1])

            if height_left > height_right:
                self._del_edge(near_right, root, pos_right)
            else:
                self._del_edge(near_left, root, pos_left)

            left_friend = root.left
            right_friend = root.right

            left_friend.right = right_friend
            right_friend.left = left_friend

            self._can_be_crossed = np.empty(0, np.object)
            if left_friend.left is not None:
                self._can_be_crossed = np.append(self._can_be_crossed, [(left_friend.left.id, left_friend.left.locus)])
            self._can_be_crossed = np.append(self._can_be_crossed, [(left_friend.id, left_friend.locus),
                                                                    (right_friend.id, right_friend.locus)])
            if right_friend.right is not None:
                self._can_be_crossed = np.append(self._can_be_crossed, [(right_friend.right.id,
                                                                         right_friend.right.locus)])
            self._can_be_crossed = self._can_be_crossed.reshape(self._can_be_crossed.shape[0] // 2, 2)
            del root
        else:
            # It have to be an edge -> find that child that locus intersects
            update = False
            point_to_search = sweep_line
            last_vertical = -1
            count_vert = 0
            ind = -1
            for i in range(len(root.edges)):
                position = self._get_position_depend_on_edge(root.edges[i], point_to_search)
                if position == _EdgeType.LEFT:  # on the left of edge
                    if last_vertical == -1:
                        self._rec_del(root.children[i], loci, sweep_line)
                    update = True
                    break
                elif position == _EdgeType.VERTICAL:  # is on the edge
                    if root.edges[i].type == _EdgeType.RIGHT:
                        last_vertical = i
                    else:
                        last_vertical = i + 1
                    ind = i
                    count_vert += 1
                    update = True

            if count_vert == 1:
                self._rec_del(root.children[last_vertical], loci, sweep_line)
            elif count_vert > 1:
                def _get_distance(x, y, point):
                    line = Edge.Line()
                    line.set_vals_by_points(Point(x[0], y[0]), Point(x[1], y[1]))
                    return abs(line.a*point.x + line.b*point.y + line.c)/(line.a**2 + line.b**2)**0.5

                ans = last_vertical
                dist = 10000000000000000
                for i in range(count_vert):
                    x_point = self._get_parabolas_intersection_on_axis_x(root.edges[ind-i], sweep_line)
                    if isinstance(x_point, Point):
                        ans = ind-1
                        break
                    y_point = self._get_parabolas_intersection_on_axis_y(root.edges[ind-i], x_point)
                    if dist < _get_distance(x_point, y_point, sweep_line):
                        dist = _get_distance(x_point, y_point, sweep_line)
                        ans = ind-i
                self._rec_del(root.children[ans], loci, sweep_line)
            elif not update:  # the most right edge
                self._rec_del(root.children[-1], loci, sweep_line)

    def _get_near(self, root, val):
        parent = root.parent
        root_pos = -1
        for i in range(len(parent.children)):
            if parent.children[i] == root:
                root_pos = i
                break

        if root_pos != int(val < 0) * (len(parent.children) - 1):
            return parent, 1, root_pos - int(val > 0)
        else:
            parent, height, pos = self._get_near(parent, val)
            return parent, height + 1, pos

    def _get_parabolas_intersection(self, left_locus, right_locus, middle_locus):
        f_loci, s_loci, t_loci = left_locus.point, middle_locus.point, right_locus.point
        a1, a2 = -2 * (f_loci.x - s_loci.x), -2 * (f_loci.x - t_loci.x)
        b1, b2 = -2 * (f_loci.y - s_loci.y), -2 * (f_loci.y - t_loci.y)
        c1 = f_loci.x ** 2 - s_loci.x ** 2 + f_loci.y ** 2 - s_loci.y ** 2
        c2 = f_loci.x ** 2 - t_loci.x ** 2 + f_loci.y ** 2 - t_loci.y ** 2

        p = (c2 * b1 - c1 * b2) / (b2 * a1 - a2 * b1)
        q = (a2 * c1 - a1 * c2) / (b2 * a1 - a2 * b1)
        return Point(p, q)

    def _insert_edge(self, parent, pos, edge):
        parent.edges[pos] = edge

    def _del_edge(self, parent, child, pos_to_del):
        child_pos = 0
        if parent.children[pos_to_del] == child:
            child_pos = pos_to_del
        elif pos_to_del > 0 and parent.children[pos_to_del - 1] == child:
            child_pos = pos_to_del - 1
        elif pos_to_del + 1 < len(parent.children) and parent.children[pos_to_del + 1] == child:
            child_pos = pos_to_del + 1

        for i in range(pos_to_del, len(parent.edges) - 1, 1):
            parent.edges[i] = parent.edges[i + 1]
        parent.edges = parent.edges[:-1]

        for i in range(child_pos, len(parent.children) - 1, 1):
            parent.children[i] = parent.children[i + 1]
        parent.children = parent.children[:-1]

        if len(parent.edges) == 0:
            grandpa = parent.parent
            last_child = parent.children[0]
            for i in range(len(grandpa.children)):
                if grandpa.children[i] == parent:
                    grandpa.children[i] = last_child
                    last_child.parent = grandpa
                    break

            del parent

    def _get_parabolas_intersection_on_axis_y(self, edge, x_point):
        line = edge.edge.line
        return [(-line.a * x - line.c) / line.b for x in x_point]

    def get_crossed(self):
        return self._can_be_crossed

    def set_ends_to_all(self, x_min, y_min, x_max, y_max):
        self._rec_dfs(self._root, x_min, y_min, x_max, y_max)

    def _rec_dfs(self, root, x_min, y_min, x_max, y_max):
        if root.type == _BeachType.ARC:
            return
        for beach_edge in root.edges:
            edge = beach_edge.edge
            line = edge.line
            if edge.start is None:
                edge.start = Point(-100, 100)
                edge.end = Point(-100, 100)
                """
                if abs(line.a) < EPS_CONST:
                    edge.start = Point(x_min, -line.c/line.b)
                    edge.end = Point(x_max, -line.c/line.b)
                elif abs(line.b) < EPS_CONST:
                    edge.start = Point(-line.c / line.a, y_min)
                    edge.end = Point(-line.c / line.a, y_max)
                else:
                    new_y_min = -(line.c + line.a*x_min)/line.b
                    if y_min <= new_y_min <= y_max:
                        edge.start = Point(x_min, new_y_min)
                    else:
                        if new_y_min <= y_min:
                            new_y = y_min
                        else:
                            new_y = y_max
                        new_x_min = -(line.c + line.b*new_y)/line.a
                        edge.start = Point(new_x_min, new_y)

                    new_y_max = -(line.c + line.a*x_max)/line.b
                    if y_min <= new_y_max <= y_max:
                        edge.end = Point(x_max, new_y_max)
                    else:
                        if new_y_max <= y_min:
                            new_y = y_min
                        else:
                            new_y = y_max
                        new_x_max = -(line.c + line.b*new_y)/line.a
                        edge.end = Point(new_x_max, new_y)"""
            elif edge.end is None:
                # if (edge.start.y < y_min or edge.start.y > y_max or edge.start.x < x_min or edge.start.x > x_max):
                #   edge.end = edge.start
                if beach_edge.type == _EdgeType.LEFT:
                    new_y_min = -(line.c + line.a*x_min)/line.b
                    if x_min > edge.start.x:
                        edge.end = Point(edge.start.x - 10, -(line.c + line.b * (edge.start.x - 10)) / line.a)
                    elif y_min <= new_y_min <= y_max:
                        edge.end = Point(x_min, new_y_min)
                    else:
                        if new_y_min <= y_min:
                            new_y = y_min
                        else:
                            new_y = y_max
                        new_x_min = -(line.c + line.b*new_y)/line.a
                        edge.end = Point(new_x_min, new_y)
                elif beach_edge.type == _EdgeType.RIGHT:
                    new_y_max = -(line.c + line.a*x_max)/line.b
                    if x_max < edge.start.x:
                        edge.end = Point(edge.start.x + 10, -(line.c + line.b * (edge.start.x + 10)) / line.a)
                    elif y_min <= new_y_max <= y_max:
                        edge.end = Point(x_max, new_y_max)
                    else:
                        if new_y_max <= y_min:
                            new_y = y_min
                        else:
                            new_y = y_max
                        new_x_max = -(line.c + line.b*new_y)/line.a
                        edge.end = Point(new_x_max, new_y)
                else:
                    new_x = -line.c/line.a
                    edge.end = Point(new_x, min(y_min, edge.start.y-10))
        for child in root.children:
            self._rec_dfs(child, x_min, y_min, x_max, y_max)

    def _check_angel(self, x_point, y_point, intersection_point, edge1, edge2):
        point1 = edge1.edge.right_locus.point
        point2 = edge1.edge.left_locus.point
        if point1 == edge2.edge.right_locus.point:
            point_mid = point1
            point1 = edge2.edge.left_locus.point
        elif point2 == edge2.edge.right_locus.point:
            point_mid = point2
            point2 = edge2.edge.left_locus.point
        elif point1 == edge2.edge.left_locus.point:
            point_mid = point1
            point1 = edge2.edge.right_locus.point
        else:
            point_mid = point2
            point2 = edge2.edge.right_locus.point
        line = Edge.Line()
        line.set_vals_by_points(point1, point2)
        vor_line = Edge.Line()
        vor_line.set_vals_by_points(Point(x_point[0], y_point[0]), Point(x_point[1], y_point[1]))
        point_line_center = Point((point1.x + point2.x)/2, (point1.y + point2.y)/2)
        if point1.y < point2.y:
            point1, point2 = point2, point1
        angle = self._get_angle(point1, point2, point_mid)
        dist_0 = line.a*x_point[0] + line.b*y_point[0] + line.c
        dist_1 = line.a*x_point[1] + line.b*y_point[1] + line.c
        dist_center = line.a*intersection_point.x + line.b*intersection_point.y + line.c
        if dist_center > 0 and dist_0 < 0 and abs(angle) <= 90 or \
                abs(angle > 90) and dist_center < 0 and dist_0 < 0:
            return True
        else:
            return False

    def _get_angle(self, a, b, u):
        vec_a = Point(a.x - u.x, a.y - u.y)
        vec_b = Point(b.x - u.x, b.y - u.y)
        dot = vec_a.x*vec_b.x + vec_a.y*vec_b.y
        det = vec_a.x*vec_b.y - vec_a.y*vec_b.x
        return atan2(det, dot)*180.0/math.pi

    def __del__(self):
        def rec_del(root):
            for child in root.children:
                rec_del(child)
                del child

        rec_del(self._root)
        del self._root
        self._can_be_crossed = None
