# -*- coding: utf-8 -*-
"""
Created on Sat May 07 19:45:44 2016

@author: Martin
"""

import numpy as np
import matplotlib.pyplot as plt

gwcoords = np.loadtxt('gwcoords.log')
bopbf_ff = np.loadtxt('bopbf_ff.log')
bopbf_ft = np.loadtxt('bopbf_ft.log')
bopbf_tf = np.loadtxt('bopbf_tf.log')
bopbf_tt = np.loadtxt('bopbf_tt.log')
bopb_ff = np.loadtxt('bopb_ff.log')
bopb_ft = np.loadtxt('bopb_ft.log')
bopb_tf = np.loadtxt('bopb_tf.log')
bopb_tt = np.loadtxt('bopb_tt.log')
bopixels_ff = np.loadtxt('bopixels_ff.log')
bopixels_ft = np.loadtxt('bopixels_ft.log')
bopixels_tf = np.loadtxt('bopixels_tf.log')
bopixels_tt = np.loadtxt('bopixels_tt.log')
noisy_bopbf_ff = np.loadtxt('noisy_bopbf_ff.log')
noisy_bopbf_ft = np.loadtxt('noisy_bopbf_ft.log')
noisy_bopbf_tf = np.loadtxt('noisy_bopbf_tf.log')
noisy_bopbf_tt = np.loadtxt('noisy_bopbf_tt.log')
noisy_bopb_ff = np.loadtxt('noisy_bopb_ff.log')
noisy_bopb_ft = np.loadtxt('noisy_bopb_ft.log')
noisy_bopb_tf = np.loadtxt('noisy_bopb_tf.log')
noisy_bopb_tt = np.loadtxt('noisy_bopb_tt.log')
noisy_bopixels_ff = np.loadtxt('noisy_bopixels_ff.log')
noisy_bopixels_ft = np.loadtxt('noisy_bopixels_ft.log')
noisy_bopixels_tf = np.loadtxt('noisy_bopixels_tf.log')
noisy_bopixels_tt = np.loadtxt('noisy_bopixels_tt.log')

fig = plt.figure(None, (8, 4), dpi=80, facecolor='w', edgecolor='k')
plt.plot(gwcoords[:,0], gwcoords[:,1], 'ko-')
plt.xlabel('Iteration')
plt.ylabel('Delta')
plt.tight_layout()
fig.savefig('gridcoord.png', dpi=100)
plt.show()

fig = plt.figure(None, (8, 4), dpi=80, facecolor='w', edgecolor='k')
plt.plot(bopbf_ff[:,0], bopbf_ff[:,1], 'ko-', label='+/- reward, not terminal bounce')
plt.plot(bopbf_ft[:,0], bopbf_ft[:,1], 'ko--', label='+/- reward, terminal bounce')
plt.plot(bopbf_tf[:,0], bopbf_tf[:,1], 'ko-.', label='+ reward, not terminal bounce')
plt.plot(bopbf_tt[:,0], bopbf_tt[:,1], 'ko:', label='+ reward, terminal bounce')
plt.xlabel('Iteration')
plt.ylabel('Delta')
plt.tight_layout()
plt.legend()
fig.savefig('bopbf.png', dpi=100)
plt.show()

fig = plt.figure(None, (8, 4), dpi=80, facecolor='w', edgecolor='k')
plt.plot(bopb_ff[:,0], bopb_ff[:,1], 'ko-', label='+/- reward, not terminal bounce')
plt.plot(bopb_ft[:,0], bopb_ft[:,1], 'ko--', label='+/- reward, terminal bounce')
plt.plot(bopb_tf[:,0], bopb_tf[:,1], 'ko-.', label='+ reward, not terminal bounce')
plt.plot(bopb_tt[:,0], bopb_tt[:,1], 'ko:', label='+ reward, terminal bounce')
plt.xlabel('Iteration')
plt.ylabel('Delta')
plt.tight_layout()
plt.legend()
fig.savefig('bopb.png', dpi=100)
plt.show()

fig = plt.figure(None, (8, 4), dpi=80, facecolor='w', edgecolor='k')
plt.plot(bopixels_ff[:,0], bopixels_ff[:,1], 'ko-', label='+/- reward, not terminal bounce')
plt.plot(bopixels_ft[:,0], bopixels_ft[:,1], 'ko--', label='+/- reward, terminal bounce')
plt.plot(bopixels_tf[:,0], bopixels_tf[:,1], 'ko-.', label='+ reward, not terminal bounce')
plt.plot(bopixels_tt[:,0], bopixels_tt[:,1], 'ko:', label='+ reward, terminal bounce')
plt.xlabel('Iteration')
plt.ylabel('Delta')
plt.tight_layout()
plt.legend()
fig.savefig('bopixels.png', dpi=100)
plt.show()

fig = plt.figure(None, (8, 4), dpi=80, facecolor='w', edgecolor='k')
plt.plot(noisy_bopbf_ff[:,0], noisy_bopbf_ff[:,1], 'ko-', label='+/- reward, not terminal bounce')
plt.plot(noisy_bopbf_ft[:,0], noisy_bopbf_ft[:,1], 'ko--', label='+/- reward, terminal bounce')
plt.plot(noisy_bopbf_tf[:,0], noisy_bopbf_tf[:,1], 'ko-.', label='+ reward, not terminal bounce')
plt.plot(noisy_bopbf_tt[:,0], noisy_bopbf_tt[:,1], 'ko:', label='+ reward, terminal bounce')
plt.xlabel('Iteration')
plt.ylabel('Delta')
plt.tight_layout()
plt.legend()
fig.savefig('noisy_bopbf.png', dpi=100)
plt.show()

fig = plt.figure(None, (8, 4), dpi=80, facecolor='w', edgecolor='k')
plt.plot(noisy_bopb_ff[:,0], noisy_bopb_ff[:,1], 'ko-', label='+/- reward, not terminal bounce')
plt.plot(noisy_bopb_ft[:,0], noisy_bopb_ft[:,1], 'ko--', label='+/- reward, terminal bounce')
plt.plot(noisy_bopb_tf[:,0], noisy_bopb_tf[:,1], 'ko-.', label='+ reward, not terminal bounce')
plt.plot(noisy_bopb_tt[:,0], noisy_bopb_tt[:,1], 'ko:', label='+ reward, terminal bounce')
plt.xlabel('Iteration')
plt.ylabel('Delta')
plt.tight_layout()
plt.legend()
fig.savefig('noisy_bopb.png', dpi=100)
plt.show()

fig = plt.figure(None, (8, 4), dpi=80, facecolor='w', edgecolor='k')
plt.plot(noisy_bopixels_ff[:,0], noisy_bopixels_ff[:,1], 'ko-', label='+/- reward, not terminal bounce')
plt.plot(noisy_bopixels_ft[:,0], noisy_bopixels_ft[:,1], 'ko--', label='+/- reward, terminal bounce')
plt.plot(noisy_bopixels_tf[:,0], noisy_bopixels_tf[:,1], 'ko-.', label='+ reward, not terminal bounce')
plt.plot(noisy_bopixels_tt[:,0], noisy_bopixels_tt[:,1], 'ko:', label='+ reward, terminal bounce')
plt.xlabel('Iteration')
plt.ylabel('Delta')
plt.tight_layout()
plt.legend()
fig.savefig('noisy_bopixels.png', dpi=100)
plt.show()