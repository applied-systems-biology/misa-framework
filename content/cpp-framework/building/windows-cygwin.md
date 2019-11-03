+++
title = "Building on Windows Cygwin"
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

We tested building on Windows via the [Cygwin64](https://www.cygwin.com/) environment.

{{% notice warning %}}Please note that the Visual Studio compiler is not
supported due to missing OpenMP 3.x capabilities.{{% /notice %}}

{{% notice warning %}}We consider building on Windows experimental due to unexplained
freezing of multi-threaded workloads if started from ImageJ (running directly within the command line does not show this behavior).{{% /notice %}}

# Prerequisites

Download and install [Cygwin64](https://www.cygwin.com/). Please consider our [Readme file](https://github.com/applied-systems-biology/misaxx-utils/blob/master/windows-builds/cygwin/README.md) for a list of required packages.

# Using the superbuild script

We provide fully automated scripts that installs MISA++ Core, the modules we
provide, and all necessary dependencies.

1. Download or clone the [MISA++ Utils repository](https://github.com/applied-systems-biology/misaxx-utils)
2. Open an Cygwin shell and navigate into the `windows-builds/cygwin` folder
3. Run `./build.sh`
3. Run `./package.sh`

{{% notice info %}}We cannot guarantee that all URLs within the build scripts stay
valid forever. If a download fails, replace the URL in the affected build script.{{% /notice %}}
