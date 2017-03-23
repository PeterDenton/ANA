"""
This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code https://zenodo.org/record/x and the paper https://arxiv.org/abs/17xx.xxxxx.
"""

print "Importing..."
from mpl_toolkits.basemap import Basemap
import matplotlib.pyplot as plt
import numpy as np
from astropy.coordinates import SkyCoord
from astropy import units as u

gal = True

print "Reading data..."
# read in data
dataf = open("data/SkyMap.txt", "r")
N_thetas, N_phis, N_Repeat = [int(x) for x in dataf.readline().split()]
dts = ["count"]
dt = [(d, "f") for d in dts]
data = np.loadtxt(dataf, dtype = dt)
dataf.close()
data = np.array(data["count"])

# read in IC event locations
dataf = open("inputs/IC54.txt", "r")
event_ids = []
bs = []
ls = []
for line in dataf.readlines():
	if line[0] == "#":	continue
	line = line.split()
	event_id = int(line[0])
	if event_id == 32:	continue
	Edep = float(line[1])
	if Edep < 60:		continue
	dec = float(line[5])
	RA = float(line[6])
	is_track = (line[8] == "Track")
	sc = SkyCoord(dec = dec * u.deg, ra = RA * u.deg, frame = "icrs")
	b = sc.galactic.b.degree
	l = -sc.galactic.l.degree
	event_ids.append(event_id)
	if gal:
		bs.append(b)
		ls.append(l)
	else:
		bs.append(dec)
		ls.append(360 - RA)
dataf.close()

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
m = Basemap(projection = "hammer", lat_0 = 0, lon_0 = 0)
m = Basemap(projection = "moll", lat_0 = 0, lon_0 = 0)
m.drawmapboundary(fill_color = "none") # draw a line around the map region
m.drawparallels(np.arange(-90., 120., 30.), labels = [1, 0, 0, 0], color = "gray", linewidth = 0.6, fontsize = 12) # draw parallels
m.drawmeridians([0, 60, 120, 240, 300], fontsize = 12, color = "gray", linewidth = 0.6) # draw meridians

lats = 90 - 180 * np.indices((N_thetas, N_phis))[0, :, :] / (N_thetas - 1)
lons = 360 * np.indices((N_thetas, N_phis))[1, :, :] / (N_thetas - 1)
lons = 180 - lons # reverse to agree with IC plots
x, y = m(lons, lats)

print "Drawing contours..."
levels = np.linspace(dmin - 0.4, dmax, 100)
print dmin, dmax
c = m.contourf(x, y, data, levels = levels, cmap = plt.get_cmap("Oranges"), zorder = -9)
plt.gca().set_rasterization_zorder(-1)
print ""
#print "Fixing contour edges..."
#for collection in c.collections:
#	collection.set_edgecolor("face")

print "Drawing text..."
if gal:
	plt.text(0.98, 0.02, r"${\rm Galactic}$", transform = plt.gca().transAxes, ha = "right", va = "bottom", fontsize = 12)
	plt.text(0.24, 0.496, r"${\rm Galactic\ Plane}$", transform = plt.gca().transAxes, ha = "center", va = "top", fontsize = 7)
else:
	plt.text(0.98, 0.02, r"${\rm Equatorial}$", transform = plt.gca().transAxes, ha = "right", va = "bottom", fontsize = 12)
	plt.text(0.92, 0.496, r"${\rm Equatorial\ Plane}$", transform = plt.gca().transAxes, ha = "center", va = "top", fontsize = 7)

for i in xrange(len(event_ids)):
	x, y = m(ls[i], bs[i])
	plt.text(x, y, r"$%i$" % event_ids[i], ha = "center", va = "center", fontsize = 10, color = (0, 0, 0.6))

dpi = 400
print "Saving .eps..."
plt.savefig("fig/SkyMap.eps", bbox_inches = "tight", dpi = dpi)
#print "Saving .pdf..."
#plt.savefig("fig/SkyMap.pdf", bbox_inches = "tight", dpi = dpi)

