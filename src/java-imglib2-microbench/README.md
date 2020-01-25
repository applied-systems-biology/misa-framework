**MISA++: A modular and high-performance framework for image analysis**

Ruman Gerst (1,2), Anna Medyukhina (1), Marc Thilo Figge(1,2,\*)

(1) Applied Systems Biology, Leibniz Institute for Natural Product Research and Infection Biology - Hans-Knöll-Institute, Jena, Germany

(2) Faculty of Biological Sciences, Friedrich-Schiller-University Jena, Germany

\* To whom correspondence should be addressed.

https://applied-systems-biology.github.io/misa-framework/

# About

Runs common image processing algorithms on the input images.

Example data: https://github.com/applied-systems-biology/misa-framework/releases/download/1.0.0/misaxx_microbench_example_data.zip

# Copyright

Copyright by Ruman Gerst

Research Group Applied Systems Biology - Head: Prof. Dr. Marc Thilo Figge

https://www.leibniz-hki.de/en/applied-systems-biology.html

HKI-Center for Systems Biology of Infection

Leibniz Institute for Natural Product Research and Infection Biology - Hans Knöll Insitute (HKI)

Adolf-Reichwein-Straße 23, 07745 Jena, Germany

The project code is licensed under BSD 2-Clause.
See the LICENSE file provided with the code for the full license.

# Building

This project requires Maven (http://maven.apache.org/) and Java 8.
Java 9 or higher does not work. If you need Java version 8, you can find it here: https://adoptopenjdk.net/

Run following command in a terminal: 

```bash
mvn compile
mvn package
```

The compiled binaries will be placed in the `target` directory.

# Usage

The program has following parameters:

* `--input` the input directory
* `--output` the output directory
* `--threads <number>` number of threads

The input directory must have following structure:

```
<input-directory>/<sample>/in/data.tif
```

There can be as many samples as required.
