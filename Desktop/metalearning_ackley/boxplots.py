# -*- coding: utf-8 -*-
"""
Created on Sat May 07 06:38:41 2016

@author: Martin
"""

import numpy as np
import matplotlib.pyplot as plt

E = np.loadtxt('evolution_search.log')
R = np.loadtxt('random_search.log')

data = [R, E]
plt.figure()
plt.boxplot(data,vert=False,widths=0.8,labels=('Random', 'IFEP'))

plt.show()
