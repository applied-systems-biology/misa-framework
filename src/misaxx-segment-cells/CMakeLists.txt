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

cmake_minimum_required(VERSION 3.11) # Or higher if required
project(misaxx-segment-cells VERSION 1.0.0 DESCRIPTION "")

find_package(misaxx-core REQUIRED)
find_package(misaxx-imaging REQUIRED)
# Add additional packages if necessary

add_library(misaxx-segment-cells include/misaxx-segment-cells/module_interface.h
                include/misaxx-segment-cells/module.h
                src/misaxx-segment-cells/module_interface.cpp
                src/misaxx-segment-cells/module.cpp
                src/misaxx-segment-cells/module_info.cpp src/misaxx-segment-cells/algorithms/segment_experiment.cpp src/misaxx-segment-cells/algorithms/segment_experiment.h src/misaxx-segment-cells/algorithms/quantify_conidia.cpp src/misaxx-segment-cells/algorithms/quantify_conidia.h src/misaxx-segment-cells/attachments/conidia_count.cpp include/misaxx-segment-cells/attachments/conidia_count.h)

# Add additional link targets if necessary
target_link_libraries(misaxx-segment-cells misaxx::misaxx-core misaxx::misaxx-imaging)

# MISA++ helper script (automatically included by Core Library)
set(MISAXX_LIBRARY misaxx-segment-cells)
set(MISAXX_LIBRARY_NAMESPACE misaxx_segment_cells::)
set(MISAXX_API_NAME misaxx_segment_cells)
set(MISAXX_API_INCLUDE_PATH misaxx-segment-cells)
set(MISAXX_API_NAMESPACE misaxx_segment_cells)
misaxx_with_default_module_info()
misaxx_with_default_api()

# Only if it's a worker module:
misaxx_with_default_executable()

# Debian package creation
SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_NAME "misaxx-segment-cells")
SET(CPACK_DEBIAN_PACKAGE_VERSION "1.0.0")
SET(CPACK_DEBIAN_PACKAGE_RELEASE "1")
SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "MISA++ distance transform watershed cell segmentation example")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Ruman Gerst")
SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://applied-systems-biology.github.io/misa-framework/")
SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
INCLUDE(CPack)
