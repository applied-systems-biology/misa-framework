# -*- coding: utf-8 -*-
'''
Auxiliary functions for parallel image processing

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

'''

from multiprocessing import Process
import time


def print_progress(procdone, totproc, start):
	donepercent = procdone*100/totproc
	elapse = time.time() - start
	tottime = totproc*1.*elapse/procdone
	left = tottime - elapse
	units = 'sec'
	if left > 60:
		left = left/60.
		units = 'min'
		if left > 60:
			left = left/60.
			units = 'hours'		
	
	
	print 'done', procdone, 'of', totproc, '(', donepercent, '% ), approx. time left: ', left, units 


def run_parallel(process, files, params, procname):

	print 'Run', procname

	procs = []

	totproc = len(files)
	procdone = 0
	start = time.time()
	print 'Started at ', time.ctime()

	for i in range(len(files)):
		filename = files[i]

		while int(len(procs)) >= int(params.max_threads):
			time.sleep(0.005)
			for p in procs:
				if not p.is_alive():
					procs.remove(p)
					procdone +=1
					print_progress(procdone, totproc, start)

		p = Process(target=process, args=(filename, params))
		p.start()
		procs.append(p)

	while len(procs) > 0:
		for p in procs:
			if not p.is_alive():
				procs.remove(p)
				procdone +=1
				print_progress(procdone, totproc, start)

	print procname, 'done'














