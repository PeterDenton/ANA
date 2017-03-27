"""
This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code https://zenodo.org/record/x and the paper https://arxiv.org/abs/17xx.xxxxx.
"""

print "Importing..."
from mpl_toolkits.basemap import Basemap
import matplotlib.pyplot as plt
import numpy as np

print "Reading data..."
# read in data
dataf = open("data/MW_SkyMap.txt", "r")
N_thetas, N_phis, N_Repeat = [int(x) for x in dataf.readline().split()]
dts = ["count"]
dt = [(d, "f") for d in dts]
data = np.loadtxt(dataf, dtype = dt)
dataf.close()
data = np.array(data["count"])

# get ranges
dmax = np.nanmax(data)

for i in xrange(len(data)):
	if np.isinf(data[i]):
		data[i] = dmax + 1

dmin = np.nanmin(data)

for i in xrange(len(data)):
	if data[i] == dmax + 1:
		data[i] = dmin - 0.3

# reshape
data.resize(N_thetas, N_phis)

print "Drawing skymap lines..."
# draw map with lines
m = Basemap(projection = "moll", lat_0 = 0, lon_0 = 0)
m.drawmapboundary(fill_color = "none") # draw a line around the map region
m.drawparallels(np.arange(-90., 120., 30.), labels = [1, 0, 0, 0], color = "gray", linewidth = 0.6, fontsize = 12) # draw parallels
m.drawmeridians([0, 60, 120, 240, 300], fontsize = 12, color = "gray", linewidth = 0.6) # draw meridians

lats = 90 - 180 * (np.indices((N_thetas, N_phis))[0, :, :] + 0.5) / N_thetas
lons = 360 * (np.indices((N_thetas, N_phis))[1, :, :] + 0.5) / N_thetas
lons = 180 - lons # reverse to agree with IC plots
x, y = m(lons, lats)

print "Drawing contours..."
levels = np.linspace(dmin - 0.4, dmax, 100)
c = m.contourf(x, y, data, levels = levels, cmap = plt.get_cmap("Blues"), zorder = -9)
plt.gca().set_rasterization_zorder(-1)
print ""

print "Drawing text..."
ta = plt.gca().transAxes
plt.text(0.98, 0.02, r"${\rm Galactic}$", transform = ta, ha = "right", va = "bottom", fontsize = 12)
plt.text(0.01, 0.52, r"$+180^\circ$", transform = ta, ha = "left", va = "bottom", fontsize = 7)
plt.text(0.99, 0.52, r"$-180^\circ$", transform = ta, ha = "right", va = "bottom", fontsize = 7)

dpi = 250
print "Saving .eps..."
plt.savefig("fig/MW_SkyMap.eps", bbox_inches = "tight", dpi = dpi)

