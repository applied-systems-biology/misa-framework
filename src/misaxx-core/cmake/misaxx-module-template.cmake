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

#
# This file contains additional functions that make it easier to create MISA++ modules
#
include(GenerateExportHeader)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Ensures that MISA++ is correctly configured
function(misaxx_ensure_configuration)
    if(MISAXX_LIBRARY)
    else()
        message(FATAL_ERROR "Please set MISAXX_LIBRARY to the library target (e.g. misaxx-imaging-ome)")
    endif()
    if(MISAXX_LIBRARY_NAMESPACE)
    else()
        message(FATAL_ERROR "Please set MISAXX_LIBRARY_NAMESPACE to the namespace where the library is accessible in other CMake projects (e.g. misaxx::)")
    endif()
    if(MISAXX_API_NAME)
    else()
        message(FATAL_ERROR "Please set MISAXX_API_NAME to the name of the module in C++ code (e.g. misaxx_ome)")
    endif()
    if(MISAXX_API_INCLUDE_PATH)
    else()
        message(FATAL_ERROR "Please set MISAXX_API_INCLUDE_PATH to the include path where the module API is located (e.g. misaxx/ome/)")
    endif()
    if(MISAXX_API_NAMESPACE)
    else()
        message(FATAL_ERROR "Please set MISAXX_API_NAMESPACE to the namespace where all module functionality should be located (e.g. misaxx::ome)")
    endif()
endfunction()

# Adds a default executable '<library>-bin' for the MISA++ library
# The executable creates an installable target
function(misaxx_with_default_executable)
    misaxx_ensure_configuration()
    message("-- Adding default executable for ${MISAXX_LIBRARY}")
    # If necessary, create the default executable path
    if(EXISTS ${CMAKE_SOURCE_DIR}/src/main.cpp)
        message("--   Using existing main.cpp")
    else()
        file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/src/)
        message("--   Creating ${CMAKE_SOURCE_DIR}/src/main.cpp based on module name ${MISAXX_API_NAME}")
        file(WRITE ${CMAKE_SOURCE_DIR}/src/main.cpp "\#include <${MISAXX_API_INCLUDE_PATH}/module.h>\n\
\#include <${MISAXX_API_INCLUDE_PATH}/module_info.h>\n\
\#include <misaxx/core/runtime/misa_cli.h>\n\
\n\
using namespace misaxx;\n\
using namespace ${MISAXX_API_NAMESPACE};\n\
\n\
int main(int argc, const char** argv) {\n\
    misa_cli cli {};\n\
    cli.set_module_info(${MISAXX_API_NAMESPACE}::module_info());\n\
    cli.set_root_module<${MISAXX_API_NAMESPACE}::module>(\"${MISAXX_LIBRARY}\");\n\
    return cli.prepare_and_run(argc, argv);\n\
}")
        message(WARNING "Please make sure that ${CMAKE_SOURCE_DIR}/src/main.cpp is correct")
    endif()

    # Module executable
    add_executable("${MISAXX_LIBRARY}-bin" src/main.cpp)
    target_link_libraries("${MISAXX_LIBRARY}-bin" ${MISAXX_LIBRARY})
    set_target_properties("${MISAXX_LIBRARY}-bin" PROPERTIES OUTPUT_NAME "${MISAXX_LIBRARY}")

    # Create install target for the executable
    install(TARGETS "${MISAXX_LIBRARY}-bin" DESTINATION bin)

    # Create and install the module link, so external programs can find this executable
    message("--   A module link JSON will be created for this executable")
    if(WIN32)
        set(MISA_MODULE_LINK_OPERATING_SYSTEM Windows)
        set(MISA_MODULE_LINK_EXECUTABLE_PATH ${CMAKE_INSTALL_PREFIX}/bin/${MISAXX_LIBRARY}.exe)
    else()
        # Assume Linux here for now
        set(MISA_MODULE_LINK_OPERATING_SYSTEM Linux)
        set(MISA_MODULE_LINK_EXECUTABLE_PATH ${CMAKE_INSTALL_PREFIX}/bin/${MISAXX_LIBRARY})
    endif()
    if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
        set(MISA_MODULE_LINK_ARCHITECTURE x64)
    else()
        # Assume x32 here for now
        set(MISA_MODULE_LINK_ARCHITECTURE x32)
    endif()
    file(WRITE ${CMAKE_BINARY_DIR}/misa-module-link.json "{\n\
    \"operating-system\" : \"${MISA_MODULE_LINK_OPERATING_SYSTEM}\",\n\
    \"architecture\" : \"${MISA_MODULE_LINK_ARCHITECTURE}\",\n\
    \"executable-path\" : \"${MISA_MODULE_LINK_EXECUTABLE_PATH}\"\n\
}")
    install(FILES ${CMAKE_BINARY_DIR}/misa-module-link.json
            RENAME ${MISAXX_LIBRARY}-${PROJECT_VERSION}-${MISA_MODULE_LINK_OPERATING_SYSTEM}-${MISA_MODULE_LINK_ARCHITECTURE}.json
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/misaxx/modules)

