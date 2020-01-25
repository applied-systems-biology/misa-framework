# -*- coding: utf-8 -*-
'''
Counting glomeruli in Light-Sheet microscopy images of kidney. 
Full details of the alogrithm can be found in the paper 
Klingberg et al. (2017) Fully Automated Evaluation of Total Glomerular Number and 
Capillary Tuft Size in Nephritic Kidneys Using Lightsheet Microscopy,
J. Am. Soc. Nephrol., 28: 452-459. 

For running in command line: ``python compute_volume.py -i settings.csv``

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

import re, os
import pandas as pd
import numpy as np
import mahotas
from scipy import ndimage
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


def overlay(mask, img, color, borders = True, normalize = True):	
	if borders:	
		borders = mahotas.borders((mask).astype(np.uint8))
	else:
		borders = mask
	ind = np.where(borders)
	if normalize and img.max() > 0:
		output = img*255./img.max()
	else:
		output = np.zeros_like(img)

	output[ind] = color
	return output


################################################################################
#Segmentation

def segment(folder, params):
	'''
	Segment all layers in a folder
	'''

	#create folders for the output
	filelib.make_folders([params.inputfolder + '../segmented/outlines/' + folder, params.inputfolder + '../segmented/masks/' + folder])

		
	#list all files in the folder
	files = filelib.list_image_files(params.inputfolder + folder)
	files.sort()
	ind = np.int_(np.arange(0, len(files), 10))
	files = np.array(files)[ind]

	if not len(filelib.list_image_files(params.inputfolder + '../segmented/masks/' + folder)) == len(files):
		params.folder = folder

		#segment all layers in parallel
		boost.run_parallel(process = segment_layer, files = files, params = params, procname = 'Segmentation of glomeruli')


def segment_layer(filename, params):
	'''
	Segment one layer in a stack
	'''
	#extract pixel size in xy and z
	xsize, zsize = extract_zoom(params.folder)

	#load image
	img = tifffile.imread(params.inputfolder + params.folder + filename)

	#normalize image
	img = ndimage.median_filter(img, 3)
	per_low = np.percentile(img, 5)
	img[img < per_low] = per_low
	img = img - img.min()

	per_high = np.percentile(img, 99)
	img[img > per_high] = per_high
	img = img*255./img.max()


	imgf = ndimage.gaussian_filter(img*1., 30./xsize).astype(np.uint8)

	kmask = (imgf > mahotas.otsu(imgf.astype(np.uint8)))*255.

	sizefactor = 10
	small = ndimage.interpolation.zoom(kmask, 1./sizefactor)	#scale the image to a smaller size

	rad = int(300./xsize)

	small_ext = np.zeros([small.shape[0] + 4*rad, small.shape[1] + 4*rad])
	small_ext[2*rad : 2*rad + small.shape[0], 2*rad : 2*rad + small.shape[1]] = small

	small_ext = mahotas.close(small_ext.astype(np.uint8), mahotas.disk(rad))
	small = small_ext[2*rad : 2*rad + small.shape[0], 2*rad : 2*rad + small.shape[1]]
	small = mahotas.close_holes(small)*1.			
	small = small*255./small.max()

	kmask = ndimage.interpolation.zoom(small, sizefactor)	#scale back to normal size
	kmask = normalize(kmask)
	kmask = (kmask > mahotas.otsu(kmask.astype(np.uint8)))*255.	#remove artifacts of interpolation

	if np.median(imgf[np.where(kmask > 0)]) < (np.median(imgf[np.where(kmask == 0)]) + 1)*3:
		kmask = np.zeros_like(kmask)


	#save indices of the kidney mask
#	ind = np.where(kmask > 0)
#	ind = np.array(ind)
#	np.save(params.inputfolder + '../segmented/masks/' + params.folder + filename[:-4] + '.npy', ind)

	#save outlines
	im = np.zeros([img.shape[0], img.shape[1], 3])
	img = tifffile.imread(params.inputfolder + params.folder + filename)
	im[:,:,0] = im[:,:,1] = im[:,:,2] = np.array(img)
	output = overlay(kmask, im, (255,0,0), borders = True)
	tifffile.imsave(params.inputfolder + '../segmented/outlines/' + params.folder + filename[:-4] + '.tif', (output).astype(np.uint8))

	
#############################################################################
#Quantification

def quantify(folder, params):
	'''
	Quantify a stack
	'''
	if not os.path.exists(params.inputfolder + '../statistics/' + folder[:-1] + '.csv'):
	
		#list files in the folder
		files = filelib.list_image_files(params.inputfolder + folder)
		files.sort()

		#create a folder for statistics
		filelib.make_folders([params.inputfolder + '../statistics/' + filelib.return_path(folder[:-1] + '.csv')])

		#extract voxel size
		xsize, zsize = extract_zoom(folder)

		#compute volume of the kidney
		kidney_volume = 0

		for i in range(len(files)):
			ind = np.load(params.inputfolder + '../segmented/masks/' + folder + files[i][:-4] + '.npy')
			kidney_volume = kidney_volume + len(ind[0])


		stat = pd.DataFrame()		
		stat['Kidney_volume'] = [kidney_volume*xsize**2*zsize]
		stat['Image_name'] = folder[:-1]
		stat.to_csv(params.inputfolder + '../statistics/' + folder[:-1] + '.csv', sep = '\t')




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

#segment each stack
for folder in folders:
	print folder
	segment(folder, params)

#quantify the segmented data
boost.run_parallel(process = quantify, files = folders, params = params, procname = 'Quantification')

filelib.combine_statistics(params.inputfolder + '../statistics/', params.inputfolder  + '../statistics_combined.csv')



































