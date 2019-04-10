+++
title = "Building"
weight = 10
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

MISA++ uses the [CMake](https://cmake.org/) build system (version 3.12 or higher) and is designed around
the GCC compiler. We tested compilation under GCC version 7.3.

If you want to use another compiler, check if it supports C++ 2017 or higher and
OpenMP 4.5 or higher. We only used OpenMP 3.x features, so using an older OpenMP
version might work.

You will also need following additional libraries:

| Library             | Version | Notes                                                                   |
| ------------------- | ------- | ----------------------------------------------------------------------- |
| Boost               | 1.67    |                                                                         |
| SQLite              | 3       |                                                                         |
| OME files           | 0.5.0   | See https://www.openmicroscopy.org/ for download and build instructions |
| OpenCV              | 4.0     | OpenCV 3.x might also work                                              |
| JSON for Modern C++ | 3.5.0   | See https://github.com/nlohmann/json                                    |

The MISA++ libraries and applications we provide depend on each other:

{{<mermaid align="center">}}
graph TB;
A[MISA++ Core] --> B[MISA++ Imaging]
A --> C[MISA++ Result Analyzer]
B --> D[MISA++ OME]
D --> E["MISA++ Tissue Segmentation"]
E --> F["MISA++ Kidney Glomeruli Segmentation"]
B --> G["MISA++ OME Visualizer"]
{{< /mermaid >}}

Please make sure to build the dependencies the correct order.
