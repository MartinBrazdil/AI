# -*- coding: utf-8 -*-
"""
Created on Sun May 08 19:56:40 2016

@author: Martin
"""

import numpy as np
import matplotlib.pyplot as plt



one_layer_identity_evol = np.loadtxt('one_layer_identity_evol.log')
one_layer_identity_eval = np.loadtxt('one_layer_identity_eval.log')
two_layers_identity_evol = np.loadtxt('two_layers_identity_evol.log')
two_layers_identity_eval = np.loadtxt('two_layers_identity_eval.log')
conv_perc_identity_evol = np.loadtxt('conv_perc_identity_evol.log')
conv_perc_identity_eval = np.loadtxt('conv_perc_identity_eval.log')

one_layer_relu_evol = np.loadtxt('one_layer_relu_evol.log')
one_layer_relu_eval = np.loadtxt('one_layer_relu_eval.log')
two_layers_relu_evol = np.loadtxt('two_layers_relu_evol.log')
two_layers_relu_eval = np.loadtxt('two_layers_relu_eval.log')
conv_perc_relu_evol = np.loadtxt('conv_perc_relu_evol.log')
conv_perc_relu_eval = np.loadtxt('conv_perc_relu_eval.log')

one_layer_tanh_evol = np.loadtxt('one_layer_tanh_evol.log')
one_layer_tanh_eval = np.loadtxt('one_layer_tanh_eval.log')
two_layers_tanh_evol = np.loadtxt('two_layers_tanh_evol.log')
two_layers_tanh_eval = np.loadtxt('two_layers_tanh_eval.log')
conv_perc_tanh_evol = np.loadtxt('conv_perc_tanh_evol.log')
conv_perc_tanh_eval = np.loadtxt('conv_perc_tanh_eval.log')

one_layer_logistic_evol = np.loadtxt('one_layer_logistic_evol.log')
one_layer_logistic_eval = np.loadtxt('one_layer_logistic_eval.log')
two_layers_logistic_evol = np.loadtxt('two_layers_logistic_evol.log')
two_layers_logistic_eval = np.loadtxt('two_layers_logistic_eval.log')
conv_perc_logistic_evol = np.loadtxt('conv_perc_logistic_evol.log')
conv_perc_logistic_eval = np.loadtxt('conv_perc_logistic_eval.log')

def scatter(ax, x, xlabel, y, ylabel):
    ax.scatter(x, y, c='k')
    ax.set_xlabel(xlabel, fontsize=18)
    ax.set_ylabel(ylabel, fontsize=18)
    plt.tight_layout()
    plt.legend()



#alpha
fig, (ax1) = plt.subplots(1,1,figsize=(12,3))
scatter(ax1, one_layer_identity_evol[:,0], 'Alpha1', one_layer_identity_evol[:,1], 'Fitness')
fig.savefig('one_layer_identity_evol.png', dpi=100)
plt.show()

#alpha1, alpha2, ratio
fig, ((ax1, ax2, ax3), (ax4, ax5, ax6)) = plt.subplots(2,3,figsize=(12,6))
scatter(ax1, two_layers_identity_evol[:,0], 'Alpha1', two_layers_identity_evol[:,1], 'Alpha2')
scatter(ax2, two_layers_identity_evol[:,0], 'Alpha1', two_layers_identity_evol[:,2], 'Ratio')
scatter(ax3, two_layers_identity_evol[:,0], 'Alpha1', two_layers_identity_evol[:,3], 'Fitness')
scatter(ax4, two_layers_identity_evol[:,1], 'Alpha2', two_layers_identity_evol[:,2], 'Ratio')
scatter(ax5, two_layers_identity_evol[:,1], 'Alpha2', two_layers_identity_evol[:,3], 'Fitness')
scatter(ax6, two_layers_identity_evol[:,2], 'Ratio', two_layers_identity_evol[:,3], 'Fitness')
fig.savefig('two_layers_identity_evol.png', dpi=100)
plt.show()

