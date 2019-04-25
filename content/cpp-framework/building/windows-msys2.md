+++
title = "Building on Windows"
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

We tested building on Windows via the [MSYS2](https://www.msys2.org/) environment.

{{% notice warning %}}Please note that the Visual Studio compiler is not
supported due to missing OpenMP 3.x capabilities.{{% /notice %}}


# Prerequisites

1. Download and install [MSYS2](https://www.msys2.org/).
2. Start MSYS2 (MinGW 64-bit) and update the packages by typing `pacman -Syyu`
3. Let the process finish and **close** the terminal as instructed
4. Restart MSYS2 (MinGW 64-bit)

# Using the superbuild script

We provide fully automated scripts that installs MISA++ Core, the modules we
provide, and all necessary dependencies.

1. Download or clone the [MISA++ Utils repository](https://github.com/applied-systems-biology/misaxx-utils)
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

{{% panel theme="danger" header="Warning" %}}
The OME libraries might be incompatible with the boost version provided by MSYS2 (Cannot find libraries).
If this is the case four you, remove boost via `pacman -R mingw-w64-x86_64-boost` and remove the
`mingw-w64-$MSYS2_PLATFORM-boost \` line in `superbuild.sh`.

First make sure that `patch` is installed by running `pacman -S patch`.

Clone the MSYS2 package build script repository via `git clone https://github.com/msys2/MINGW-packages.git`
and go back to the commit that contains the script to install Boost version 1.69 by navigating into the
`MINGW-packages` repository and running `git checkout fff2fc0d53aa95b85cee0c785e56159b0565ea72`.

Navigate into the `mingw-w64-boost` folder and run `makepkg -si`. The command will compile the correct Boost
version and install it.
{{% /panel %}}

# Manually building

Install the necessary MSYS2 packages:

```bash
pacman -S --noconfirm --needed unzip mingw-w64-x86_64-cmake \
wget \
mingw-w64-x86_64-toolchain \
mingw-w64-x86_64-boost \
mingw-w64-x86_64-make \
libsqlite \
libsqlite-devel \
mingw-w64-x86_64-opencv \
mingw-w64-x86_64-libtiff \
mingw-w64-x86_64-xerces-c \
mingw-w64-x86_64-xalan-c \
mingw-w64-x86_64-libpng \
mingw-w64-x86_64-python2
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
* Building OME libraries dynamically caused segmentation faults during application runtime (Error 0xc0000005). Build the libraries statically to avoid the issue.
* Dynamically built MISA++ libraries caused random (~ every 10th execution) 0xc0000005 errors. We again decided to only link statically.
* OME Model requires `OME_HOME` to be set to `/mingw64/` or any other folder that contains `./share/xml` and `./share/xsl`
{{% /panel %}}
