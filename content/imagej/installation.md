+++
title = "Installation"
weight = 10
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

# Building from source

Building MISA++ for ImageJ requires the [Maven](https://maven.apache.org/) build system
and that a [JDK](https://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html) installation is present on the current system.

1. Download the source code
2. Navigate into the source directory
3. Run `mvn package`

The `misa_imagej` plugin can be found in the `target` directory. Copy it into
the ImageJ plugin directory.

MISA++ for ImageJ requires additional libraries that do not come pre-installed
with ImageJ or Fiji.

Copy following libraries from `target/dependencies` into the ImageJ `jars` directory:

* autolink
* flexmark
* graphics2d
* jfreesvg
* openhtmltopdf
* pdfbox
* poi
* sqlite
* bcprov-jdk15on
* bcpkix-jdk15on
* icepdf
* commons-exec


# Windows superbuild

MISA++ for ImageJ is part of the Windows superbuild. See [Building](/cpp-framework/building/windows-msys2) for more information.