#alpha1, alpha2, filters
fig, ((ax1, ax2, ax3), (ax4, ax5, ax6)) = plt.subplots(2,3,figsize=(12,6))
scatter(ax1, conv_perc_identity_evol[:,0], 'Alpha1', conv_perc_identity_evol[:,1], 'Alpha2')
scatter(ax2, conv_perc_identity_evol[:,0], 'Alpha1', conv_perc_identity_evol[:,2], 'Filters')
scatter(ax3, conv_perc_identity_evol[:,0], 'Alpha1', conv_perc_identity_evol[:,3], 'Fitness')
scatter(ax4, conv_perc_identity_evol[:,1], 'Alpha2', conv_perc_identity_evol[:,2], 'Filters')
scatter(ax5, conv_perc_identity_evol[:,1], 'Alpha2', conv_perc_identity_evol[:,3], 'Fitness')
scatter(ax6, conv_perc_identity_evol[:,2], 'Filters', conv_perc_identity_evol[:,3], 'Fitness')
fig.savefig('conv_perc_identity_evol.png', dpi=100)
plt.show()



#alpha
fig, (ax1) = plt.subplots(1,1,figsize=(12,3))
scatter(ax1, one_layer_relu_evol[:,0], 'Alpha1', one_layer_relu_evol[:,1], 'Fitness')
fig.savefig('one_layer_relu_evol.png', dpi=100)
plt.show()

#alpha1, alpha2, ratio
fig, ((ax1, ax2, ax3), (ax4, ax5, ax6)) = plt.subplots(2,3,figsize=(12,6))
scatter(ax1, two_layers_relu_evol[:,0], 'Alpha1', two_layers_relu_evol[:,1], 'Alpha2')
scatter(ax2, two_layers_relu_evol[:,0], 'Alpha1', two_layers_relu_evol[:,2], 'Ratio')
scatter(ax3, two_layers_relu_evol[:,0], 'Alpha1', two_layers_relu_evol[:,3], 'Fitness')
scatter(ax4, two_layers_relu_evol[:,1], 'Alpha2', two_layers_relu_evol[:,2], 'Ratio')
scatter(ax5, two_layers_relu_evol[:,1], 'Alpha2', two_layers_relu_evol[:,3], 'Fitness')
scatter(ax6, two_layers_relu_evol[:,2], 'Ratio', two_layers_relu_evol[:,3], 'Fitness')
fig.savefig('two_layers_relu_evol.png', dpi=100)
plt.show()

#alpha1, alpha2, filters
fig, ((ax1, ax2, ax3), (ax4, ax5, ax6)) = plt.subplots(2,3,figsize=(12,6))
scatter(ax1, conv_perc_relu_evol[:,0], 'Alpha1', conv_perc_relu_evol[:,1], 'Alpha2')
scatter(ax2, conv_perc_relu_evol[:,0], 'Alpha1', conv_perc_relu_evol[:,2], 'Filters')
scatter(ax3, conv_perc_relu_evol[:,0], 'Alpha1', conv_perc_relu_evol[:,3], 'Fitness')
scatter(ax4, conv_perc_relu_evol[:,1], 'Alpha2', conv_perc_relu_evol[:,2], 'Filters')
scatter(ax5, conv_perc_relu_evol[:,1], 'Alpha2', conv_perc_relu_evol[:,3], 'Fitness')
scatter(ax6, conv_perc_relu_evol[:,2], 'Filters', conv_perc_relu_evol[:,3], 'Fitness')
fig.savefig('conv_perc_relu_evol.png', dpi=100)
plt.show()



#alpha
fig, (ax1) = plt.subplots(1,1,figsize=(12,3))
scatter(ax1, one_layer_tanh_evol[:,0], 'Alpha1', one_layer_tanh_evol[:,1], 'Fitness')
fig.savefig('one_layer_tanh_evol.png', dpi=100)
plt.show()

#alpha1, alpha2, ratio
fig, ((ax1, ax2, ax3), (ax4, ax5, ax6)) = plt.subplots(2,3,figsize=(12,6))
scatter(ax1, two_layers_tanh_evol[:,0], 'Alpha1', two_layers_tanh_evol[:,1], 'Alpha2')
scatter(ax2, two_layers_tanh_evol[:,0], 'Alpha1', two_layers_tanh_evol[:,2], 'Ratio')
scatter(ax3, two_layers_tanh_evol[:,0], 'Alpha1', two_layers_tanh_evol[:,3], 'Fitness')
scatter(ax4, two_layers_tanh_evol[:,1], 'Alpha2', two_layers_tanh_evol[:,2], 'Ratio')
scatter(ax5, two_layers_tanh_evol[:,1], 'Alpha2', two_layers_tanh_evol[:,3], 'Fitness')
scatter(ax6, two_layers_tanh_evol[:,2], 'Ratio', two_layers_tanh_evol[:,3], 'Fitness')
fig.savefig('two_layers_tanh_evol.png', dpi=100)
plt.show()

