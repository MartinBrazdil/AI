# -*- coding: utf-8 -*-
"""
@author: Martin
"""

import numpy as np
import matplotlib.pyplot as plt

fig = plt.figure()
batch_performance = np.loadtxt('one_layer_performance.log')
plt.plot(batch_performance[:,0], batch_performance[:,1], 'k+')
plt.xlabel('Iteration', fontsize=12)
plt.ylabel('Score', fontsize=12)
plt.tight_layout()
plt.show()
fig.savefig('one_layer_performance_hard.png', dpi=100)

fig = plt.figure()
batch_performance = np.loadtxt('two_layers_performance.log')
plt.plot(batch_performance[:,0], batch_performance[:,1], 'k+')
plt.xlabel('Iteration', fontsize=12)
plt.ylabel('Score', fontsize=12)
plt.tight_layout()
plt.show()
fig.savefig('two_layers_performance_hard.png', dpi=100)

fig = plt.figure()
batch_performance = np.loadtxt('conv_perc_performance.log')
plt.plot(batch_performance[:,0], batch_performance[:,1], 'k+')
plt.xlabel('Iteration', fontsize=12)
plt.ylabel('Score', fontsize=12)
plt.tight_layout()
plt.show()
fig.savefig('conv_perc_performance_hard.png', dpi=100)
