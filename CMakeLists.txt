# ----- Project description and directories ------------------------------------
cmake_minimum_required(VERSION 3.8)
project(LukFiBlock
        VERSION 1.0.0 # TODO: set proper version
        HOMEPAGE_URL "https://gitlab.com/egorgone/lukfiblock"
        DESCRIPTION "LukFiBlock for SMS++")

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR})

# ----- Requirements -----------------------------------------------------------
find_package(Boost REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(SMSpp REQUIRED)

# ----- Library ----------------------------------------------------------------
add_library(LukFiBlock STATIC LukFiBlock.cpp)
target_compile_features(LukFiBlock PUBLIC cxx_std_17)

target_include_directories(LukFiBlock
                           PUBLIC
                           $<INSTALL_INTERFACE:include/SMS++>
                           $<BUILD_INTERFACE:${PROJECT_INCLUDE_DIR}>
                           )

target_link_libraries(LukFiBlock PUBLIC
                      SMS++::SMSpp
                      Boost::boost
                      Eigen3::Eigen)

# Alias for testing
add_library(SMS++::LukFiBlock ALIAS LukFiBlock)

# ----- Install instructions ---------------------------------------------------
include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/SMS++)

# Install library
install(TARGETS LukFiBlock
        EXPORT  LukFiBlock-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Install headers
install(FILES
        ${PROJECT_SOURCE_DIR}/LukFiBlock.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/LukFiBlock)

# Install export file *Targets.cmake
install(EXPORT LukFiBlock-targets
        FILE   LukFiBlockTargets.cmake
        NAMESPACE SMS++::
        DESTINATION ${INSTALL_CONFIGDIR})

# Create *ConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/LukFiBlockConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion)

# Create *Config.cmake
configure_package_config_file(
        ${CMAKE_CURRENT_LIST_DIR}/cmake/LukFiBlockConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/LukFiBlockConfig.cmake
        INSTALL_DESTINATION ${INSTALL_CONFIGDIR})

# Install the Config and ConfigVersion
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/LukFiBlockConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/LukFiBlockConfigVersion.cmake
        DESTINATION ${INSTALL_CONFIGDIR})

# ----- Export from the build tree ---------------------------------------------
export(EXPORT LukFiBlock-targets
       FILE ${CMAKE_CURRENT_BINARY_DIR}/LukFiBlockTargets.cmake
       NAMESPACE SMS++::)

# Register package in user's package registry (~/.cmake/registry)
export(PACKAGE LukFiBlock)
