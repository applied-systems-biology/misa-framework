+++
title = "C++ Framework"
weight = 20
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

The MISA++ (Modular image stack analysis for C++) framework allows the creation of C++ applications to process data. It features functions that simplify following common requirements of such applications:

* Automated parallelization of tasks
* Memory-efficient caching of data on hard-drive
* Providing additional algorithm parameters
* Modularization of common tasks
* Assigning quantitative results to data
* Integration into other tools and pipelines

MISA++ is used to develop *modules* that either provide additional data types and other utility functions (utility modules) or modules that are compiled into an application that solves a specific task (worker modules).

We also provide additional modules that integrate the open source imaging libraries [OpenCV](http://opencv.org/) and [OME Files](https://www.openmicroscopy.org/ome-files/) into our framework.

# All features

## Core library

### Creation of parallelizeable workloads

* Easy creation of parallelizable and atomic tasks
* Easy structuring of tasks into groups
* Dependency management between workloads
* No need for manual thread creation due to automated parallelization

### Standardized input parameters

* Highly structured and well standardized way to create algorithm parameters
* Different parameter namespaces
* * Algorithm-specific parameters
* * Sample-specific parameters
* * Global parameters

### Memory-efficient handling of large data sets

* Data-access via thread-safe caches
* Easy importing of input data
* Easy creation of output data
* Methods to easily create new cache types

### Assigning quantification results

* Quantification results can be easily attached to input or output data
* Well-defined, fully-automated and portable way to assign quantification results to data for external tools
* Predefined data cache that acts as shared storage for quantified data

### CMake build system integration

* Automated generation of installable shared libraries
* Automated generation of installable executables for worker modules

### Support for quantified data

* Matrix data type including mathematical operations
* Values with unit
* * Unit conversions
* * Higher order units

## Imaging capabilities

### OpenCV integration

* Single-image cache
* Image stack cache
* * Allows access to each single-image cache
* * Easy parallelization

### OME Files integration

* OME TIFF cache that allows access to individual OME TIFF planes
* * Can read OME TIFF format including metadata
* * Can write OME TIFF format including metadata
* Automated conversion from/to OpenCV images
* Read and write access to OME metadata such as microscopy parameters or physical size
* Support for OME quantity types
* * Including higher-order quantity types such as area and volume
* * Voxel/3D quantity types
