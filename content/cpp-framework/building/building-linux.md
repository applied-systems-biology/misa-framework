+++
title = "Building on Linux"
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

This building guide was tested for Ubuntu 18.04 and 19.10. To build \*.deb packages, you
can alternatively use our [Docker-based
Linux build scripts](https://github.com/applied-systems-biology/misaxx-utils/linux-builds).

# Building the C++ framework

## MISA++ Core

The core library requires at least GCC version 8 and the Boost libraries.

```bash
sudo apt install libboost-filesystem-dev libboost-regex-dev libboost-program-options-dev build-essential gcc-8 git
git clone https://github.com/applied-systems-biology/misaxx-core.git

pushd misaxx-core
mkdir build
pushd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
popd
popd
```

## MISA++ Analyzer

The analyzer application requires an installation of the core libraries.

```bash
sudo apt install libsqlite3-dev
git clone https://github.com/applied-systems-biology/misaxx-analyzer.git

pushd misaxx-analyzer
mkdir build
pushd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
popd
popd
```

## MISA++ Imaging (OpenCV integration)

The OpenCV integration requires OpenCV 3.x or 4.x.

{{% notice warning %}}We have experienced that multiple installations of OpenCV
can lead to the build system making use of the wrong library files - even
with CMake claiming to have found the correct version.{{% /notice %}}

```bash
sudo apt install libopencv-dev libtiff5-dev
git clone https://github.com/applied-systems-biology/misaxx-imaging.git

pushd misaxx-imaging
mkdir build
pushd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
popd
popd
```

## MISA++ OME (OME TIFF integration)

The OME TIFF integration requires an installation of `misaxx-imaging` and the
[OME Files library](https://www.openmicroscopy.org/ome-files/).

{{% notice warning %}}We have experienced issues with Boost 1.70 or higher in building
the OME libraries. This is caused by a change in how Boost handles CMake targets.{{% /notice %}}

{{% notice warning %}}During the build process of OME Model, a Python2 script is used to
generate C++ code. Due to lack of Python version specification, the script will crash
if Python3 is the default Python. In this case, create a Python2 virtualenv and
restart the build process.{{% /notice %}}

```bash
sudo apt install libxerces-c-dev libxalan-c-dev python wget libboost-all-dev

git clone https://github.com/applied-systems-biology/misaxx-ome.git
wget "https://downloads.openmicroscopy.org/ome-common-cpp/5.5.0/source/ome-common-cpp-5.5.0.zip"
unzip ome-common-cpp-5.5.0.zip
wget "https://downloads.openmicroscopy.org/ome-model/5.6.0/source/ome-model-5.6.0.zip"
unzip ome-model-5.6.0.zip
wget "https://downloads.openmicroscopy.org/ome-files-cpp/0.5.0/source/ome-files-cpp-0.5.0.zip"
unzip ome-files-cpp-0.5.0.zip

pushd ome-common-cpp-5.5.0
mkdir build
pushd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
popd
popd

pushd ome-model-5.6.0
mkdir build
pushd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
popd
popd

pushd ome-files-cpp-0.5.0
mkdir build
pushd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
popd
popd

pushd misaxx-ome
mkdir build
pushd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
popd
popd
```

## MISA++ OME Visualizer (Example application)

This application allows visualization of int32 OME TIFF files and requires
that `misaxx-ome` is installed.

```bash
git clone https://github.com/applied-systems-biology/misaxx-ome-visualizer.git

pushd misaxx-ome-visualizer
mkdir build
pushd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
popd
popd
```


## MISA++ Tissue segmentation (Example application)

This application segments tissue from whole-organ light-sheet microscopy images and requires
that `misaxx-ome` is installed.

```bash
git clone https://github.com/applied-systems-biology/misaxx-tissue.git

pushd misaxx-tissue
mkdir build
pushd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
popd
popd
```

## MISA++ Kidney glomeruli segmentation (Example application)

This application segments the glomeruli in whole organ light-sheet microscopy images and requires
that `misaxx-tissue` is installed.

```bash
git clone https://github.com/applied-systems-biology/misaxx-kidney-glomeruli.git

pushd misaxx-kidney-glomeruli
mkdir build
pushd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
popd
popd
```
