import gc
import numpy as np
from matplotlib import pyplot as plt
from matplotlib.widgets import Button
from matplotlib import collections as mc
import math

# Initializing number of dots
from Point import Point
from Voroniy import Voroniy

SIZE = 7


def inc(event):
    global SIZE
    SIZE += 1


def dec(event):
    global SIZE
    SIZE = max(3, SIZE-1)


def gen_point():
    x = np.random.random_sample()*20
    if np.random.random_sample() <= 0.5:
        x = -x
    y = np.random.random_sample()*20
    if np.random.random_sample() <= 0.5:
        y = -y
    return Point(x, y)


def set_vals(event):
    voroniy = Voroniy()
    points = [gen_point() for i in range(SIZE)]
    voroniy.set_points(points)
    voroniy.fortune_algorithm(-20, -20, 20, 20, p, lc)
    all_edges = voroniy.get_all_edges()
    x = np.empty(0)
    for i in range(len(all_edges)):
        x = np.append(x, [[all_edges[i][0].x, all_edges[i][0].y], [all_edges[i][1].x, all_edges[i][1].y]])
    x = np.reshape(x, (x.shape[0] // 4, 2, 2))
    lc.set_segments(x)
    p.set_data([point.x for point in points],
               [point.y for point in points])



# Initializing dots

# dots = [dot() for i in range(N)]
# First set up the figure, the axis, and the plot element we want to animate
fig = plt.figure()
ax = plt.axes(xlim=(-20, 20), ylim=(-20, 20))
p, = ax.plot([],
             [],
             'ro')

lc = mc.LineCollection([], colors='green', linewidths=2)
ax.add_collection(lc)

set_vals(10)

axgen = plt.axes([0, 0, 0.05, 0.05])
axinc = plt.axes([0.06, 0, 0.05, 0.05])
axdec = plt.axes([0.11, 0, 0.05, 0.05])
button = Button(axgen, 'Gen')
button.on_clicked(set_vals)
button1 = Button(axinc, 'Inc')
button1.on_clicked(inc)
button2 = Button(axdec, 'Dec')
button2.on_clicked(dec)
plt.show()
