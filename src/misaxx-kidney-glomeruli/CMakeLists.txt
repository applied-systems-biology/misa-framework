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
project(misaxx-kidney-glomeruli VERSION 1.0.0.1 DESCRIPTION "MISA++ Kidney Glomeruli Segmentation")

find_package(misaxx-tissue-segmentation REQUIRED)
find_package(misaxx-imaging-ome REQUIRED)

add_library(misaxx-kidney-glomeruli-segmentation
        include/misaxx-kidney-glomeruli/module.h
        include/misaxx-kidney-glomeruli/module_interface.h
        src/misaxx-kidney-glomeruli/algorithms/segmentation2d/segmentation2d_base.h
        src/misaxx-kidney-glomeruli/algorithms/segmentation2d/segmentation2d_klingberg.h
        src/misaxx-kidney-glomeruli/algorithms/segmentation3d/segmentation3d_base.h
        src/misaxx-kidney-glomeruli/algorithms/quantification/quantification_base.h
        src/misaxx-kidney-glomeruli/algorithms/quantification/quantification_klingberg.h
        include/misaxx-kidney-glomeruli/attachments/glomerulus.h
        include/misaxx-kidney-glomeruli/attachments/glomeruli.h
        src/misaxx-kidney-glomeruli/module_interface.cpp
        src/misaxx-kidney-glomeruli/module.cpp
        src/misaxx-kidney-glomeruli/attachments/glomerulus.cpp
        src/misaxx-kidney-glomeruli/algorithms/segmentation2d/segmentation2d_klingberg.cpp
        src/misaxx-kidney-glomeruli/algorithms/quantification/quantification_klingberg.cpp
        src/misaxx-kidney-glomeruli/attachments/glomeruli.cpp
        src/misaxx-kidney-glomeruli/algorithms/quantification/quantification_klingberg_2d.cpp
        src/misaxx-kidney-glomeruli/algorithms/quantification/quantification_klingberg_2d.h
        include/misaxx-kidney-glomeruli/module_info.h
        src/misaxx-kidney-glomeruli/module_info.cpp
        src/misaxx-kidney-glomeruli/algorithms/segmentation3d/segmentation3d_klingberg.cpp
        src/misaxx-kidney-glomeruli/algorithms/segmentation3d/segmentation3d_klingberg.h src/misaxx-kidney-glomeruli/algorithms/filtering/glomeruli_filtering.cpp src/misaxx-kidney-glomeruli/algorithms/filtering/glomeruli_filtering.h)

target_link_libraries(misaxx-kidney-glomeruli-segmentation PUBLIC misaxx::misaxx-tissue-segmentation)

set(MISAXX_LIBRARY misaxx-kidney-glomeruli-segmentation)
set(MISAXX_LIBRARY_NAMESPACE misaxx::)
set(MISAXX_API_NAME misaxx_kidney_glomeruli)
set(MISAXX_API_INCLUDE_PATH misaxx-kidney-glomeruli)
set(MISAXX_API_NAMESPACE misaxx_kidney_glomeruli)
misaxx_with_default_module_info()
misaxx_with_default_api()
misaxx_with_default_executable()

# Debian package creation
SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_NAME "misaxx-kidney-glomeruli")
SET(CPACK_DEBIAN_PACKAGE_VERSION "1.0.0.1")
SET(CPACK_DEBIAN_PACKAGE_RELEASE "1")
SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "MISA++ Kidney glomeruli segmentation")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Ruman Gerst")
SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://applied-systems-biology.github.io/misa-framework/")
SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
INCLUDE(CPack)
