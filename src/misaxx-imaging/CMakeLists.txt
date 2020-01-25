# /**
# * Copyright by Ruman Gerst
# * Research Group Applied Systems Biology - Head: Prof. Dr. Marc Thilo Figge
# * https://www.leibniz-hki.de/en/applied-systems-biology.html
# * HKI-Center for Systems Biology of Infection
# * Leibniz Institute for Natural Product Research and Infection Biology - Hans Knöll Insitute (HKI)
# * Adolf-Reichwein-Straße 23, 07745 Jena, Germany
# *
# * This code is licensed under BSD 2-Clause
# * See the LICENSE file provided with this code for the full license.
# */

cmake_minimum_required(VERSION 3.11)
project(misaxx-imaging VERSION 1.0.1.0 DESCRIPTION "MISA++ Imaging Core")

#set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(misaxx-core REQUIRED)

# The library accepts any OpenCV version newer than 3.0
find_package(OpenCV COMPONENTS core imgproc ximgproc imgcodecs highgui)
find_package(TIFF REQUIRED)

add_library(misaxx-imaging
        include/misaxx/imaging/accessors/misa_image_file.h
        include/misaxx/imaging/accessors/misa_image_stack.h
        include/misaxx/imaging/caches/misa_image_file_cache.h
        include/misaxx/imaging/caches/misa_image_stack_cache.h
        include/misaxx/imaging/patterns/misa_image_pattern.h
        include/misaxx/imaging/patterns/misa_image_stack_pattern.h
        src/misaxx/imaging/accessors/misa_image_file.cpp
        src/misaxx/imaging/accessors/misa_image_stack.cpp
        src/misaxx/imaging/caches/misa_image_file_cache.cpp
        src/misaxx/imaging/caches/misa_image_stack_cache.cpp
        src/misaxx/imaging/patterns/misa_image_pattern.cpp
        src/misaxx/imaging/patterns/misa_image_stack_pattern.cpp
        include/misaxx/imaging/utils/percentiles.h
        include/misaxx/imaging/utils/thresholding.h
        include/misaxx/imaging/utils/misc.h
        src/misaxx/imaging/descriptions/misa_image_description.cpp
        include/misaxx/imaging/descriptions/misa_image_description.h
        src/misaxx/imaging/descriptions/misa_image_stack_description.cpp
        include/misaxx/imaging/descriptions/misa_image_stack_description.h
        src/misaxx/imaging/utils/tiffio.cpp
        include/misaxx/imaging/utils/tiffio.h
        include/misaxx/imaging/module_info.h
        src/misaxx/imaging/module_info.cpp)

set(MISAXX_LIBRARY misaxx-imaging)
set(MISAXX_LIBRARY_NAMESPACE misaxx::)
set(MISAXX_API_NAME misaxx_imaging)
set(MISAXX_API_INCLUDE_PATH misaxx/imaging)
set(MISAXX_API_NAMESPACE misaxx::imaging)
misaxx_with_default_module_info()
misaxx_with_default_api()

target_include_directories(misaxx-imaging PUBLIC $<BUILD_INTERFACE:${OpenCV_INCLUDE_DIR}>)
target_include_directories(misaxx-imaging PUBLIC $<BUILD_INTERFACE:${TIFF_INCLUDE_DIR}>)
target_link_libraries(misaxx-imaging PUBLIC ${OpenCV_LIBS}
        ${TIFF_LIBRARIES}
        misaxx::misaxx-core)

# Debian package creation
SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_NAME "libmisaxx-imaging")
SET(CPACK_DEBIAN_PACKAGE_VERSION "1.0.0.1")
SET(CPACK_DEBIAN_PACKAGE_RELEASE "1")
SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "MISA++ framework OpenCV integration library")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Ruman Gerst")
SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://applied-systems-biology.github.io/misa-framework/")
SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
INCLUDE(CPack)
