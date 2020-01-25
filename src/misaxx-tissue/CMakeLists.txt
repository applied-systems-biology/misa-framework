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
project(misaxx-tissue VERSION 1.0.0.1 DESCRIPTION "MISA++ Tissue Segmentation")

find_package(misaxx-imaging-ome REQUIRED)

add_library(misaxx-tissue-segmentation
        include/misaxx-tissue/module.h
        include/misaxx-tissue/module_interface.h
        src/misaxx-tissue/algorithms/segmentation2d/segmentation2d_klingberg_1.h
        src/misaxx-tissue/algorithms/segmentation2d/segmentation2d_base.h
        src/misaxx-tissue/algorithms/segmentation3d/segmentation3d_base.h
        src/misaxx-tissue/algorithms/segmentation3d/segmentation3d_none.h
        src/misaxx-tissue/algorithms/quantification.h
        src/misaxx-tissue/module_interface.cpp
        src/misaxx-tissue/module.cpp
        src/misaxx-tissue/algorithms/quantification.cpp
        src/misaxx-tissue/algorithms/segmentation3d/segmentation3d_none.cpp
        src/misaxx-tissue/algorithms/segmentation2d/segmentation2d_klingberg_1.cpp
        src/misaxx-tissue/attachments/tissue.cpp include/misaxx-tissue/attachments/tissue.h
        include/misaxx-tissue/module_info.h
        src/misaxx-tissue/module_info.cpp)

target_link_libraries(misaxx-tissue-segmentation misaxx::misaxx-imaging-ome)

set(MISAXX_LIBRARY misaxx-tissue-segmentation)
set(MISAXX_LIBRARY_NAMESPACE misaxx::)
set(MISAXX_API_NAME misaxx_tissue)
set(MISAXX_API_INCLUDE_PATH misaxx-tissue)
set(MISAXX_API_NAMESPACE misaxx_tissue)
misaxx_with_default_module_info()
misaxx_with_default_api()
misaxx_with_default_executable()

# Debian package creation
SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_NAME "misaxx-tissue")
SET(CPACK_DEBIAN_PACKAGE_VERSION "1.0.0.1")
SET(CPACK_DEBIAN_PACKAGE_RELEASE "1")
SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "MISA++ tissue segmentation")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Ruman Gerst")
SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://applied-systems-biology.github.io/misa-framework/")
SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
INCLUDE(CPack)
