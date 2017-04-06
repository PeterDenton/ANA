"""
This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code https://zenodo.org/record/438675 and the paper https://arxiv.org/abs/1703.09721.
"""

import matplotlib.pyplot as plt
import numpy as np
import label_lines as ll

# Load data
fname = "data/Likelihood.txt"
data = np.genfromtxt(fname).transpose()

# Process data into -2log(L/L0)
max_index = np.nanargmax(data[1])
max_log_likelihood = data[1][max_index]
for i in xrange(len(data[1])):
	data[1][i] = -2 * (data[1][i] - max_log_likelihood)

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

