# -*- coding: utf-8 -*-
'''
Counting glomeruli in Light-Sheet microscopy images of kidney. 
Full details of the alogrithm can be found in the paper 
Klingberg et al. (2017) Fully Automated Evaluation of Total Glomerular Number and 
Capillary Tuft Size in Nephritic Kidneys Using Lightsheet Microscopy,
J. Am. Soc. Nephrol., 28: 452-459. 

For running in command line: ``python analyze_kidney.py -i <file with settings> ``

:Author:
  `Anna Medyukhina`_
  email: anna.medyukhina@leibniz-hki.de or anna.medyukhina@gmail.com	

:Organization:
  Applied Systems Biology Group, Leibniz Institute for Natural Product Research and Infection Biology - Hans Knöll Institute (HKI)

:Version: 2015.11.12

Copyright (c) 2014-2015, 
Leibniz Institute for Natural Product Research and Infection Biology – 
Hans Knöll Institute (HKI)

Licence: BSD-3-Clause, see ./LICENSE or 
https://opensource.org/licenses/BSD-3-Clause for full details

Requirements
------------
* `Python 2.7.3  <http://www.python.org>`_
* `Numpy 1.9.1 <http://www.numpy.org>`_
* `Scipy.ndimage 2.0 <http://www.scipy.org>`_
* `Mahotas 1.0.3 `_
* `argparse 1.1 `_
* `pandas 0.15.2 <http://pandas.pydata.org>`_

Reference
---------
Klingberg et al. (2017) Fully Automated Evaluation of Total Glomerular Number and 
Capillary Tuft Size in Nephritic Kidneys Using Lightsheet Microscopy,
J. Am. Soc. Nephrol., 28: 452-459. 
'''

import sys
sys.path.append('include')

import re
import pandas as pd
import numpy as np
import mahotas
from scipy import ndimage
import skimage.io
import time


import filelib
import boost
import tifffile



def list_subfolders(inputfolder, subfolders = []):
	'''
	list folders, each containing layers of one stack
	'''
	files = filelib.list_subfolders(inputfolder, subfolders = subfolders) 
	files.sort()

	folders = []
	for f in files:
		folders.append(filelib.return_path(f))

	folders = np.unique(folders)

	return folders


def extract_zoom(folder):
	'''
	Extract zoom data from image name
	'''
	parts = folder.split('zoom')
	p = re.compile('\d+')
	if len(parts) > 1:
		zoom = p.findall(parts[1])[0]
	else:
		zoom = '063'

	zsize = 5.
	if zoom == '063':
		xsize = 5.159
	if zoom == '08':
		xsize = 4.063

	return xsize, zsize


def normalize(img, per = 100, min_signal_value = 0):	
	ph = np.percentile(img, per)
	pl = np.percentile(img, 100-per)
	if ph > min_signal_value:
		img = np.where(img>ph, ph, img)
		img = np.where(img<pl, pl, img)
		img = img - img.min()
		img = img*255./img.max()

	else:
		img = np.zeros_like(img)
	
	return img


################################################################################
#Segmentation

def segment(folder, params):
	'''
	Segment all layers in a folder
	'''
	#create folders for the output
	filelib.make_folders([params.inputfolder + '../segmented/masks/glomeruli/' + folder, params.inputfolder + '../segmented/masks/kidney/' + folder, params.inputfolder + '../log/' + folder])

	start = time.time()
	#list all files in the folder
	files = filelib.list_image_files(params.inputfolder + folder)
	files.sort()

	params.folder = folder

	#segment all layers in parallel
	boost.run_parallel(process = segment_layer, files = files, params = params, procname = 'Segmentation of glomeruli')
#	segment_layer(files[50], params)
	t = pd.Series({'Segmentation': time.time() - start, 'threads': params.max_threads})
	t.to_csv(params.inputfolder + '../log/' + folder + 'Segmentation.csv', sep = '\t')
		

