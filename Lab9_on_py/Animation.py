import random

import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation
from matplotlib import collections as mc
import math

from Voroniy import Voroniy


class AnimateDots:

    def __init__(self):
        self.points = None
        self.voroniy = None

    def set_points(self, points):
        self.points = points
        self.voroniy = Voroniy()
        self.voroniy.set_points(points)


