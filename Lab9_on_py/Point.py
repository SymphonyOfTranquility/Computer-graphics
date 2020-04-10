EPS_CONST = 0.000000001


class Point:

    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y

    def __lt__(self, other):
        if abs(other.y - self.y) < EPS_CONST:
            if self.x < other.x:
                return True
            else:
                return False
        elif other.y > self.y:
            return True
        else:
            return False