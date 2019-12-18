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
* Example implementations of image analysis algorithms (Tissue segmentation, Kidney glomeruli segmenation, OME visualizer, and more)
* ImageJ plugin binaries, including dependency libraries

{{< button href="https://github.com/applied-systems-biology/misa-framework/releases/download/1.0.0/misaxx-linux-ubuntu-18.04.zip" theme="success" >}} Download MISA++ for Ubuntu 18.04{{< /button >}} {{< button href="https://github.com/applied-systems-biology/misa-framework/releases/download/1.0.0/misaxx-linux-ubuntu-19.10.zip" theme="success" >}} Download MISA++ for Ubuntu 19.10{{< /button >}}

You can install the \*.deb packages using `sudo apt install ./*.deb`. Put the plugin files (located in the `misa-imagej` folder)
into an existing [Fiji](https://fiji.sc/) installation to install the ImageJ plugin.

# Windows

We provide a ready-to-use [Fiji](https://fiji.sc/) distribution that comes pre-installed with
**MISA++ for ImageJ**, *MISA++ Kidney Glomeruli Segmentation*,
*MISA++ Tissue Segmentation*, *MISA++ OME Visualizer*, *MISA++ Microbenchmarks*, *MISA++ Deconvolution Simulation*, *MISA++ Cell Segmentation* and *MISA++ Result Analyzer*.

Alternatively, you can download only the MISA++ binaries.

{{< button href="https://github.com/applied-systems-biology/misa-framework/releases/download/1.0.0/misaxx-fiji-aio-windows.zip" theme="success" >}} Download MISA+Fiji for Windows x64{{< /button >}} {{< button href="https://github.com/applied-systems-biology/misa-framework/releases/download/1.0.0/misaxx-bin-windows.zip" theme="success" >}} Download MISA++ binaries for Windows x64{{< /button >}}

{{% notice info %}}MISA++ is developed on Linux - We consider this Windows release as experimental. We are aware of a bug that causes freezes of multi-threaded MISA++ workloads if executed from within ImageJ.
As workaround, export MISA++ runs as standalone package and run MISA++ via the command line interface.{{% /notice %}}

{{% notice info %}}Please make sure that your anti virus protection does not block the MISA++ application. Try refreshing the list of modules if a module could not be loaded.{{% /notice %}}

# Source code

The software is Open Source and licensed under [BSD-2-Clause](https://opensource.org/licenses/BSD-2-Clause).
You can find the source codes of the MISA++ framework, modules and applications
under following links:

| Library                              | Source code                                                                                                             | Class reference                                                    |
| ------------------------------------ | ----------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------ |
| MISA++ Core                          | <a href="https://github.com/applied-systems-biology/misaxx-core" target="_blank"> Download source code </a>             | <a href="/docs/misaxx-core" target="_blank"> Open </a>             |
| MISA++ Imaging                       | <a href="https://github.com/applied-systems-biology/misaxx-imaging" target="_blank"> Download source code </a>          | <a href="/docs/misaxx-imaging" target="_blank"> Open </a>          |
| MISA++ Microbenchmarks               | <a href="https://github.com/applied-systems-biology/misaxx-microbench" target="_blank"> Download source code</a>        |                                                                    |
| MISA++ Deconvolution Simulation      | <a href="https://github.com/applied-systems-biology/misaxx-deconvolve" target="_blank"> Download source code</a>        |                                                                    |
| MISA++ Cell Segmentation             | <a href="https://github.com/applied-systems-biology/misaxx-segment-cells" target="_blank"> Download source code</a>     |                                                                    |
| MISA++ OME                           | <a href="https://github.com/applied-systems-biology/misaxx-ome" target="_blank"> Download source code </a>              | <a href="/docs/misaxx-ome" target="_blank"> Open </a>              |
| MISA++ OME Visualizer                | <a href="https://github.com/applied-systems-biology/misaxx-ome-visualizer" target="_blank"> Download source code </a>   | <a href="/docs/misaxx-ome-visualizer" target="_blank"> Open </a>   |
| MISA++ Tissue Segmentation           | <a href="https://github.com/applied-systems-biology/misaxx-tissue" target="_blank"> Download source code </a>           | <a href="/docs/misaxx-tissue" target="_blank"> Open </a>           |
| MISA++ Kidney Glomeruli Segmentation | <a href="https://github.com/applied-systems-biology/misaxx-kidney-glomeruli" target="_blank"> Download source code </a> | <a href="/docs/misaxx-kidney-glomeruli" target="_blank"> Open </a> |
| MISA++ Result Analyzer               | <a href="https://github.com/applied-systems-biology/misaxx-analyzer" target="_blank"> Download source code </a>         | <a href="/docs/misaxx-analyzer" target="_blank"> Open </a>         |
| MISA++ for ImageJ                    | <a href="https://github.com/applied-systems-biology/misa-imagej" target="_blank"> Download source code </a>             |                                                                    |


```bash
git clone https://github.com/applied-systems-biology/misaxx-core.git
git clone https://github.com/applied-systems-biology/misaxx-imaging.git
git clone https://github.com/applied-systems-biology/misaxx-microbench.git
git clone https://github.com/applied-systems-biology/misaxx-deconvolve.git
git clone https://github.com/applied-systems-biology/misaxx-segment-cells.git
git clone https://github.com/applied-systems-biology/misaxx-ome.git
git clone https://github.com/applied-systems-biology/misaxx-ome-visualizer.git
git clone https://github.com/applied-systems-biology/misaxx-tissue.git
git clone https://github.com/applied-systems-biology/misaxx-kidney-glomeruli.git
git clone https://github.com/applied-systems-biology/misaxx-analyzer.git
git clone https://github.com/applied-systems-biology/misa-imagej.git
```

## Linux docker builds

We use automated build scripts to compile Linux packages. These scripts use a
Docker environment and can be found [here](https://github.com/applied-systems-biology/misaxx-utils/tree/master/linux-builds).

## Windows Cygwin builds (experimental)

To build MISA++ on Windows, we use the [Cygwin](https://cygwin.com/) environment.
Our script can be found [here](https://github.com/applied-systems-biology/misaxx-utils/tree/master/windows-builds).

# Logos

Download following package for the MISA++ logos:

{{< button href="https://github.com/applied-systems-biology/misa-framework/releases/download/1.0.0/misaxx_logos.zip" theme="success" >}} Download Logos{{< /button >}}

# Example data

Our [step by step guide](/imagej/step-by-step) requires you to download example data:

{{< button href="https://github.com/applied-systems-biology/misa-framework/releases/download/1.0.0/misaxx_example_data.zip" theme="success" >}} Download example data{{< /button >}}

The example data was published by Klingberg *et. al*

> Klingberg, Anika, et al. "Fully automated evaluation of total glomerular number and capillary tuft size in nephritic kidneys using lightsheet microscopy."
Journal of the American Society of Nephrology 28.2 (2017): 452-459.

## Other example data

If you want to try out other algorithms (as well as Python and Java implementations), you can find appropriate example data here:

| Algorithm                             | Download                                                                                                                                                 |
| ------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Glomeruli segmentation (MISA++)       | [Download example data](https://github.com/applied-systems-biology/misa-framework/releases/download/1.0.0/misaxx_kidney_glomeruli_example_data.zip)      |
| Glomeruli segmentation (Python, Java) | [Download example data](https://github.com/applied-systems-biology/misa-framework/releases/download/1.0.0/python_java_kidney_glomeruli_example_data.zip) |
| Cell segmentation (MISA++)            | [Download example data](https://github.com/applied-systems-biology/misa-framework/releases/download/1.0.0/misaxx_segment_cells_example_data.zip)         |
| Cell segmentation (Python, Java)      | [Download example data](https://github.com/applied-systems-biology/misa-framework/releases/download/1.0.0/python_java_segment_cells_example_data.zip)    |
| Deconvolution                         | [Download example data](https://github.com/applied-systems-biology/misa-framework/releases/download/1.0.0/misaxx_deconvolve_example_data.zip)            |
| Single operation benchamrks           | [Download example data](https://github.com/applied-systems-biology/misa-framework/releases/download/1.0.0/misaxx_microbench_example_data.zip)            |

# Publication supplements

In our publication, we compare MISA++ against Java and Python implementations. You can find the compiled Java binaries [here](https://github.com/applied-systems-biology/misa-framework/releases/download/1.0.0/aio-java-imglib2-bin.zip).
All source codes are available in our [all-in-one code repository](https://github.com/applied-systems-biology/misaxx-softwarex-code).
