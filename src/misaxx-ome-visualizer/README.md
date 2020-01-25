**MISA++: A modular and high-performance framework for image analysis**

Ruman Gerst (1,2), Anna Medyukhina (1), Marc Thilo Figge(1,2,\*)

(1) Applied Systems Biology, Leibniz Institute for Natural Product Research and Infection Biology - Hans-Knöll-Institute, Jena, Germany

(2) Faculty of Biological Sciences, Friedrich-Schiller-University Jena, Germany

\* To whom correspondence should be addressed.

https://applied-systems-biology.github.io/misa-framework/

# About

The MISA++ OME Visualizer visualizes Int32 images produced by MISA++ applications.
The visualization assigns a color to each Int32 pixel value and generates
a RGB colored image.

The color assignment is global for the whole OME TIFF.

# Copyright

Copyright by Ruman Gerst

Research Group Applied Systems Biology - Head: Prof. Dr. Marc Thilo Figge

https://www.leibniz-hki.de/en/applied-systems-biology.html

HKI-Center for Systems Biology of Infection

Leibniz Institute for Natural Product Research and Infection Biology - Hans Knöll Insitute (HKI)

Adolf-Reichwein-Straße 23, 07745 Jena, Germany

The project code is licensed under BSD 2-Clause.
See the LICENSE file provided with the code for the full license.

# Dependencies

MISA++ Kidney Glomeruli Segmentation depends on following libraries:

| Library     | Version | Author      | URL |
| ----------- | ------- | ----------- | --- |
| MISA++ Core | 1.0.0   | Ruman Gerst |     |
| MISA++ OME  | 1.0.0   | Ruman Gerst |     |

You need a compiler capable of C++ version 2017 or higher to compile MISA++ Imaging.

# Building

The project uses the [CMake](https://cmake.org/) build system and requires no
additional build instructions that differ from the standard CMake building process.

# Running

You need to set `OME_HOME` to a folder that contains `shared/xml` and `shared/xsl`.
The folders are part of the OME Model installation and is required for loading OME TIFF files.

# Documentation

Please visit https://applied-systems-biology.github.io/misa-framework/ for full build instructions and documentation.
