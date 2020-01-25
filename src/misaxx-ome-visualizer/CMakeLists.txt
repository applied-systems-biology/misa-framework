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
project(misaxx-ome-visualizer VERSION 1.0.0.1 DESCRIPTION "OME TIFF Visualization")

find_package(misaxx-core REQUIRED)
find_package(misaxx-imaging-ome REQUIRED)
# Add additional packages if necessary

add_library(misaxx-ome-visualizer include/misaxx-ome-visualizer/module_interface.h
        include/misaxx-ome-visualizer/module.h
        src/misaxx-ome-visualizer/module_interface.cpp
        src/misaxx-ome-visualizer/module.cpp
        src/misaxx-ome-visualizer/algorithms/visualize_task.cpp
        src/misaxx-ome-visualizer/algorithms/visualize_task.h
        src/misaxx-ome-visualizer/attachments/colormap.cpp
        include/misaxx-ome-visualizer/attachments/colormap.h
        src/misaxx-ome-visualizer/algorithms/find_colormap_task.cpp
        src/misaxx-ome-visualizer/algorithms/find_colormap_task.h
        include/misaxx-ome-visualizer/module_info.h
        src/misaxx-ome-visualizer/module_info.cpp)

# Add additional link targets if necessary
target_link_libraries(misaxx-ome-visualizer misaxx::misaxx-core misaxx::misaxx-imaging-ome)

# MISA++ helper script (automatically included by Core Library)
set(MISAXX_LIBRARY misaxx-ome-visualizer)
set(MISAXX_LIBRARY_NAMESPACE misaxx::)
set(MISAXX_API_NAME misaxx-ome-visualizer)
set(MISAXX_API_INCLUDE_PATH misaxx-ome-visualizer)
set(MISAXX_API_NAMESPACE misaxx_ome_visualizer)
misaxx_with_default_module_info()
misaxx_with_default_api()

# Only if it's a worker module:
misaxx_with_default_executable()

# Debian package creation
SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_NAME "misaxx-ome-visualizer")
SET(CPACK_DEBIAN_PACKAGE_VERSION "1.0.0.1")
SET(CPACK_DEBIAN_PACKAGE_RELEASE "1")
SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "MISA++ OME TIFF visualizer")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Ruman Gerst")
SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://applied-systems-biology.github.io/misa-framework/")
SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
INCLUDE(CPack)
