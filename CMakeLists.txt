# CMake build configuration for CPU burn
# Author: Harish Rohan Kambhampaty

cmake_minimum_required(VERSION 3.14)
project(cpu_burn VERSION 1.0.0 
        LANGUAGES CXX
        DESCRIPTION "CPU stress testing tool that performs DGEMM operations using OpenBLAS"
        HOMEPAGE_URL "https://github.com/UntitledError-09/cpu_burn")

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configure RPATH settings
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Define installation directories according to GNU standards
include(GNUInstallDirs)

# Find OpenBLAS package
find_package(OpenBLAS REQUIRED)
if(NOT OpenBLAS_FOUND)
  message(FATAL_ERROR "OpenBLAS not found. Please install OpenBLAS development files.")
endif()

# Define library sources and headers
set(LIB_SOURCES
    src/cpu_burn.cpp
)

set(LIB_HEADERS
    include/cpu_burn/cpu_burn.hpp
)

# Add library target
add_library(cpu_burn SHARED ${LIB_SOURCES})
target_include_directories(cpu_burn
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(cpu_burn PUBLIC ${OpenBLAS_LIBRARIES})
target_include_directories(cpu_burn PRIVATE ${OpenBLAS_INCLUDE_DIRS})

# Set library version properties
set_target_properties(cpu_burn PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    # Explicitly state the output name to ensure consistency
    OUTPUT_NAME "cpu_burn"
)

# Add executable target
add_executable(cpu_burn_exe src/main.cpp)
set_target_properties(cpu_burn_exe PROPERTIES 
    OUTPUT_NAME cpu_burn
    INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
    BUILD_WITH_INSTALL_RPATH TRUE
)
target_link_libraries(cpu_burn_exe PRIVATE cpu_burn)

# Install targets
install(TARGETS cpu_burn cpu_burn_exe
    EXPORT cpu_burn-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install headers
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Export targets
install(EXPORT cpu_burn-targets
    FILE cpu_burn-targets.cmake
    NAMESPACE cpu_burn::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpu_burn
)

# Create and install config files
include(CMakePackageConfigHelpers)

# Define variables needed for packages depending on cpu_burn - use full paths
set(CPU_BURN_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/cmake/cpu_burn" CACHE STRING
  "Installation directory for cmake files")
set(CPU_BURN_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}" CACHE STRING
  "Installation directory for header files")
set(CPU_BURN_LIBRARY_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}" CACHE STRING
  "Installation directory for libraries")

# Configure the config file using our template
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cpu_burn-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cpu_burn-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpu_burn
    PATH_VARS 
        CPU_BURN_INCLUDE_DIR
        CPU_BURN_LIBRARY_DIR
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/cpu_burn-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Install config files
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/cpu_burn-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cpu_burn-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpu_burn
)

# Print debug info about paths
message(STATUS "CPU_BURN_CMAKE_DIR: ${CPU_BURN_CMAKE_DIR}")
message(STATUS "CPU_BURN_INCLUDE_DIR: ${CPU_BURN_INCLUDE_DIR}")
message(STATUS "CPU_BURN_LIBRARY_DIR: ${CPU_BURN_LIBRARY_DIR}")
message(STATUS "RPATH settings: ${CMAKE_INSTALL_RPATH}")
