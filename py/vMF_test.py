"""
This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code zenodo.org/record/x and the paper https://arxiv.org/abs/17xx.xxxxx.
"""

import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import fsolve

def vMF(cos_theta, k):
	C = k / (4 * np.pi * np.sinh(k))
	return C * np.exp(k * cos_theta)

def kappa_helper(k, sigma):
	return 1 - k * np.power(sigma, 2) - np.exp(-2 * k)

def kappa(sigma):
	return fsolve(kappa_helper, sigma ** -2, args = sigma)[0]

dataf = open("data/vMF_test.txt", "r")
alpha50, sigma_direction = [float(x) for x in dataf.readline().split()]
dts = ["cos_theta"]
dt = [(d, "f") for d in dts]
data = np.loadtxt(dataf, dtype = dt)
dataf.close()
N_Repeat = len(data["cos_theta"])

N_Bins = 50
n, bins, patches = plt.hist(data["cos_theta"], bins = N_Bins, normed = True, histtype = "step", color = "b")

v = plt.axis()

cos_thetas = np.linspace(v[0], v[1], 100)
k = kappa(sigma_direction)
print "kappa =", k
print "sigma =", sigma_direction
print "alpha50 =", alpha50 * 180 / np.pi
vMFs = [2 * np.pi * vMF(cos_theta, k) for cos_theta in cos_thetas]
plt.plot(cos_thetas, vMFs, "r-")

v = list(plt.axis())

plt.plot([np.median(data["cos_theta"]), np.median(data["cos_theta"])], v[2:], "b-")
plt.plot([np.cos(alpha50), np.cos(alpha50)], v[2:], "r--")

v[0] = 0.7
plt.axis(v)

s = r"$\alpha_{50\%"
s += r"}=%i^\circ$" % round(alpha50 * 180 / np.pi)
plt.text(0.05, 0.95, s, transform = plt.gca().transAxes, ha = "left", va = "top")

plt.xlabel(r"$\cos\theta$")

plt.savefig("fig/vMF_test.eps")

