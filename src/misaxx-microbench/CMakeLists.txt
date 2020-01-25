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
project(misaxx-microbench VERSION 1.0.0 DESCRIPTION "")

find_package(misaxx-core REQUIRED)
find_package(misaxx-imaging REQUIRED)
# Add additional packages if necessary

add_library(misaxx-microbench include/misaxx-microbench/module_interface.h
                include/misaxx-microbench/module.h
                src/misaxx-microbench/module_interface.cpp
                src/misaxx-microbench/module.cpp
                src/misaxx-microbench/module_info.cpp
                src/misaxx-microbench/algorithms/microbench_task.cpp
                src/misaxx-microbench/algorithms/microbench_task.h 
                src/misaxx-microbench/attachments/microbench_runtimes.cpp
                include/misaxx-microbench/attachments/microbench_runtimes.h)

# Add additional link targets if necessary
target_link_libraries(misaxx-microbench misaxx::misaxx-core misaxx::misaxx-imaging)

# MISA++ helper script (automatically included by Core Library)
set(MISAXX_LIBRARY misaxx-microbench)
set(MISAXX_LIBRARY_NAMESPACE misaxx_microbench::)
set(MISAXX_API_NAME misaxx_microbench)
set(MISAXX_API_INCLUDE_PATH misaxx-microbench)
set(MISAXX_API_NAMESPACE misaxx_microbench)
misaxx_with_default_module_info()
misaxx_with_default_api()

# Only if it's a worker module:
misaxx_with_default_executable()

# Debian package creation
SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_NAME "misaxx-microbench")
SET(CPACK_DEBIAN_PACKAGE_VERSION "1.0.0")
SET(CPACK_DEBIAN_PACKAGE_RELEASE "1")
SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "MISA++ single image processing operation benchmark")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Ruman Gerst")
SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://applied-systems-biology.github.io/misa-framework/")
SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
INCLUDE(CPack)