endfunction()

# Configures a module_info header that adapts to the current CMake project settings
function(misaxx_with_default_module_info)
    misaxx_ensure_configuration()
    # If necessary, create a header that contains the module info
    if(EXISTS ${CMAKE_SOURCE_DIR}/include/${MISAXX_API_INCLUDE_PATH}/module_info.h)
        message("--   Module info header already exists at ${CMAKE_SOURCE_DIR}/include/${MISAXX_API_INCLUDE_PATH}/module_info.h")
    else()
        message("--   Creating module info header ${CMAKE_SOURCE_DIR}/include/${MISAXX_API_INCLUDE_PATH}/module_info.h")
        message(WARNING "Please make sure that you include the dependencies in ${CMAKE_SOURCE_DIR}/include/${MISAXX_API_INCLUDE_PATH}/module_info.h")
        message(WARNING "Please add include/${MISAXX_API_INCLUDE_PATH}/module_info.h to your library's sources")

        file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/include/${MISAXX_API_INCLUDE_PATH}/)
        file(WRITE ${CMAKE_SOURCE_DIR}/include/${MISAXX_API_INCLUDE_PATH}/module_info.h "#pragma once\n\
#include <misaxx/core/module_info.h>\n\
\n\
namespace ${MISAXX_API_NAMESPACE} {\n\
    extern misaxx::misa_module_info module_info();\n\
}")
    endif()

    if(EXISTS ${CMAKE_SOURCE_DIR}/src/${MISAXX_API_INCLUDE_PATH}/module_info.cpp)
        message("--   Module info CPP already exists at ${CMAKE_SOURCE_DIR}/src/${MISAXX_API_INCLUDE_PATH}/module_info.cpp")
    else()
        message("--   Creating module info CPP ${CMAKE_SOURCE_DIR}/src/${MISAXX_API_INCLUDE_PATH}/module_info.cpp")
        message(WARNING "Please make sure that you include the dependencies in ${CMAKE_SOURCE_DIR}/src/${MISAXX_API_INCLUDE_PATH}/module_info.cpp")
        message(WARNING "Please add src/${MISAXX_API_INCLUDE_PATH}/module_info.cpp to your library's sources")

        file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/src/${MISAXX_API_INCLUDE_PATH}/)
        file(WRITE ${CMAKE_SOURCE_DIR}/src/${MISAXX_API_INCLUDE_PATH}/module_info.cpp "#include <misaxx/core/module_info.h>\n\
#include <misaxx/core/module_info.h>\n\
#include <${MISAXX_API_INCLUDE_PATH}/module_info.h>\n\
\n\
misaxx::misa_module_info ${MISAXX_API_NAMESPACE}::module_info() {\n\
    misaxx::misa_module_info info;\n\
    info.set_id(\"${PROJECT_NAME}\");\n\
    info.set_version(\"${PROJECT_VERSION}\");\n\
    info.set_name(\"${PROJECT_DESCRIPTION}\");\n\
    info.set_description(\"A MISA++ module\");\n\
    \n\
    info.add_dependency(misaxx::module_info());\n\
    // TODO: Add dependencies via info.add_dependency()\n\
    return info;
}")
    endif()

