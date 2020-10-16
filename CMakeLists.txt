# --------------------------------------------------------------------------- #
#    Main CMake file for LukFi Block                                          #
#                                                                             #
#                              Niccolo' Iardella                              #
#                          Operations Research Group                          #
#                         Dipartimento di Informatica                         #
#                             Universita' di Pisa                             #
# --------------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
project(LukFiBlock VERSION 0.2.0 LANGUAGES CXX)

# ----- Settings ------------------------------------------------------------ #
#include(CTest)

# ----- Requirements -------------------------------------------------------- #
find_package(SMSpp REQUIRED)

# ----- Library ------------------------------------------------------------- #
add_library(LukFiBlock)
target_compile_features(LukFiBlock PUBLIC cxx_std_17)

target_sources(LukFiBlock PRIVATE LukFiBlock.cpp)

# Different INSTALL_INTERFACE and BUILD_INTERFACE paths are used when
# generating the target import file (***Targets.cmake), that must be different
# for local build and installation.
target_include_directories(
        LukFiBlock PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/SMS++>)

target_link_libraries(LukFiBlock PUBLIC SMS++::SMSpp)

# Alias for testing
add_library(SMS++::LukFiBlock ALIAS LukFiBlock)

# ----- Install instructions ------------------------------------------------ #
# For installing the library and all its CMake modules on the system.
# Not required for local builds (see below).

include(GNUInstallDirs)

# Install library
install(TARGETS LukFiBlock
        EXPORT LukFiBlock-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Install headers
install(FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/LukFiBlock.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SMS++)

# Install target import file
install(EXPORT LukFiBlock-targets
        FILE LukFiBlockTargets.cmake
        NAMESPACE SMS++::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LukFiBlock)

# Generate package version file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/LukFiBlockConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion)

# Generate package configuration file
configure_package_config_file(
        ${CMAKE_CURRENT_LIST_DIR}/cmake/LukFiBlockConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/LukFiBlockConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LukFiBlock)

# Install package version and configuration files
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/LukFiBlockConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/LukFiBlockConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LukFiBlock)

# ----- Export from the build tree ------------------------------------------ #
# This is needed so other projects can find the library in a local build.
# It does not affect installation.

if (CMAKE_EXPORT_PACKAGE_REGISTRY)

    export(EXPORT LukFiBlock-targets
           FILE ${CMAKE_CURRENT_BINARY_DIR}/LukFiBlockTargets.cmake
           NAMESPACE SMS++::)

    # Register package in user's package registry (~/.cmake/registry)
    export(PACKAGE LukFiBlock)
endif ()

# ----- Subdirectories ------------------------------------------------------ #
#if (BUILD_TESTING)
#    add_subdirectory(test)
#endif()
