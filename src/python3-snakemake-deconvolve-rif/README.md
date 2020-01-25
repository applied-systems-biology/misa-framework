**MISA++: A modular and high-performance framework for image analysis**

Ruman Gerst (1,2), Anna Medyukhina (1), Marc Thilo Figge(1,2,\*)

(1) Applied Systems Biology, Leibniz Institute for Natural Product Research and Infection Biology - Hans-Knöll-Institute, Jena, Germany

(2) Faculty of Biological Sciences, Friedrich-Schiller-University Jena, Germany

\* To whom correspondence should be addressed.

https://applied-systems-biology.github.io/misa-framework/

# About

Segments cells with a distance transform watershed method.

# Copyright

Copyright by Ruman Gerst

Research Group Applied Systems Biology - Head: Prof. Dr. Marc Thilo Figge

https://www.leibniz-hki.de/en/applied-systems-biology.html

HKI-Center for Systems Biology of Infection

Leibniz Institute for Natural Product Research and Infection Biology - Hans Knöll Insitute (HKI)

Adolf-Reichwein-Straße 23, 07745 Jena, Germany

The project code is licensed under BSD 2-Clause.
See the LICENSE file provided with the code for the full license.

# Requirements

This project requires Python 3 and various dependency libraries.
You can use `pip` to install the required libraries:

```bash
pip install -r requirements.txt
```

# Usage

The program can be run with following command:

```bash
snakemake -j <num-threads> --config input=<input-directory> output=<output-directory>
```

The input directory must have following structure:

```
<input-directory>/<sample>/in/data.tif
<input-directory>/<sample>/psf/psf.tif
```

`psf.tif` files must be 32 bit floating point images.

There can be as many samples as required.
