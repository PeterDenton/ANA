"""
This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code https://zenodo.org/record/x and the paper https://arxiv.org/abs/17xx.xxxxx.
"""

# from this post:
# http://stackoverflow.com/questions/16992038/inline-labels-in-matplotlib
# usage:
#	import label_lines as ll
#	plt.plot(x, y, label = "label")
#	ll.labelLines(plt.gca().get_lines())

from math import atan2,degrees,log10,log
import numpy as np

from matplotlib.dates import date2num
from datetime import datetime

def trans(x, xmin, xmax, logx = False):
	if logx:
		x = log(x)
		xmin = log(xmin)
		xmax = log(xmax)
	r = xmax - xmin
	return 1.0 * (x - xmin) / r	

#Label line with line2D label data
def labelLine(line,x,label=None,align=True,logx=False,logy=False,**kwargs):

	ax = line.axes
	xdata = line.get_xdata()
	ydata = line.get_ydata()
	xmin,xmax = ax.get_xlim()
	ymin,ymax = ax.get_ylim()

	# Convert datetime objects to floats
	if isinstance(x, datetime):
		x = date2num(x)

	if (x < xdata[0]) or (x > xdata[-1]):
		print('x label location is outside data range!')
		return

	#Find corresponding y co-ordinate and angle of the
	ip = 1
	for i in range(len(xdata)):
		if x < xdata[i]:
			ip = i
			break

	y = ydata[ip-1] + (ydata[ip]-ydata[ip-1])*(x-xdata[ip-1])/(xdata[ip]-xdata[ip-1])

	if not label:
		label = line.get_label()

	if align:
		#Compute the slope
		dx = trans(xdata[ip], xmin, xmax, logx) - trans(xdata[ip-1], xmin, xmax, logx)
		dy = trans(ydata[ip], ymin, ymax, logy) - trans(ydata[ip-1], ymin, ymax, logy)
		ang = degrees(atan2(dy,dx))
	else:
		ang = 0

	#Set a bunch of keyword arguments
	if 'color' not in kwargs:
		kwargs['color'] = line.get_color()

	if ('horizontalalignment' not in kwargs) and ('ha' not in kwargs):
		kwargs['ha'] = 'center'

	if ('verticalalignment' not in kwargs) and ('va' not in kwargs):
		kwargs['va'] = 'center'

	if 'backgroundcolor' not in kwargs:
		kwargs['backgroundcolor'] = ax.get_axis_bgcolor()

	if 'clip_on' not in kwargs:
		kwargs['clip_on'] = True

	if 'zorder' not in kwargs:
		kwargs['zorder'] = 2.5

	ax.text(x,y,label,rotation=ang,**kwargs)

def labelLines(lines,align=True,xvals=None,logx=False,logy=False,**kwargs):

	ax = lines[0].axes
	labLines = []
	labels = []

	#Take only the lines which have labels other than the default ones
	for line in lines:
		label = line.get_label()
		if "_line" not in label:
			labLines.append(line)
			labels.append(label)

	if xvals is None:
		xmin,xmax = ax.get_xlim()
		if logx:
			xvals = np.logspace(log10(xmin),log10(xmax),len(labLines)+2)[1:-1]
		else:
			xvals = np.linspace(xmin,xmax,len(labLines)+2)[1:-1]

	for line,x,label in zip(labLines,xvals,labels):
		labelLine(line,x,label,align,logx,logy,**kwargs)

