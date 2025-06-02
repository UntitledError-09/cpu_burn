# CMake build configuration for CPU burn
# Author: Harish Rohan Kambhampaty

cmake_minimum_required(VERSION 3.14)
project(cpu_burn VERSION 1.0.0 
        LANGUAGES CXX
        DESCRIPTION "CPU stress testing and power measurement tool"
        HOMEPAGE_URL "https://github.com/UntitledError-09/cpu_burn")

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
)

# Add executable target
add_executable(cpu_burn_exe src/main.cpp)
set_target_properties(cpu_burn_exe PROPERTIES OUTPUT_NAME cpu_burn)
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

# Define variables needed for packages depending on cpu_burn
set(CPU_BURN_CMAKE_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/cpu_burn" CACHE STRING
  "Installation directory for cmake files, relative to ${CMAKE_INSTALL_PREFIX}.")
set(CPU_BURN_INCLUDE_DIR "${CMAKE_INSTALL_INCLUDEDIR}" CACHE STRING
  "Installation directory for header files, relative to ${CMAKE_INSTALL_PREFIX}.")
set(CPU_BURN_LIBRARY_DIR "${CMAKE_INSTALL_LIBDIR}" CACHE STRING
  "Installation directory for libraries, relative to ${CMAKE_INSTALL_PREFIX}.")
    
# Configure the config file using our template
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cpu_burn-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cpu_burn-config.cmake
    @ONLY
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
