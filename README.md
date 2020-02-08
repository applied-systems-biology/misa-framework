# MISA++: A standardized interface for automated bioimage analysis
[doi.org/10.1016/j.softx.2020.100405](https://doi.org/10.1016/j.softx.2020.100405)

Ruman Gerst (1,2), Anna Medyukhina (1), Marc Thilo Figge(1,2,\*)

(1) Applied Systems Biology, Leibniz Institute for Natural Product Research and Infection Biology - Hans-Knöll-Institute, Jena, Germany

(2) Faculty of Biological Sciences, Friedrich-Schiller-University Jena, Germany

\* To whom correspondence should be addressed.

https://applied-systems-biology.github.io/misa-framework/

# About

This repository contains the MISA++ framework website and binary downloads.
The subfolder "src" contains all source codes associated to the publication.
The subfolder "docs" contains the website, including documentation as HTML site.

# Copyright

Copyright by Ruman Gerst

Research Group Applied Systems Biology - Head: Prof. Dr. Marc Thilo Figge

https://www.leibniz-hki.de/en/applied-systems-biology.html

HKI-Center for Systems Biology of Infection

Leibniz Institute for Natural Product Research and Infection Biology - Hans Knöll Insitute (HKI)

Adolf-Reichwein-Straße 23, 07745 Jena, Germany

The project code is licensed under BSD 2-Clause.
See the LICENSE file provided with the code for the full license.

# Editing the website

The website is built using [Hugo](https://gohugo.io/) that builds the final website into the `docs` folder.
Use following command to obtain the website source:

```bash
git clone https://github.com/applied-systems-biology/misa-framework.git
git submodule init
git submodule update
```

To preview the website during development, run following command:
```bash
hugo server
```

To generate the final website, run following command:

```bash
hugo
```