endfunction()

# Configures the current library target as shared library,
# creates necessary install targets for
function(misaxx_with_default_api)
    misaxx_ensure_configuration()
    message("-- ${MISAXX_LIBRARY} is configured to be a shared library ${MISAXX_LIBRARY_NAMESPACE}${MISAXX_LIBRARY}")

    # Option for shared library
    option(BUILD_SHARED_LIBS "Build shared library" ON)

    # If necessary, create the *.in file for library configuration
    if(EXISTS ${CMAKE_SOURCE_DIR}/cmake/${MISAXX_LIBRARY}-config.cmake.in)
        message("--   Using existing library configuration file ${CMAKE_SOURCE_DIR}/cmake/${MISAXX_LIBRARY}-config.cmake.in")
    else()
        message("--   Creating configuration file ${CMAKE_SOURCE_DIR}/cmake/${MISAXX_LIBRARY}-config.cmake.in")
        message(WARNING "Please make sure that the package dependencies in ${CMAKE_SOURCE_DIR}/cmake/${MISAXX_LIBRARY}-config.cmake.in are consistent with the dependencies in CMakeLists.txt")
        file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake/)
        file(WRITE ${CMAKE_SOURCE_DIR}/cmake/${MISAXX_LIBRARY}-config.cmake.in "\@PACKAGE_INIT\@\n\n\
find_package(misaxx-core REQUIRED)\n\n\
if(NOT TARGET ${MISAXX_LIBRARY})\n\
include(\${CMAKE_CURRENT_LIST_DIR}/${MISAXX_LIBRARY}-targets.cmake)\n\
endif()")
    endif()

    # Internal alias
    add_library("${MISAXX_LIBRARY_NAMESPACE}::${MISAXX_LIBRARY}" ALIAS ${MISAXX_LIBRARY})

    # Setup include directories
    generate_export_header(${MISAXX_LIBRARY}
            EXPORT_MACRO_NAME "${MISAXX_API_NAME}_API"
            EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/${MISAXX_API_INCLUDE_PATH}/common.h
            )
    target_include_directories(${MISAXX_LIBRARY}
            PUBLIC
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
            $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
            $<INSTALL_INTERFACE:include>
            PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
            )

    # Install targets
    message("--   Creating default install operations")
    set_target_properties(${MISAXX_LIBRARY} PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
            )
    install(TARGETS ${MISAXX_LIBRARY}
            EXPORT "${MISAXX_LIBRARY}-targets"
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${LIBLEGACY_INCLUDE_DIRS}
            )
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )
    install(DIRECTORY ${CMAKE_BINARY_DIR}/include/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )
    install(EXPORT ${MISAXX_LIBRARY}-targets
            FILE ${MISAXX_LIBRARY}-targets.cmake
            NAMESPACE "${MISAXX_LIBRARY_NAMESPACE}"
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${MISAXX_LIBRARY}
            )
    configure_package_config_file(
            ${CMAKE_SOURCE_DIR}/cmake/${MISAXX_LIBRARY}-config.cmake.in
            ${CMAKE_BINARY_DIR}/cmake/${MISAXX_LIBRARY}-config.cmake
            INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${MISAXX_LIBRARY}
    )
    write_basic_package_version_file(
            ${CMAKE_BINARY_DIR}/cmake/${MISAXX_LIBRARY}-config-version.cmake
            VERSION ${${MISAXX_LIBRARY}_VERSION}
            COMPATIBILITY AnyNewerVersion
    )
    install(
            FILES
            ${CMAKE_BINARY_DIR}/cmake/${MISAXX_LIBRARY}-config.cmake
            ${CMAKE_BINARY_DIR}/cmake/${MISAXX_LIBRARY}-config-version.cmake
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${MISAXX_LIBRARY}
    )
endfunction()

# Adds additional compiler warnings
function(misaxx_add_compiler_warnings)
    misaxx_ensure_configuration()
    # Additional warnings
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${MISAXX_LIBRARY} PRIVATE -Wredundant-decls
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
endfunction()