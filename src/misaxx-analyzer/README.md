**MISA++: A modular and high-performance framework for image analysis**

Ruman Gerst (1,2), Anna Medyukhina (1), Marc Thilo Figge(1,2,\*)

(1) Applied Systems Biology, Leibniz Institute for Natural Product Research and Infection Biology - Hans-Knöll-Institute, Jena, Germany

(2) Faculty of Biological Sciences, Friedrich-Schiller-University Jena, Germany

\* To whom correspondence should be addressed.

https://applied-systems-biology.github.io/misa-framework/

# About
The MISA++ Result Analyzer processes the provided MISA++ output folder for
further analysis.

Following files are generated:

* `attachments/serialization-schemas-full.json` contains the serialization schemata for all attached objects and all child objects
* `attachment-index.sqlite` contains all attached objects and child objects as SQLite database

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

MISA++ Result Analyzer depends on following libraries:

| Library     | Version | Author            | URL                 |
| ----------- | ------- | ----------------- | ------------------- |
| SQLite      | 3       | SQLite Consortium | https://sqlite.org/ |
| MISA++ Core | 1.0.0   | Ruman Gerst       |                     |

You need a compiler capable of C++ version 2017 or higher to compile MISA++ Result Analyzer.

# Building

The project uses the [CMake](https://cmake.org/) build system and requires no
additional build instructions that differ from the standard CMake building process.

You might need to provide `SQLite3_INCLUDE_DIRS` and `SQLite3_LIBRARIES` if
CMake is not able to find SQLite 3.

# Documentation

Please visit https://applied-systems-biology.github.io/misa-framework/ for full build instructions and documentation.
