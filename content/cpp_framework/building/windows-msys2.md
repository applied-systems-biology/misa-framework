+++
title = "Building on Windows"
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

We tested building on Windows via the [MSYS2](https://www.msys2.org/) environment.

{{% alert theme="info" %}}Please note that the Visual Studio compiler is not
supported due to missing OpenMP 3.x capabilities.{{% /alert %}}


# Prerequisites

1. Download and install [MSYS2](https://www.msys2.org/).
2. Start MSYS2 (MinGW 64-bit) and update the packages by typing `pacman -Syyu`
3. Let the process finish and **close** the terminal as instructed
4. Restart MSYS2 (MinGW 64-bit)

# Using the superbuild script

We provide fully automated scripts that installs MISA++ Core, the modules we
provide, and all necessary dependencies.

1. Download or clone the [MISA++ Utils repository](https://asb-git.hki-jena.de/RGerst/misaxx-utils)
2. Open an MSYS2 (MinGW 64-bit) shell and navigate into the `superbuild-win32` folder
3. Run `./superbuild.sh` and follow the instructions

To create a distributable package of the MISA++ applications, run following command:

```bash
./superbuild.sh
./package.py
```

To create a [Fiji](http://fiji.sc/) distribution that comes pre-installed with MISA++
for ImageJ and the MISA++ applications, run following command:

```bash
./superbuild.sh
./package.py
./package-fiji.sh
```

# Manually building

Install the necessary MSYS2 packages:

```bash
MSYS2_PLATFORM=x86_64
pacman -S --noconfirm --needed unzip mingw-w64-$MSYS2_PLATFORM-cmake \
wget \
mingw-w64-$MSYS2_PLATFORM-toolchain \
mingw-w64-$MSYS2_PLATFORM-boost \
mingw-w64-$MSYS2_PLATFORM-make \
libsqlite \
libsqlite-devel \
mingw-w64-$MSYS2_PLATFORM-opencv \
mingw-w64-$MSYS2_PLATFORM-libtiff \
mingw-w64-$MSYS2_PLATFORM-xerces-c \
mingw-w64-$MSYS2_PLATFORM-xalan-c \
mingw-w64-$MSYS2_PLATFORM-libpng \
mingw-w64-$MSYS2_PLATFORM-python2
```

Obtain the MISA++ sources and the sources of dependencies that are not in
MSYS2. Build them like any CMake build:

```bash
cd $SOURCE_CODE
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=/mingw64/ -G "Unix Makefiles" ..
```

{{% panel theme="danger" header="Warning" %}}
* LEMON Graph library cannot be built dynamically (`-DBUILD_SHARED_LIBS=OFF` must be set)
* Building OME libraries dynamically caused segmentation faults during application runtime (Error 0xc0000005). Build the libraries statically to avoid the issue.
* Dynamically built MISA++ libraries caused random (~ every 10th execution) 0xc0000005 errors. We again decided to only link statically.
* OME Model requires `OME_HOME` to be set to `/mingw64/` or any other folder that contains `./share/xml` and `./share/xsl`
{{% /panel %}}
