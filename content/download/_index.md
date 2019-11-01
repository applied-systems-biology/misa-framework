+++
title = "Download"
weight = 30
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

# All platforms: ImageJ plugin

If you only want the plugin or are using an operating system other than Windows,
download following package:

{{< button href="https://github.com/applied-systems-biology/misa-framework/releases/download/1.0.0/misa-imagej-plugin.zip" theme="success" >}} Download plugin (Windows, Linux) {{< /button >}}

{{% notice info %}}This download contains only the plugin and dependency libraries. Please do not forget to obtain compiled MISA++ packages for your operating system
that are interfaced with the plugin.{{% /notice %}}

# Linux

We provide ready-to-install packages for Ubuntu 18.04 and Ubuntu 19.10 that contains following components:

* MISA++ core components (MISA++ Core, MISA++ Analyzer)
* Example implementations of third-party library integrations (OpenCV, OME TIFF)
* Example implementations of image analysis algorithms (Tissue segmentation, Kidney glomeruli segmenation, OME visualizer)
* ImageJ plugin binaries, including dependency libraries

{{< button href="https://github.com/applied-systems-biology/misa-framework/releases/download/1.0.0/misaxx-linux-ubuntu-18.04.zip" theme="success" >}} Download MISA++ for Ubuntu 18.04{{< /button >}} {{< button href="https://github.com/applied-systems-biology/misa-framework/releases/download/1.0.0/misaxx-linux-ubuntu-19.10.zip" theme="success" >}} Download MISA++ for Ubuntu 19.10{{< /button >}}

You can install the \*.deb packages using `sudo apt install ./*.deb`. Put the plugin files (located in the `misa-imagej` folder)
into an existing [Fiji](https://fiji.sc/) installation to install the ImageJ plugin.

# Windows

We provide a ready-to-use [Fiji](https://fiji.sc/) distribution that comes pre-installed with
**MISA++ for ImageJ**, *MISA++ Kidney Glomeruli Segmentation*,
*MISA++ Tissue Segmentation*, *MISA++ OME Visualizer* and *MISA++ Result Analyzer*.

{{< button href="https://github.com/applied-systems-biology/misa-framework/releases/download/1.0.0/misa-imagej-fiji-standalone-win32.zip" theme="success" >}} Download MISA+Fiji for Windows{{< /button >}}

{{% notice info %}}MISA++ is developed on Linux - We consider this Windows release as experimental. We are aware of a bug that prevents the execution of compiled *.exe files.
Under certain circumstances, pressing "Refresh" in the list of MISA++ modules solves this issue.{{% /notice %}}

# Source code

The software is Open Source and licensed under [BSD-2-Clause](https://opensource.org/licenses/BSD-2-Clause).
You can find the source codes of the MISA++ framework, modules and applications
under following links:

| Library                              | Source code                                                                                                             | Class reference                                                    |
| ------------------------------------ | ----------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------ |
| MISA++ Core                          | <a href="https://github.com/applied-systems-biology/misaxx-core" target="_blank"> Download source code </a>             | <a href="/docs/misaxx-core" target="_blank"> Open </a>             |
| MISA++ Imaging                       | <a href="https://github.com/applied-systems-biology/misaxx-imaging" target="_blank"> Download source code </a>          | <a href="/docs/misaxx-imaging" target="_blank"> Open </a>          |
| MISA++ OME                           | <a href="https://github.com/applied-systems-biology/misaxx-ome" target="_blank"> Download source code </a>              | <a href="/docs/misaxx-ome" target="_blank"> Open </a>              |
| MISA++ OME Visualizer                | <a href="https://github.com/applied-systems-biology/misaxx-ome-visualizer" target="_blank"> Download source code </a>   | <a href="/docs/misaxx-ome-visualizer" target="_blank"> Open </a>   |
| MISA++ Tissue Segmentation           | <a href="https://github.com/applied-systems-biology/misaxx-tissue" target="_blank"> Download source code </a>           | <a href="/docs/misaxx-tissue" target="_blank"> Open </a>           |
| MISA++ Kidney Glomeruli Segmentation | <a href="https://github.com/applied-systems-biology/misaxx-kidney-glomeruli" target="_blank"> Download source code </a> | <a href="/docs/misaxx-kidney-glomeruli" target="_blank"> Open </a> |
| MISA++ Result Analyzer               | <a href="https://github.com/applied-systems-biology/misaxx-analyzer" target="_blank"> Download source code </a>         | <a href="/docs/misaxx-analyzer" target="_blank"> Open </a>         |
| MISA++ for ImageJ                    | <a href="https://github.com/applied-systems-biology/misa-imagej" target="_blank"> Download source code </a>             |                                                                    |


```bash
git clone https://github.com/applied-systems-biology/misaxx-core.git
git clone https://github.com/applied-systems-biology/misaxx-imaging.git
git clone https://github.com/applied-systems-biology/misaxx-ome.git
git clone https://github.com/applied-systems-biology/misaxx-ome-visualizer.git
git clone https://github.com/applied-systems-biology/misaxx-tissue.git
git clone https://github.com/applied-systems-biology/misaxx-kidney-glomeruli.git
git clone https://github.com/applied-systems-biology/misaxx-analyzer.git
git clone https://github.com/applied-systems-biology/misa-imagej.git
```

## Linux docker builds

We use automated build scripts to compile Linux packages. These scripts use a
Docker environment and can be found [here](https://github.com/applied-systems-biology/misaxx-utils/linux-builds).

## Windows MSYS2 builds (experimental)

To build MISA++ on Windows, we use the [MSYS2](https://msys2.org/) environment.
Our script can be found [here](https://github.com/applied-systems-biology/misaxx-utils/linux-builds).

{{% notice info %}}The Windows build is currently experimental. The build process might fail due to changes in MSYS2.
We also experience issues in compiled executabled failing to load dynamic libraries, causing crashes.{{% /notice %}}

# Logos

Download following package for the MISA++ logos:

{{< button href="https://github.com/applied-systems-biology/misa-framework/releases/download/1.0.0/misaxx_logos.zip" theme="success" >}} Download Logos{{< /button >}}

# Example data

Our [step by step guide](/imagej/step-by-step) requires you to download example data:

{{< button href="https://github.com/applied-systems-biology/misa-framework/releases/download/1.0.0/misaxx_example_data.zip" theme="success" >}} Download example data{{< /button >}}

The example data was published by Klingberg *et. al*

> Klingberg, Anika, et al. "Fully automated evaluation of total glomerular number and capillary tuft size in nephritic kidneys using lightsheet microscopy."
Journal of the American Society of Nephrology 28.2 (2017): 452-459.