def segment_layer(filename, params):
	'''
	Segment one layer in a stack
	'''
	start = time.time()
	#extract pixel size in xy and z
	xsize, zsize = extract_zoom(params.folder)

	#load image
	img = tifffile.imread(params.inputfolder + params.folder + filename)

	#normalize image
	img = ndimage.median_filter(img, 3)
	img = img*255./img.max()

	##segment kidney tissue

	sizefactor = 10.
	small = ndimage.interpolation.zoom(img, 1./sizefactor)	#scale the image to a smaller size

	imgf = ndimage.gaussian_filter(small, 3./xsize)	#Gaussian filter
	median = np.percentile(imgf, 40)	#40-th percentile for thresholding

	kmask = imgf>median*1.5	#thresholding
	kmask = mahotas.dilate(kmask, mahotas.disk(5))	
	kmask = mahotas.close_holes(kmask)					#closing holes
	kmask = mahotas.erode(kmask, mahotas.disk(5))*255

	#remove objects that are darker than 2*percentile
	l,n = ndimage.label(kmask)	
	llist = np.unique(l)
	if len(llist) > 2:
		means = ndimage.mean(imgf, l, llist)
		bv = llist[np.where(means < median*2)]
		ix = np.in1d(l.ravel(), bv).reshape(l.shape)
		kmask[ix] = 0

	kmask = ndimage.interpolation.zoom(kmask, sizefactor)	#scale back to normal size
	kmask = normalize(kmask)
	kmask = (kmask > mahotas.otsu(kmask.astype(np.uint8)))*255.	#remove artifacts of interpolation

	#save indices of the kidney mask
	ind = np.where(kmask > 0)
	ind = np.array(ind)
	np.save(params.inputfolder + '../segmented/masks/kidney/' + params.folder + filename[:-4] + '.npy', ind)
	skimage.io.imsave(params.inputfolder + '../segmented/masks/kidney/' + params.folder + filename[:-4] + '.tif', (kmask > 0).astype(np.uint8) * 255)


	#segment glomeruli, if there is a kidney tissue
	if kmask.max() > 0:
		#remove all intensity variations larger than maximum radius of a glomerulus
		d = mahotas.disk(int(float(params.maxrad)/xsize))
		img = img - mahotas.open(img.astype(np.uint8), d)
		img = img*255./img.max()
		ch = img[np.where(kmask > 0)]

		#segment glomeruli by otsu thresholding	only if this threshold is higher than the 75-th percentile in the kidney mask	
		t = mahotas.otsu(img.astype(np.uint8))

		if t > np.percentile(ch, 75)*1.5:			
			cells = img > t
			cells[np.where(kmask == 0)] = 0
			cells = mahotas.open(cells, mahotas.disk(int(float(params.minrad)/2./xsize)))

		else:
			cells = np.zeros_like(img)

	else:
		cells = np.zeros_like(img)

	
	#save indices of the glomeruli mask
	ind = np.where(cells > 0)
	ind = np.array(ind)
	np.save(params.inputfolder + '../segmented/masks/glomeruli/' + params.folder + filename[:-4] + '.npy', ind)
	skimage.io.imsave(params.inputfolder + '../segmented/masks/glomeruli/' + params.folder + filename[:-4] + '.tif',
					  (cells > 0).astype(np.uint8) * 255)
	# skimage.io.imsave(params.inputfolder + '../segmented/masks/glomeruli/' + params.folder + filename[:-4] + '.tif', (ind * 255).astype(np.uint8))


def load_mask(filename, shape):
	'''
	load mask from input file
	'''
	ind = np.load(filename)
	mask = np.zeros(shape)
	mask[ind[0],ind[1]] = 1
	return mask



