# -*- coding: utf-8 -*-
'''
Auxiliary functions for working with files

:Author:
  `Anna Medyukhina`_
  email: anna.medyukhina@leibniz-hki.de or anna.medyukhina@gmail.com	

:Organization:
  Applied Systems Biology Group, Leibniz Institute for Natural Product Research and Infection Biology - Hans Knöll Institute (HKI)

:Version: 2015.11.16

Copyright (c) 2014-2015, 
Leibniz Institute for Natural Product Research and Infection Biology – 
Hans Knöll Institute (HKI)

Licence: BSD-3-Clause, see ./LICENSE or 
https://opensource.org/licenses/BSD-3-Clause for full details

Requirements
------------
* `Python 2.7.3  <http://www.python.org>`_
* `pandas 0.15.2 <http://pandas.pydata.org>`_

'''

import os, sys, re
import pandas as pd



global image_extensions
image_extensions=['png','jpg','jpeg','bmp','PNG','JPG','JPEG','BMP','tif','TIFF','tiff','TIF', 'npy']



def list_image_files(inputfolder):
	'''
lists image files in an input folder
returns list of image files with paths to them
	'''
	global image_extenstions
	files=os.listdir(inputfolder)
	imgfiles=[]
	for i_file in files:
		parts=i_file.split(".")
		extension=parts[-1]
		if (extension in image_extensions):
			imgfiles.append(i_file)
	return imgfiles


def list_npy_files(inputfolder):
	'''
lists image files in an input folder
returns list of image files with paths to them
	'''
	extensions = ['npy']
	files=os.listdir(inputfolder)
	imgfiles=[]
	for i_file in files:
		parts=i_file.split(".")
		extension=parts[-1]
		if (extension in extensions):
			imgfiles.append(i_file)
	return imgfiles


def is_in_extensions(filename, extensions):
	
	if filename.split('.')[-1] in extensions:
		return True
	else:
		return False




def list_subfolders(inputfolder, subfolder = '', subfolders = [], extensions = []):
	'''
	recursive listing of folders
	'''

	if len(extensions) == 0:
		global image_extensions
		extensions = image_extensions

	files = os.listdir(inputfolder + subfolder)
	for path in files:
		if os.path.isdir(inputfolder + subfolder + path):
			if path[-1] != '/':
				path = path + '/'
			subfolders = list_subfolders(inputfolder,  subfolder = subfolder + path, subfolders = subfolders, extensions = extensions)
		else:
			if is_in_extensions(path, extensions):
				subfolders.append(subfolder + path)

	return subfolders


def make_folders(folders):
	''' creates non existing folder from the input list'''
	for folder in folders:
		if not os.path.exists(folder):
			try:
				os.makedirs(folder)
			except:
				pass


def return_path(filename):
	fn = filename.split('/')[-1]
	return filename[:-len(fn)]


def list_csv_files(folder):
	files = os.listdir(folder)
	csvfiles = []
	for f in files:
		if f.split('.')[-1] == 'csv':
			csvfiles.append(f)
	return csvfiles


def combine_statistics(inputfolder, outputfile):
	make_folders([return_path(outputfile)])
	subfolders = list_subfolders(inputfolder, subfolders = [], extensions = ['csv'])

	data = pd.DataFrame()

	for sf in subfolders:
		curdata = pd.DataFrame.from_csv(inputfolder + sf, sep = '\t')
		data = pd.concat([data, curdata], ignore_index = True)


	data.to_csv(outputfile, sep = '\t')
		


def combine_statistics_arr(inputfolders, filenames):
	for i in range(len(inputfolders)):
		inputfolder = inputfolders[i]
		filename = filenames[i]
		combine_statistics(inputfolder, inputfolder + filename)


def extract_groups(stat):
	group = []
	for i in range(len(stat)):
		group.append(stat.iloc[i].Image_name.split('/')[0])
	stat['Group'] = group

	return stat






























