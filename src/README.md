# MISA++: standardized interface for automated bioimage analysis

This folder contains all code related to the publication.

## MISA++ vs Java vs Python comparison projects

Following projects were used to compare the performance between MISA++, Python, and Java:

### Glomeruli analysis

| Project                             | Description                                                                                                                             |
| ----------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------- |
| misaxx-kidney-glomeruli             | MISA++ implementation.                                                                                                                  |
| imglib2-segment-glomeruli           | Java implementation.                                                                                                                    |
| python2-custom-segment-glomeruli    | Original Python implementation published by Klingberg et al. 2017                                                                       |
| python3-snamemake-segment-glomeruli | Python implementation adapted to a Snakemake application. Snakemake uses a DAG for parallelization, unlike the original implementation. |

### Cell segmentation

| Project                         | Description            |
| ------------------------------- | ---------------------- |
| misaxx-segment-cells            | MISA++ implementation. |
| imglib2-segment-cells           | Java implementation.   |
| python3-snakemake-segment-cells | Python implementation. |

### Deconvolution

| Project                          | Description                                                        |
| -------------------------------- | ------------------------------------------------------------------ |
| misaxx-deconvolve                | MISA++ implementation.                                             |
| java-imglib2-deconvolve-rif      | Java implementation. It uses DeconvolutionLab2 for the processing. |
| python3-snakemake-deconvolve-rif | Python implementation.                                             |

### Single operation benchmarks (Microbenchmarks)

| Project                      | Description            |
| ---------------------------- | ---------------------- |
| misaxx-microbench            | MISA++ implementation. |
| java-imglib2-microbench      | Java implementation.   |
| python3-snamemake-microbench | Python implementation. |

## ImageJ plugin projects

Following projects are related to the MISA++ ImageJ plugin:

| Project         | Description                                                                                                           |
| --------------- | --------------------------------------------------------------------------------------------------------------------- |
| misaxx-analyzer | MISA++ result analyzer. Converts a MISA++ output into a SQLite database. This tool is required for the ImageJ plugin. |
| misa-imagej     | The ImageJ plugin.                                                                                                    |


## Dependency projects

Following projects are part of out publication, but only provide functionality used
by other applications:

| Project               | Description                                                                                                            |
| --------------------- | ---------------------------------------------------------------------------------------------------------------------- |
| misaxx-core           | MISA++ core library. It contains the basic MISA++ functions, such as DAG parallelization, or parameter handling.       |
| misaxx-imaging        | MISA++ OpenCV integration. Provides MISA++ data caches for single images or a directory that contains multiple images. |
| misaxx-ome            | MISA++ OME TIFF integration. Integrates the *OME Files* library to allow reading OME TIFF files as OpenCV images.      |
| misaxx-tissue         | MISA++ tool that segments tissue in 3D LSFM data. It is used within the glomeruli segmentation tool.                   |
| misaxx-ome-visualizer | Example tool that visualizes 32 bit integer label OME TIFF files as RGB.                                               | 
