**MISA++: A modular and high-performance framework for image analysis**

Ruman Gerst (1,2), Anna Medyukhina (1), Marc Thilo Figge(1,2,\*)

(1) Applied Systems Biology, Leibniz Institute for Natural Product Research and Infection Biology - Hans-Knöll-Institute, Jena, Germany

(2) Faculty of Biological Sciences, Friedrich-Schiller-University Jena, Germany

\* To whom correspondence should be addressed.

https://applied-systems-biology.github.io/misa-framework/

# About

The MISA++ Core library is the basis of any MISA++ module and application.
It provides standards for parallelization, data and memory management,
parameters, command line interface, quantification data and interaction
with the MISA++ application.

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

MISA++ Core depends on following libraries:

| Library             | Version         | Author          | URL                               |
| ------------------- | --------------- | --------------- | --------------------------------- |
| JSON for modern C++ | 3.6.1 or higher | Niels Lohmann   | https://github.com/nlohmann/json/ |
| Boost               | 1.63 or higher  | Boost Community | https://www.boost.org/            |
| OpenMP              | 4.5             | OpenMP ARB      | https://www.openmp.org/           |

You need a compiler capable of C++ version 2017 or higher to compile MISA++ Core.

# Building

MISA++ core uses the [CMake](https://cmake.org/) build system and requires no
additional build instructions that differ from the standard CMake building process.

# Documentation

Please visit https://applied-systems-biology.github.io/misa-framework/ for full build instructions and documentation.
