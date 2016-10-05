# -*- coding: utf-8 -*-
"""
Created on Fri May 06 15:16:29 2016

@author: Martin
"""

from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import matplotlib.pyplot as plt
import scipy.interpolate
import numpy as np
import math

def fitnessFunc(chromosome):
	""""""
	firstSum = 0.0
	secondSum = 0.0
	for c in chromosome:
		firstSum += c**2.0
		secondSum += math.cos(2.0*math.pi*c)
	n = float(len(chromosome))
	return -20.0*math.exp(-0.2*math.sqrt(firstSum/n)) - math.exp(secondSum/n) + 20 + math.e


fig = plt.figure()
fig.dpi = 200
ax = fig.gca(projection='3d')

bound = 1
samples = 200
X = np.linspace(-bound, bound, samples)
Y = np.linspace(-bound, bound, samples)
X, Y = np.meshgrid(X, Y)

Z = np.array([fitnessFunc(chromosome) for chromosome in zip(X.flatten(), Y.flatten())]).reshape(samples, samples)

surf = ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap=cm.gist_gray, linewidth=0, antialiased=False)
#ax.view_init(elev=90, azim=90)
#ax.set_axis_off()
ax.set_xlim3d([-1, 1])
ax.set_ylim3d([-1, 1])
ax.set_zlim3d([0, 5])
plt.show()


from mpl_toolkits.mplot3d import Axes3D

D = np.loadtxt('evolution.log')

g = 10 # generation size
G = zip(np.arange(0, D.shape[0]-g+1, g), np.arange(g, D.shape[0]+1, g))

fig = plt.figure()
fig.dpi = 400
ax = fig.add_subplot(111, projection='3d')
ax.view_init(elev=90, azim=90)
ax.set_axis_off()
ax.set_xlim3d([-1, 1])
ax.set_ylim3d([-1, 1])
ax.set_zlim3d([0, 5])
ax.scatter(D[:,0], D[:,1], D[:,2], 'r', True)
plt.show()

for i in G:
    print i[0], i[1]
    Dg = D[i[0]:i[1],:]
    print np.max(Dg)
    fig = plt.figure()
    fig.dpi = 400
    ax = fig.add_subplot(111, projection='3d')
    ax.view_init(elev=90, azim=90)
    ax.set_axis_off()
    ax.set_xlim3d([-1, 1])
    ax.set_ylim3d([-1, 1])
    ax.set_zlim3d([0, 5])
    ax.scatter(Dg[:,0], Dg[:,1], Dg[:,2], 'r', True)
    plt.show()


# Generate data:
#x, y, z = 10 * np.random.random((3,10))
x = D[:,0]
y = D[:,1]
z = D[:,2]

# Set up a regular grid of interpolation points
xi, yi = np.linspace(x.min(), x.max(), 100), np.linspace(y.min(), y.max(), 100)
xi, yi = np.meshgrid(xi, yi)

# Interpolate; there's also method='cubic' for 2-D data such as here
zi = scipy.interpolate.griddata((x, y), z, (xi, yi), method='linear')

plt.imshow(zi, vmin=z.min(), vmax=z.max(), origin='lower',
           extent=[x.min(), x.max(), y.min(), y.max()], cmap=cm.jet)
plt.colorbar()
plt.show()

