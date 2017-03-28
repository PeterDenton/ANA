"""
This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code https://zenodo.org/record/x and the paper https://arxiv.org/abs/17xx.xxxxx.
"""

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.colors import LogNorm

R_0 = 8.29

N_Bins = 400

data = np.genfromtxt("data/MW_Visualization.txt").transpose()

data /= 1e3 # pc to kpc

plt.rcParams["font.size"] = 12

f, (a0, a1) = plt.subplots(2, 1, gridspec_kw = {"width_ratios": [5, 1]})

v = [[-15, 15], [-15, 15]] # just the MW
a0.hist2d(data[0], data[1], bins = N_Bins, cmap = plt.cm.Oranges, norm = LogNorm(), range = v)
a0.set_aspect("equal")

a0.text(-R_0, 0, r"$\odot$", verticalalignment = "center", horizontalalignment = "center")

a0.set_xlabel(r"$x{\rm\ (kpc)}$")
a0.set_ylabel(r"$y{\rm\ (kpc)}$")

v = [[-15, 15], [-3, 3]] # just the MW
a1.hist2d(data[0], data[2], bins = N_Bins, cmap = plt.cm.Oranges, norm = LogNorm(), range = v)
a1.set_aspect("equal")

a1.text(-R_0, 0, r"$\odot$", verticalalignment = "center", horizontalalignment = "center")

a1.set_xlabel(r"$x{\rm\ (kpc)}$")
a1.set_ylabel(r"$z{\rm\ (kpc)}$")

a1.set_yticks([-3, 0, 3])

f.subplots_adjust(hspace = 0)

f.savefig("fig/MW_Visualization.eps")

