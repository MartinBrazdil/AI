# -*- coding: utf-8 -*-
"""
@author: Martin
"""

import numpy as np
import matplotlib.pyplot as plt

ru_evaluation = np.loadtxt('ru_evaluation.log')
egevmc_evolution = np.loadtxt('egevmc_evolution.log')
egevmc_evaluation = np.loadtxt('egevmc_evaluation.log')
egfvmc_evolution = np.loadtxt('egfvmc_evolution.log')
egfvmc_evaluation = np.loadtxt('egfvmc_evaluation.log')
gevmc_evolution = np.loadtxt('gevmc_evolution.log')
gevmc_evaluation = np.loadtxt('gevmc_evaluation.log')
gfvmc_evolution = np.loadtxt('gfvmc_evolution.log')
gfvmc_evaluation = np.loadtxt('gfvmc_evaluation.log')
osegevmc_evolution = np.loadtxt('osegevmc_evolution.log')
osegevmc_evaluation = np.loadtxt('osegevmc_evaluation.log')
osegfvmc_evolution = np.loadtxt('osegfvmc_evolution.log')
osegfvmc_evaluation = np.loadtxt('osegfvmc_evaluation.log')
osgevmc_evolution = np.loadtxt('osgevmc_evolution.log')
osgevmc_evaluation = np.loadtxt('osgevmc_evaluation.log')
osgfvmc_evolution = np.loadtxt('osgfvmc_evolution.log')
osgfvmc_evaluation = np.loadtxt('osgfvmc_evaluation.log')

def scatter(ax, x, xlabel, y, ylabel):
    ax.scatter(x, y, c='k')
    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    plt.tight_layout()
    plt.legend()

fig, (ax1, ax2, ax3) = plt.subplots(1,3,figsize=(12,3))
scatter(ax1, egevmc_evolution[:,0], 'Gamma', egevmc_evolution[:,2], 'Fitness')
scatter(ax2, egevmc_evolution[:,0], 'Gamma', egevmc_evolution[:,1], 'Epsilon')
scatter(ax3, egevmc_evolution[:,1], 'Epsilon', egevmc_evolution[:,2], 'Fitness')
fig.savefig('bopixels_egevmc_default_init.png', dpi=100)
plt.show()

fig, (ax1, ax2, ax3) = plt.subplots(1,3,figsize=(12,3))
scatter(ax1, egfvmc_evolution[:,0], 'Gamma', egfvmc_evolution[:,2], 'Fitness')
scatter(ax2, egfvmc_evolution[:,0], 'Gamma', egfvmc_evolution[:,1], 'Epsilon')
scatter(ax3, egfvmc_evolution[:,1], 'Epsilon', egfvmc_evolution[:,2], 'Fitness')
fig.savefig('bopixels_egfvmc_default_init.png', dpi=100)
plt.show()

fig, ax1 = plt.subplots(1,1,figsize=(12,3))
scatter(ax1, gevmc_evolution[:,0], 'Gamma', gevmc_evolution[:,1], 'Fitness')
fig.savefig('bopixels_gevmc_default_init.png', dpi=100)
plt.show()

fig, ax1 = plt.subplots(1,1,figsize=(12,3))
scatter(ax1, gfvmc_evolution[:,0], 'Gamma', gfvmc_evolution[:,1], 'Fitness')
fig.savefig('bopixels_gfvmc_default_init.png', dpi=100)
plt.show()

fig, (ax1, ax2, ax3) = plt.subplots(1,3,figsize=(12,3))
scatter(ax1, osegevmc_evolution[:,0], 'Gamma', osegevmc_evolution[:,2], 'Fitness')
scatter(ax2, osegevmc_evolution[:,0], 'Gamma', osegevmc_evolution[:,1], 'Epsilon')
scatter(ax3, osegevmc_evolution[:,1], 'Epsilon', osegevmc_evolution[:,2], 'Fitness')
fig.savefig('bopixels_osegevmc_default_init.png', dpi=100)
plt.show()

fig, (ax1, ax2, ax3) = plt.subplots(1,3,figsize=(12,3))
scatter(ax1, osegfvmc_evolution[:,0], 'Gamma', osegfvmc_evolution[:,2], 'Fitness')
scatter(ax2, osegfvmc_evolution[:,0], 'Gamma', osegfvmc_evolution[:,1], 'Epsilon')
scatter(ax3, osegfvmc_evolution[:,1], 'Epsilon', osegfvmc_evolution[:,2], 'Fitness')
fig.savefig('bopixels_osegfvmc_default_init.png', dpi=100)
plt.show()

fig, ax1 = plt.subplots(1,1,figsize=(12,3))
scatter(ax1, osgevmc_evolution[:,0], 'Gamma', osgevmc_evolution[:,1], 'Fitness')
fig.savefig('bopixels_osgevmc_default_init.png', dpi=100)
plt.show()

fig, ax1 = plt.subplots(1,1,figsize=(12,3))
scatter(ax1, osgfvmc_evolution[:,0], 'Gamma', osgfvmc_evolution[:,1], 'Fitness')
fig.savefig('bopixels_osgfvmc_default_init.png', dpi=100)
plt.show()

epizodes = 100
averaging = 1000

ru_evaluation = ru_evaluation.reshape((averaging, epizodes, 2)).mean(axis=0)
egevmc_evaluation = egevmc_evaluation.reshape((averaging, epizodes, 2)).mean(axis=0)
egfvmc_evaluation = egfvmc_evaluation.reshape((averaging, epizodes, 2)).mean(axis=0)
gevmc_evaluation = gevmc_evaluation.reshape((averaging, epizodes, 2)).mean(axis=0)
gfvmc_evaluation = gfvmc_evaluation.reshape((averaging, epizodes, 2)).mean(axis=0)
osegevmc_evaluation = osegevmc_evaluation.reshape((averaging, epizodes, 2)).mean(axis=0)
osegfvmc_evaluation = osegfvmc_evaluation.reshape((averaging, epizodes, 2)).mean(axis=0)
osgevmc_evaluation = osgevmc_evaluation.reshape((averaging, epizodes, 2)).mean(axis=0)
osgfvmc_evaluation = osgfvmc_evaluation.reshape((averaging, epizodes, 2)).mean(axis=0)

fig = plt.figure(None, (8, 4), dpi=80, facecolor='w', edgecolor='k')
plt.plot(ru_evaluation[:,0], ru_evaluation[:,1], 'k')
plt.plot(egfvmc_evaluation[:,0], egfvmc_evaluation[:,1], 'k')
plt.plot(gfvmc_evaluation[:,0], gfvmc_evaluation[:,1], 'k')
plt.plot(egevmc_evaluation[:,0], egevmc_evaluation[:,1], 'k')
plt.plot(gevmc_evaluation[:,0], gevmc_evaluation[:,1], 'k')
plt.plot(osegevmc_evaluation[:,0], osegevmc_evaluation[:,1], 'k')
plt.plot(osegfvmc_evaluation[:,0], osegfvmc_evaluation[:,1], 'k')
plt.plot(osgfvmc_evaluation[:,0], osgfvmc_evaluation[:,1], 'k')
plt.plot(osgevmc_evaluation[:,0], osgevmc_evaluation[:,1], 'k')
plt.xlabel('Iteration')
plt.ylabel('Reward')
plt.tight_layout()
fig.savefig('bopixels_agents_default_init.png', dpi=100)
plt.show()

# All agents totally fail. They are as useless as RU agent.