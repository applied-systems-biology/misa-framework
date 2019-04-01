+++
title = "Concepts"
weight = 1

# Type of content, set "slide" to display it fullscreen with reveal.js
type="page"

# Creator's Display name
creatordisplayname = "Ruman Gerst"
# Creator's Email
creatoremail = "ruman.gerst@leibniz-hki.de"
# LastModifier's Display name
lastmodifierdisplayname = "Ruman Gerst"
# LastModifier's Email
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++


This page explains various concepts that are useful to know if you want to use the framework as user or developer.

**For users**

* Parameter.json
* Filesystem & caches
* Parameter schema

**For developers**

* Module (utility/worker)
* Module interface
* Module dispatcher
* Tasks & dispatchers
* Cache linking

# Parameter.json

The parameter file provides the necessary information for a module executable to do its work. It contains all algorithm parameters, determines the location of input and output files and defines the samples.

# Filesystem & caches

Input and output data is organized in "caches", which are folders containing the data. While the application is running, it will read and/or write from/to those locations on-demand. The module defines a structure of folders where it expects the data to be located or where the data will be written. If you want to provide input data, you have to place your data into this predefined "filesystem".

# Parameter schema

Each module executable can generate a description of its parameters and caches. This "Parameter schema" is in JSON format and can be used as guide to look up which parameters are available or as way for external scripts and programs to automatically interface with a MISA++ module.

# Module

A module is a shared library (.so/.dll) that has a public API according to the MISA++ Module Standard. This means that the API is structured in a specific way and provides a `misa_module_info` instance that identifies the module and its dependencies.
We call modules that just provide additional APIs "utility modules" and modules that can be compiled into an executable a "worker module".

# Module interface

A class that defines the caches and additional public functions of a worker module. It is part of the public API of a worker module.

# Module dispatcher

A class that is the implementation of a module interface. It is the starting point to define the hierarchy of parallelized workers to solve the task of the worker module.

# Tasks & Dispatchers

MISA++ provides functionality to create programs that solve tasks with multithreading, while making it easy to create dependencies between workloads or groups of workloads. The framework distinguishes between two types of workloads:

* Atomic, parallelizable tasks. They do the actual work
* Dispatchers that create tasks or other dispatchers

The module dispatcher for example is a dispatcher

# Cache linking

Internally, a cache consists of a physical location in the filesystem and a representation of its data in the program (for example `TIFF file <-> an OpenCV image`). The process of assigning a physical location to its C++ representation is called "linking".

It involves invoking a *pattern* recognition that produces a generic *description* of the folder contents. The benefit of such a system of `pattern -> description -> cache link` is that the description can be reused to create an output cache from an input cache.
