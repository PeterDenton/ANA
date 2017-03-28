"""
This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code https://zenodo.org/record/438675 and the paper https://arxiv.org/abs/17xx.xxxxx.
"""

import matplotlib.pyplot as plt
import numpy as np
import label_lines as ll
from scipy.interpolate import InterpolatedUnivariateSpline
from scipy.optimize import fsolve

# Load data
fname = "data/Likelihood.txt"
data = np.genfromtxt(fname).transpose()

# Process data into -2log(L/L0)
max_index = np.nanargmax(data[1])
max_log_likelihood = data[1][max_index]
hat_f_gal = data[0][max_index]
for i in xrange(len(data[1])):
	data[1][i] = -2 * (data[1][i] - max_log_likelihood)

# Print out statistics
print "hat_f_gal =", hat_f_gal

chisq = InterpolatedUnivariateSpline(data[0, :-1], data[1, :-1]) # last bin often has a nan
def CL(f_gal, sig, hat_f_gal, left):
	if (left and f_gal > hat_f_gal) or (not left and f_gal < hat_f_gal):
		return 1
	else:
		return chisq(f_gal) - sig

CLs = [1, 2.71, 3.84, 4, 9, 16, 25]
CL_names = [r"$1\sigma$", r"$90\%$", r"$95\%$", r"$2\sigma$", r"$3\sigma$", r"$4\sigma$", r"$5\sigma$"]
xtol = 1e-5
for i in xrange(len(CLs)):
	f_gal_l = fsolve(CL, x0 = 0.0, args = (CLs[i], hat_f_gal, True), xtol = xtol)[0]
	f_gal_r = fsolve(CL, x0 = 0.9, args = (CLs[i], hat_f_gal, False), xtol = xtol)[0]
	if chisq(0) > CLs[i]:
		print "%s & $[%.2g,%.2g]$\\\\" % (CL_names[i], f_gal_l, f_gal_r)
	else:
		print "%s & $<%.2g$\\\\" % (CL_names[i], f_gal_r)

print "f_gal = 0 disfavored at %.4f sigma" % np.sqrt(chisq(0))
if chisq(1) >= 0:
	print "f_gal = 1 disfavored at %.4f sigma" % np.sqrt(chisq(1))
#exit()

# Plot figure
plt.plot(data[0], data[1])

v = list(plt.axis())
v = [0, 1, 0, 25]
plt.axis(v)

CL95 = 3.84 # 95% CL for 1 dof (f_gal)

plt.plot([v[0], v[1]], [CL95, CL95], "k:", label = r"$95\%{\rm\ CL}$")

ll.labelLines([plt.gca().get_lines()[-1]], [plt.gca().get_lines()], xvals = [0.8])

plt.xlabel(r"$f_{\rm gal}$")
plt.ylabel(r"$-2\log\frac{\mathcal L(f_{\rm gal})}{\mathcal L(\hat f_{\rm gal})}$")

plt.savefig("fig/Likelihood.eps")

v = [0, 0.25, 0, 2]
v = [0, 0.2, 0, 2]
v = [0, 0.5, 0, 2]
plt.axis(v)
plt.savefig("fig/Likelihood_zoom.eps")

