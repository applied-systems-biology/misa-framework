+++
title = "Prerequisites"
weight = 10
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

MISA++ uses the [CMake](https://cmake.org/) build system.
Please make sure that CMake version 3.13 or higher is installed.

The compiler must support C++ version 2017 or higher.

For Windows, we recommend the [Cygwin](https://cygwin.com/) environment.

{{% notice warning %}}Please note that the Visual Studio compiler is not
supported due to missing OpenMP 3.x capabilities.{{% /notice %}}

Make sure that **MISA++ Core** and any other dependency modules are installed.
See [Building](../../building) for more information about building MISA++ and
already existing modules.