def label_cells(folder, params):
	'''
	label connected objects in a stack
	'''
	start = time.time()

	#list files in the folder
	files = filelib.list_image_files(params.inputfolder + folder)
	files.sort()

	#make output folder for labels
	filelib.make_folders([params.inputfolder + '../segmented/labels/glomeruli/' + folder])

	#initiate labelling
	num = 0 
	shape = tifffile.imread(params.inputfolder + folder + files[0][:-4] + '.tif').shape #get image dimensions from the first image
	mask = load_mask(params.inputfolder + '../segmented/masks/glomeruli/' + folder + files[0][:-4] + '.npy', shape)	#load the first mask
	l0 = np.zeros_like(mask) # Label for last layer

	labels = []
	limsize = int(float(params.maxrad))
	nfiles = list(np.array(files))

	#label
	for i in range(len(files)):		# Go through each image (in order)
		mask = load_mask(params.inputfolder + '../segmented/masks/glomeruli/' + folder + files[i][:-4] + '.npy', shape)
		l,n = ndimage.label(mask) # Label the image
		mask = (mask>0)*1.

		labels.append(np.zeros_like(l)) # Create a new label image initialized with 0 and add it to the list
		for j in range(n): # For each label:
			ind = np.where(l==j+1) ## Get all indices where the current label is (j + 1)
			ls = np.unique(l0[ind]) # Get all labels in last layer where current label is (j + 1)
			ls = ls[np.where(ls>0)] # Only consider non-background
			if len(ls) == 0: # If there is no connection, create a new label
				num+=1
				labels[-1][ind] = num

			if len(ls) == 1: # If there is a connection to 1 specific id, rename the current layer (labels[-1]) label
				labels[-1][ind] = ls[0]

			if len(ls) > 1: # If there are multiple connections, synchronize them
				labels[-1][ind] = ls[0] # Assign current label from last label (for the first)
				for k in range(1,len(ls)): # Go through all last labels (except the first one)
					for s in range(len(labels)): # Go through the list of stored labels
						labels[s] = np.where(labels[s] == ls[k], ls[0], labels[s]) #Replace all ls[k] with ls[0]
		l0 = labels[-1]


		#if number of layers exceeds limit, save the first layer
		if len(labels) > limsize:
			mask = labels.pop(0)
			fname = nfiles.pop(0)
			ind = np.where(mask > 0)
			lb = mask[ind]
			np.save(params.inputfolder + '../segmented/labels/glomeruli/' + folder + fname[:-4] + '.npy', np.array([lb, ind[0], ind[1]]))

	
	#save remaining layers
	for i in range(len(labels)):
		mask = labels.pop(0)
		fname = nfiles.pop(0)
		ind = np.where(mask > 0)
		lb = mask[ind]
		np.save(params.inputfolder + '../segmented/labels/glomeruli/' + folder + fname[:-4] + '.npy', np.array([lb, ind[0], ind[1]]))

	t = pd.Series({'Labelling': time.time() - start})
	t.to_csv(params.inputfolder + '../log/' + folder + 'Labelling.csv', sep = '\t')



#############################################################################
#Quantification

def quantify(folder, params):
	'''
	Quantify a stack
	'''
	start = time.time()
	#list files in the folder
	files = filelib.list_image_files(params.inputfolder + folder)
	files.sort()

	#create a folder for statistics
	filelib.make_folders([params.inputfolder + '../statistics/' + filelib.return_path(folder[:-1] + '.csv')])

	#extract voxel size
	xsize, zsize = extract_zoom(folder)
	print("xsize: " + str(xsize) + ", zsize: " + str(zsize))

	#compute volume of the kidney
	kidney_volume = 0

	for i in range(len(files)):
		ind = np.load(params.inputfolder + '../segmented/masks/kidney/' + folder + files[i][:-4] + '.npy')
		kidney_volume = kidney_volume + len(ind[0])


	#compute glomeruli characteristics
	maxvol = np.pi*float(params.maxrad)**2	#maximal allowed area
	minvol = np.pi*float(params.minrad)**2	#minimal allowed area


	print 'load data', len(files)
	ind0 = []
	ind1 = []
	ind2 = []
	label = []

	for i in range(len(files)):
		ind = np.load(params.inputfolder + '../segmented/labels/glomeruli/' + folder + files[i][:-4] + '.npy')
		label.append(ind[0])
		ind1.append(ind[1])
		ind2.append(ind[2])
		ind0.append(np.ones_like(ind[1])*i)



	ind0 = np.hstack(ind0)
	ind1 = np.hstack(ind1)
	ind2 = np.hstack(ind2)
	label = np.hstack(label)

	llist = np.unique(label)
	volumes = ndimage.sum((label>0)*1., label, llist)*xsize*xsize # Modification: Calculate areas
	ind = np.where((volumes < maxvol)&(volumes > minvol))
	volumes = volumes[ind]
	llist = llist[ind]
	sizes = 2* np.sqrt(volumes/np.pi)
	stat = pd.DataFrame({'Label':llist,
						'Diameter':sizes,
						'Volume':volumes})
	stat['Image_name'] = folder[:-1]
	stat['Kidney_volume'] = kidney_volume*xsize**2
	
	stat.to_csv(params.inputfolder + '../statistics/' + folder[:-1] + '.csv', sep = '\t')

	t = pd.Series({'Quantification': time.time() - start})
	t.to_csv(params.inputfolder + '../log/' + folder + 'Quantification.csv', sep = '\t')

	shape = tifffile.imread(
		params.inputfolder + folder + files[0][:-4] + '.tif').shape  # get image dimensions from the first image
	for i in range(len(files)):
		ind = np.load(params.inputfolder + '../segmented/labels/glomeruli/' + folder + files[i][:-4] + '.npy')
		lbl = ind[0]
		mask = np.ones(len(lbl), np.bool)
		mask[not lbl in llist] = 0
		lbl[mask] = 0
		lbl = np.reshape(lbl, shape)
		skimage.io.imsave(params.inputfolder + '../segmented/labels/glomeruli/' + params.folder + files[i][:-4] + '.tif',
						  lbl.astype(np.int))




