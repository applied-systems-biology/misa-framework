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
project(misaxx-imaging-ome VERSION 1.0.0.1 DESCRIPTION "MISA++ OME Integration")

find_package(misaxx-core REQUIRED)
find_package(misaxx-imaging REQUIRED)
find_package(Boost 1.63 COMPONENTS REQUIRED system log_setup log iostreams)
find_package(Threads REQUIRED)
find_package(OMEFiles 0.5.0 REQUIRED)
find_package(OMEXML 5.6.0 REQUIRED)

add_library(misaxx-imaging-ome
        src/misaxx/ome/utils/ome_to_opencv.h
        src/misaxx/ome/utils/ome_to_opencv.cpp
        include/misaxx/ome/caches/misa_ome_tiff_cache.h
        src/misaxx/ome/caches/misa_ome_tiff_cache.cpp
        include/misaxx/ome/caches/misa_ome_plane_cache.h
        src/misaxx/ome/caches/misa_ome_plane_cache.cpp
        include/misaxx/ome/patterns/misa_ome_tiff_pattern.h
        src/misaxx/ome/patterns/misa_ome_tiff_pattern.cpp
        include/misaxx/ome/descriptions/misa_ome_tiff_description.h
        src/misaxx/ome/descriptions/misa_ome_tiff_description.cpp
        src/misaxx/ome/utils/opencv_to_ome.h
        src/misaxx/ome/utils/opencv_to_ome.cpp
        src/misaxx/ome/utils/ome_tiff_io.h
        src/misaxx/ome/utils/ome_tiff_io.cpp
        include/misaxx/ome/utils/json_ome_pixel_type.h
        src/misaxx/ome/utils/json_ome_pixel_type.cpp
        include/misaxx/ome/utils/ome_helpers.h
        src/misaxx/ome/utils/ome_helpers.cpp
        include/misaxx/ome/descriptions/misa_ome_plane_description.h
        src/misaxx/ome/descriptions/misa_ome_plane_description.cpp
        src/misaxx/ome/accessors/misa_ome_plane.cpp
        include/misaxx/ome/accessors/misa_ome_plane.h
        src/misaxx/ome/accessors/misa_ome_tiff.cpp
        include/misaxx/ome/accessors/misa_ome_tiff.h
        src/misaxx/ome/utils/ome_to_ome.h
        src/misaxx/ome/utils/ome_to_ome.cpp
        include/misaxx/ome/misa_ome_tiff_description_builder.h
        src/misaxx/ome/misa_ome_tiff_description_builder.cpp
        src/misaxx/ome/attachments/misa_ome_planes_location.cpp
        include/misaxx/ome/attachments/misa_ome_planes_location.h
        src/misaxx/ome/utils/json_ome_quantity.cpp
        include/misaxx/ome/utils/json_ome_quantity.h
        src/misaxx/ome/misa_ome_tiff_description_modifier.cpp
        include/misaxx/ome/misa_ome_tiff_description_modifier.h
        src/misaxx/ome/attachments/misa_ome_unit.cpp
        include/misaxx/ome/attachments/misa_ome_unit.h
        include/misaxx/ome/attachments/misa_ome_voxel_size.h
        src/misaxx/ome/attachments/misa_ome_voxel_size.cpp
        src/misaxx/ome/attachments/misa_ome_pixel_count.cpp
        include/misaxx/ome/attachments/misa_ome_pixel_count.h
        src/misaxx/ome/attachments/misa_ome_voxel.cpp
        include/misaxx/ome/attachments/misa_ome_voxel.h
        include/misaxx/ome/utils/units_length.h
        include/misaxx/ome/utils/units_electric_potential.h
        include/misaxx/ome/utils/units_frequency.h
        include/misaxx/ome/utils/units_power.h
        include/misaxx/ome/utils/units_pressure.h
        include/misaxx/ome/utils/units_temperature.h
        include/misaxx/ome/utils/units_time.h
        include/misaxx/ome/attachments/misa_ome_quantity.h
        include/misaxx/ome/module_info.h
        src/misaxx/ome/module_info.cpp)

set(MISAXX_LIBRARY misaxx-imaging-ome)
set(MISAXX_LIBRARY_NAMESPACE misaxx::)
set(MISAXX_API_NAME misaxx_ome)
set(MISAXX_API_INCLUDE_PATH misaxx/ome)
set(MISAXX_API_NAMESPACE misaxx::ome)
misaxx_with_default_module_info()
misaxx_with_default_api()

target_link_libraries(misaxx-imaging-ome PUBLIC OME::Files misaxx::misaxx-core misaxx::misaxx-imaging)

# Debian package creation
SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_NAME "libmisaxx-ome")
SET(CPACK_DEBIAN_PACKAGE_VERSION "1.0.0.1")
SET(CPACK_DEBIAN_PACKAGE_RELEASE "1")
SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "MISA++ framework OME TIFF integration library")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Ruman Gerst")
SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://applied-systems-biology.github.io/misa-framework/")
SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
INCLUDE(CPack)
