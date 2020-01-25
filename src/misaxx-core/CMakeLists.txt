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
project(misaxx-core LANGUAGES CXX VERSION 1.0.0.1 DESCRIPTION "MISA++ Core")

include(FetchContent)
include(FeatureSummary)
include(GenerateExportHeader)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Build options
option(BUILD_SHARED_LIBS "Build shared library" ON)

# Dependencies
find_package(nlohmann_json REQUIRED)
find_package(Boost 1.63 COMPONENTS REQUIRED filesystem regex program_options)
find_package(OpenMP REQUIRED)
feature_summary(WHAT ALL)

# Setting up the library
add_library(misaxx-core
        include/misaxx/core/patterns/misa_file_pattern.h
        include/misaxx/core/patterns/misa_file_stack_pattern.h
        include/misaxx/core/misa_module_interface.h
        include/misaxx/core/misa_cached_data.h
        include/misaxx/core/misa_cache.h
        include/misaxx/core/misa_root_module_base.h
        include/misaxx/core/misa_module.h
        include/misaxx/core/misa_data_pattern.h
        include/misaxx/core/misa_data_description.h
        include/misaxx/core/misa_root_module.h
        include/misaxx/core/descriptions/misa_file_stack_description.h
        include/misaxx/core/descriptions/misa_file_description.h
        include/misaxx/core/caches/misa_exported_attachments_cache.h
        include/misaxx/core/filesystem/misa_filesystem_json_importer.h
        include/misaxx/core/filesystem/misa_filesystem_entry.h
        include/misaxx/core/filesystem/misa_filesystem_directories_importer.h
        include/misaxx/core/filesystem/misa_filesystem_empty_importer.h
        include/misaxx/core/filesystem/misa_filesystem.h
        include/misaxx/core/misa_dispatch_blueprint.h
        include/misaxx/core/misa_serializable.h
        include/misaxx/core/misa_description_storage.h
        include/misaxx/core/workers/misa_work_node.h
        include/misaxx/core/workers/misa_work_subtree_status.h
        include/misaxx/core/workers/misa_worker_status.h
        include/misaxx/core/misa_task.h
        include/misaxx/core/workers/misa_work_dependency_group.h
        include/misaxx/core/workers/misa_work_dependency_chain.h
        include/misaxx/core/workers/misa_work_dependency_segment.h
        include/misaxx/core/misa_dispatcher.h
        include/misaxx/core/misa_worker.h
        include/misaxx/core/utils/cache/cache.h
        include/misaxx/core/utils/cache/locked.h
        include/misaxx/core/utils/cache/memory_cache.h
        include/misaxx/core/utils/cache/readonly_access.h
        include/misaxx/core/utils/cache/readwrite_access.h
        include/misaxx/core/utils/cache/write_access.h
        src/misaxx/core/patterns/misa_file_pattern.cpp
        src/misaxx/core/patterns/misa_file_stack_pattern.cpp
        src/misaxx/core/misa_module_interface.cpp
        src/misaxx/core/misa_cached_data.cpp
        src/misaxx/core/misa_cache.cpp
        src/misaxx/core/misa_root_module_base.cpp
        src/misaxx/core/misa_module.cpp
        src/misaxx/core/misa_data_pattern.cpp
        src/misaxx/core/misa_data_description.cpp
        src/misaxx/core/misa_root_module.cpp
        src/misaxx/core/descriptions/misa_file_stack_description.cpp
        src/misaxx/core/descriptions/misa_file_description.cpp
        src/misaxx/core/caches/misa_exported_attachments_cache.cpp
        src/misaxx/core/filesystem/misa_filesystem_json_importer.cpp
        src/misaxx/core/filesystem/misa_filesystem_entry.cpp
        src/misaxx/core/filesystem/misa_filesystem_directories_importer.cpp
        src/misaxx/core/filesystem/misa_filesystem_empty_importer.cpp
        src/misaxx/core/filesystem/misa_filesystem.cpp
        src/misaxx/core/misa_dispatch_blueprint.cpp
        src/misaxx/core/misa_serializable.cpp
        src/misaxx/core/misa_description_storage.cpp
        src/misaxx/core/workers/misa_work_node.cpp
        src/misaxx/core/workers/misa_work_subtree_status.cpp
        src/misaxx/core/workers/misa_worker_status.cpp
        src/misaxx/core/misa_task.cpp
        src/misaxx/core/workers/misa_work_dependency_group.cpp
        src/misaxx/core/workers/misa_work_dependency_chain.cpp
        src/misaxx/core/workers/misa_work_dependency_segment.cpp
        src/misaxx/core/misa_dispatcher.cpp
        src/misaxx/core/misa_worker.cpp
        src/misaxx/core/misa_data_pattern.cpp
        include/misaxx/core/misa_primitive.h
        include/misaxx/core/misa_serialization_id.h
        src/misaxx/core/misa_cached_data_base.cpp
        include/misaxx/core/misa_cached_data_base.h
        include/misaxx/core/misa_default_cache.h
        src/misaxx/core/patterns/misa_dummy_pattern.cpp
        include/misaxx/core/patterns/misa_dummy_pattern.h
        include/misaxx/core/misa_manual_cache.h
        src/misaxx/core/accessors/misa_exported_attachments.cpp
        include/misaxx/core/accessors/misa_exported_attachments.h
        include/misaxx/core/misa_default_description_accessors.h
        src/misaxx/core/attachments/misa_location.cpp
        include/misaxx/core/attachments/misa_location.h
        src/misaxx/core/attachments/misa_unit.cpp
        include/misaxx/core/attachments/misa_unit.h
        src/misaxx/core/attachments/misa_unit_numeric.cpp
        include/misaxx/core/attachments/misa_unit_numeric.h
        src/misaxx/core/attachments/misa_quantity.cpp
        include/misaxx/core/attachments/misa_quantity.h
        src/misaxx/core/misa_serialization_id.cpp
        src/misaxx/core/attachments/misa_matrix.cpp
        include/misaxx/core/attachments/misa_matrix.h
        src/misaxx/core/runtime/misa_cli.cpp
        include/misaxx/core/runtime/misa_cli.h
        include/misaxx/core/attachments/misa_locatable.h
        src/misaxx/core/misa_dispatcher_blueprint_list.cpp
        include/misaxx/core/misa_dispatcher_blueprint_list.h
        src/misaxx/core/misa_dispatcher_builder.cpp
        include/misaxx/core/misa_dispatcher_builder.h
        include/misaxx/core/misa_parameter_base.h
        include/misaxx/core/misa_parameter.h
        include/misaxx/core/workers/misa_work_tree_path.h
        src/misaxx/core/workers/misa_work_tree_path.cpp
        include/misaxx/core/workers/misa_work_tree_node_path.h
        src/misaxx/core/workers/misa_work_tree_node_path.cpp
        src/misaxx/core/workers/misa_work_tree_sample_path.cpp
        include/misaxx/core/workers/misa_work_tree_sample_path.h
        src/misaxx/core/workers/misa_work_tree_algorithm_path.cpp
        include/misaxx/core/workers/misa_work_tree_algorithm_path.h
        src/misaxx/core/misa_parameter_builder.cpp
        include/misaxx/core/misa_parameter_builder.h
        include/misaxx/core/runtime/misa_runtime_properties.h
        include/misaxx/core/runtime/misa_cache_registry.h
        include/misaxx/core/runtime/misa_parameter_registry.h
        src/misaxx/core/runtime/misa_runtime_properties.cpp
        src/misaxx/core/runtime/misa_cache_registry.cpp
        src/misaxx/core/runtime/misa_parameter_registry.cpp
        src/misaxx/core/misa_module_info.cpp include/misaxx/core/misa_module_info.h
        include/misaxx/core/utils/cache.h
        include/misaxx/core/utils/ref.h
        include/misaxx/core/utils/string.h
        include/misaxx/core/utils/type_traits.h
        include/misaxx/core/utils/dynamic_singleton_map.h
        src/misaxx/core/utils/filesystem.cpp
        include/misaxx/core/utils/filesystem.h
        include/misaxx/core/utils/manual_stopwatch.h
        src/misaxx/core/utils/manual_stopwatch.cpp
        src/misaxx/core/attachments/misa_locatable.cpp
        include/misaxx/core/attachments/detail/misa_locatable.h
        include/misaxx/core/detail/misa_cached_data.h
        src/misaxx/core/workers/misa_work_node_impl.cpp
        src/misaxx/core/workers/misa_work_node_impl.h
        src/misaxx/core/runtime/misa_runtime_log.cpp
        include/misaxx/core/runtime/misa_runtime_log.h
        src/misaxx/core/descriptions/misa_exported_attachments_description.cpp
        include/misaxx/core/descriptions/misa_exported_attachments_description.h
        src/misaxx/core/misa_json_schema_property.cpp
        include/misaxx/core/misa_json_schema_property.h
        src/misaxx/core/misa_parameter_base.cpp
        src/misaxx/core/patterns/misa_folder_pattern.cpp
        include/misaxx/core/patterns/misa_folder_pattern.h
        src/misaxx/core/descriptions/misa_folder_description.cpp
        include/misaxx/core/descriptions/misa_folder_description.h
        src/misaxx/core/caches/misa_json_cache.cpp
        include/misaxx/core/caches/misa_json_cache.h
        src/misaxx/core/descriptions/misa_json_description.cpp
        include/misaxx/core/descriptions/misa_json_description.h
        src/misaxx/core/patterns/misa_json_pattern.cpp
        include/misaxx/core/patterns/misa_json_pattern.h
        src/misaxx/core/accessors/misa_json.cpp
        include/misaxx/core/accessors/misa_json.h
        src/misaxx/core/utils/markdown.h
        src/misaxx/core/runtime/misa_readme_builder.h
        src/misaxx/core/runtime/misa_readme_builder.cpp
        include/misaxx/core/attachments/misa_quantity_range.h
        include/misaxx/core/module_info.h
        src/misaxx/core/module_info.cpp
        src/misaxx/core/runtime/misa_runtime.cpp
        include/misaxx/core/runtime/misa_runtime.h include/misaxx/core/misa_module_base.h include/misaxx/core/runtime/detail/misa_cli.h)