def compute_stat_from_labels(labels, minvol, maxvol, stat, lmin, lmax, del_labels, istart, folder):
	'''
	Compute statistics for given range of labels (lmin, lmax) in a label stack
	'''
	print lmin, lmax
	start = time.time()
	xsize, zsize = extract_zoom(folder)
	llist = np.unique(labels)
	llist = llist[np.where((llist >= lmin)&(llist < lmax))]			

	print time.time() - start
	start = time.time()


	#compute the volumes of objects
	volumes = ndimage.sum((labels>0)*1, labels, llist)
	volumes = np.array(volumes)*xsize**2	#convert to microns

	print time.time() - start
	start = time.time()
	#remove objects with too small or too large volumes
	bv = llist[np.where((volumes >= maxvol)|(volumes <= minvol))]

	if len(bv) > 0:
		ix = np.in1d(labels.ravel(), bv).reshape(labels.shape)
		labels[ix] = 0
		del_labels = np.concatenate((del_labels, bv))

	print time.time() - start
	start = time.time()
	#update the arrays of volumes
	ind = np.where((volumes < maxvol)&(volumes > minvol))
	volumes = volumes[ind]
	llist = llist[ind]

	print time.time() - start
	start = time.time()
	#compute the diameters of the objects assuming the spherical shape
	sizes = 2* np.sqrt(volumes/np.pi)

	print time.time() - start
	start = time.time()
	#add statistics to the data frame
	if len(llist) > 0:
		curstat = pd.DataFrame({'Label':llist,
								'Diameter':sizes,
								'Volume':volumes})
		curstat['Image_name'] = folder[:-1]

		stat = pd.concat([stat, curstat], ignore_index = True)
	print time.time() - start
	start = time.time()
	return stat, del_labels
	
########################################################################3
#Statistics

def compute_stack_stat(statfile, params):

	#compute the number of cells
	stat = pd.DataFrame.from_csv(statfile, sep = '\t')

	cellnum = pd.DataFrame()


	for img in stat.Image_name.unique():
		curstat = stat[stat.Image_name == img]
		cellnum = cellnum.append(pd.Series({'Number_of_glomeruli': len(curstat), 
											'Image_name': curstat.Image_name.iloc[0],
											'Kidney_volume': curstat.Kidney_volume.iloc[0],
											'Mean_size':curstat.Diameter.mean(),
											'Stdev_of_size':curstat.Diameter.std()									
											}), ignore_index = True)

	cellnum = filelib.extract_groups(cellnum)

	cellnum.to_csv(params.inputfolder + '../statistics/stack_statistics.csv', sep = '\t')