#alpha1, alpha2, filters
fig, ((ax1, ax2, ax3), (ax4, ax5, ax6)) = plt.subplots(2,3,figsize=(12,6))
scatter(ax1, conv_perc_tanh_evol[:,0], 'Alpha1', conv_perc_tanh_evol[:,1], 'Alpha2')
scatter(ax2, conv_perc_tanh_evol[:,0], 'Alpha1', conv_perc_tanh_evol[:,2], 'Filters')
scatter(ax3, conv_perc_tanh_evol[:,0], 'Alpha1', conv_perc_tanh_evol[:,3], 'Fitness')
scatter(ax4, conv_perc_tanh_evol[:,1], 'Alpha2', conv_perc_tanh_evol[:,2], 'Filters')
scatter(ax5, conv_perc_tanh_evol[:,1], 'Alpha2', conv_perc_tanh_evol[:,3], 'Fitness')
scatter(ax6, conv_perc_tanh_evol[:,2], 'Filters', conv_perc_tanh_evol[:,3], 'Fitness')
fig.savefig('conv_perc_tanh_evol.png', dpi=100)
plt.show()



#alpha
fig, (ax1) = plt.subplots(1,1,figsize=(12,3))
scatter(ax1, one_layer_logistic_evol[:,0], 'Alpha1', one_layer_logistic_evol[:,1], 'Fitness')
fig.savefig('one_layer_logistic_evol.png', dpi=100)
plt.show()

#alpha1, alpha2, ratio
fig, ((ax1, ax2, ax3), (ax4, ax5, ax6)) = plt.subplots(2,3,figsize=(12,6))
scatter(ax1, two_layers_logistic_evol[:,0], 'Alpha1', two_layers_logistic_evol[:,1], 'Alpha2')
scatter(ax2, two_layers_logistic_evol[:,0], 'Alpha1', two_layers_logistic_evol[:,2], 'Ratio')
scatter(ax3, two_layers_logistic_evol[:,0], 'Alpha1', two_layers_logistic_evol[:,3], 'Fitness')
scatter(ax4, two_layers_logistic_evol[:,1], 'Alpha2', two_layers_logistic_evol[:,2], 'Ratio')
scatter(ax5, two_layers_logistic_evol[:,1], 'Alpha2', two_layers_logistic_evol[:,3], 'Fitness')
scatter(ax6, two_layers_logistic_evol[:,2], 'Ratio', two_layers_logistic_evol[:,3], 'Fitness')
fig.savefig('two_layers_logistic_evol.png', dpi=100)
plt.show()

#alpha1, alpha2, filters
fig, ((ax1, ax2, ax3), (ax4, ax5, ax6)) = plt.subplots(2,3,figsize=(12,6))
scatter(ax1, conv_perc_logistic_evol[:,0], 'Alpha1', conv_perc_logistic_evol[:,1], 'Alpha2')
scatter(ax2, conv_perc_logistic_evol[:,0], 'Alpha1', conv_perc_logistic_evol[:,2], 'Filters')
scatter(ax3, conv_perc_logistic_evol[:,0], 'Alpha1', conv_perc_logistic_evol[:,3], 'Fitness')
scatter(ax4, conv_perc_logistic_evol[:,1], 'Alpha2', conv_perc_logistic_evol[:,2], 'Filters')
scatter(ax5, conv_perc_logistic_evol[:,1], 'Alpha2', conv_perc_logistic_evol[:,3], 'Fitness')
scatter(ax6, conv_perc_logistic_evol[:,2], 'Filters', conv_perc_logistic_evol[:,3], 'Fitness')
fig.savefig('conv_perc_logistic_evol.png', dpi=100)
plt.show()