# For installation
target_compile_features(misaxx-core PUBLIC cxx_std_17)
add_library(misaxx::core ALIAS misaxx-core)

generate_export_header(misaxx-core
        EXPORT_MACRO_NAME MISAXX_CORE_API
        EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/misaxx/common.h
        )
target_include_directories(misaxx-core
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

# Additional warnings
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(misaxx-core PRIVATE -Wredundant-decls
            -Wcast-align
            -Wmissing-declarations
            -Wmissing-include-dirs
            -Wswitch-enum
            -Wswitch-default
            -Wextra
            -Wall
            -Winvalid-pch
            -Wredundant-decls)
endif()

target_link_libraries(misaxx-core PUBLIC
    Boost::filesystem
    Boost::regex
    Boost::program_options
    OpenMP::OpenMP_CXX
    nlohmann_json)

# Installation
set_target_properties(misaxx-core PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        )
install(TARGETS misaxx-core
        EXPORT misaxx-core-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${LIBLEGACY_INCLUDE_DIRS}
        )
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/misaxx
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )
install(DIRECTORY ${CMAKE_BINARY_DIR}/include/misaxx
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )
install(EXPORT misaxx-core-targets
        FILE misaxx-core-targets.cmake
        NAMESPACE misaxx::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/misaxx-core
        )
configure_package_config_file(
        ${CMAKE_SOURCE_DIR}/cmake/misaxx-core-config.cmake.in
        ${CMAKE_BINARY_DIR}/cmake/misaxx-core-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/misaxx-core
)
write_basic_package_version_file(
        ${CMAKE_BINARY_DIR}/cmake/misaxx-core-config-version.cmake
        VERSION ${misaxx-core_VERSION}
        COMPATIBILITY AnyNewerVersion
)
install(
        FILES
        ${CMAKE_BINARY_DIR}/cmake/misaxx-core-config.cmake
        ${CMAKE_BINARY_DIR}/cmake/misaxx-core-config-version.cmake
        ${CMAKE_SOURCE_DIR}/cmake/misaxx-module-template.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/misaxx-core
)

# Debian package creation
SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_NAME "libmisaxx-core")
SET(CPACK_DEBIAN_PACKAGE_VERSION "1.0.0.1")
SET(CPACK_DEBIAN_PACKAGE_RELEASE "1")
SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "MISA++ framework core library")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Ruman Gerst")
SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://applied-systems-biology.github.io/misa-framework/")
SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
INCLUDE(CPack)