def compute_size_distribution(statfile, params):

	stat = pd.DataFrame.from_csv(statfile, sep = '\t')

	absvalues = True


	#compute distribution of diameters


	dmin = int(round(2*float(params.minrad)))
	dmax = int(round(2*float(params.maxrad)))

	n = 5
	dmin = int(dmin)/n *n
	dmax = int(dmax)/n *n + n

	bins = np.linspace(dmin, dmax, num = (dmax-dmin)/n + 1) 
	bins = np.int_(bins)

	stat['Diameter, $\mu m$'] = stat.Diameter
	stat['Volume, x 1000 $\mu m^3$'] = stat.Volume /1000

	x = 'Diameter, $\mu m$'
	if absvalues:
		y = 'Number of glomeruli'
	else:
		y = 'Frequency'

	distr = pd.DataFrame()

	for img in stat.Image_name.unique():
		curstat = stat[stat.Image_name == img]	
		curdistr = pd.DataFrame()
		for i in range(1, len(bins)):
			curstat0 = curstat[(curstat[x] >= bins[i-1])&(curstat[x] < bins[i])]
			if absvalues:
				fr = len(curstat0)
			else:
				fr = len(curstat0)*1./len(curstat)
			curdistr = curdistr.append(pd.Series({x: bins[i-1] + (bins[i] - bins[i-1])/2., 
												y:fr}), ignore_index = True)

		curdistr['Image_name'] = curstat.Image_name.iloc[0]

		distr = pd.concat([distr, curdistr], ignore_index = True)
		
	distr = filelib.extract_groups(distr)

	distr.to_csv(params.inputfolder + '../statistics/diameter_distribution.csv', sep = '\t')

	maxvol = int(round(np.pi*float(params.maxrad)**2))
	minvol = int(round(np.pi*float(params.minrad)**2))

	n = 32
	dmin = int(minvol/1000)/n *n 
	dmax = int(maxvol/1000*0.4)/n *n + n
	bins = np.linspace(dmin, dmax, num = (dmax-dmin)/n + 1) 
	bins = np.int_(bins)

	x = 'Volume, x 1000 $\mu m^3$'


	distr = pd.DataFrame()

	for img in stat.Image_name.unique():
		curstat = stat[stat.Image_name == img]	
		curdistr = pd.DataFrame()
		for i in range(1, len(bins)):
			curstat0 = curstat[(curstat[x] >= bins[i-1])&(curstat[x] < bins[i])]
			if absvalues:
				fr = len(curstat0)
			else:
				fr = len(curstat0)*1./len(curstat)
			curdistr = curdistr.append(pd.Series({x: bins[i-1] + (bins[i] - bins[i-1])/2., 
												y: fr}), ignore_index = True)

		curdistr['Image_name'] = curstat.Image_name.iloc[0]

		distr = pd.concat([distr, curdistr], ignore_index = True)
		
	distr = filelib.extract_groups(distr)

	distr.to_csv(params.inputfolder + '../statistics/volume_distribution.csv', sep = '\t')






####################################################################

#read parameters from settings file
try:
	import argparse
	ap = argparse.ArgumentParser()
	ap.add_argument("-i","--input", required = True, help = "File with settings")

	args = ap.parse_args()
	settingsfile = args.input

except:
	settingsfile = 'settings.csv'

params = pd.Series.from_csv(settingsfile, sep = '\t')

#list folders with stacks to be analyzed
folders = list_subfolders(params.inputfolder, subfolders = []) 

params.max_threads = 1
#segment each stack
for folder in folders:
	segment(folder, params)


#label connected objects
params.max_threads = 1
boost.run_parallel(process = label_cells, files = folders, params = params, procname = 'Labelling')

#quantify the segmented data
boost.run_parallel(process = quantify, files = folders, params = params, procname = 'Quantification')

filelib.combine_statistics(params.inputfolder + '../statistics/', params.inputfolder  + '../statistics/statistics_combined.csv')

compute_stack_stat(params.inputfolder  + '../statistics/statistics_combined.csv', params)
compute_size_distribution(params.inputfolder  + '../statistics/statistics_combined.csv', params)





















