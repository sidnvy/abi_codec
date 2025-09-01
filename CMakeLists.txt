cmake_minimum_required(VERSION 3.14)
project(abi_codec)

set(CMAKE_CXX_STANDARD 17)

# Enable C language for libethc integration
enable_language(C)

# Modern dependency management with FetchContent
include(FetchContent)

# Build libethc as external dependency using FetchContent
option(WITH_LIBETHC "Build with libethc integration" ON)
if(WITH_LIBETHC)
    # Configure libethc build options BEFORE declaring FetchContent
    set(ETHC_BUILD_TESTS OFF CACHE BOOL "Disable libethc tests" FORCE)
    set(ETHC_BUILD_DOCS OFF CACHE BOOL "Disable libethc docs" FORCE)
    set(ETHC_DISABLE_SHARED_LIBS OFF CACHE BOOL "Enable shared libs" FORCE)

    # Use system dependencies (bundled versions seem incomplete)
    set(ETHC_TOMMATH_STATIC OFF CACHE BOOL "Use system libtommath" FORCE)
    set(ETHC_SECP256K1_STATIC OFF CACHE BOOL "Use system secp256k1" FORCE)

    # Download and build libethc from GitHub
    FetchContent_Declare(
        libethc
        GIT_REPOSITORY https://github.com/mhw0/libethc.git
        GIT_TAG main    # Use main branch (stable)
        SOURCE_SUBDIR .  # Build from root
    )

    # Disable problematic warnings for libethc (legacy code issues)
    set(CMAKE_C_FLAGS_BACKUP "${CMAKE_C_FLAGS}")
    set(CMAKE_CXX_FLAGS_BACKUP "${CMAKE_CXX_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-stringop-truncation -Wno-maybe-uninitialized -Wno-uninitialized -Wno-error=maybe-uninitialized")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-stringop-truncation -Wno-maybe-uninitialized -Wno-uninitialized -Wno-error=maybe-uninitialized")

    # Make libethc available
    FetchContent_MakeAvailable(libethc)

    # Restore original flags
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS_BACKUP}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_BACKUP}")
endif()

# Build Boost as external dependency using FetchContent
option(WITH_BOOST "Build with Boost integration" ON)
if(WITH_BOOST)
    # Download and build Boost from archives
    FetchContent_Declare(
        boost
        URL https://archives.boost.io/release/1.87.0/source/boost_1_87_0.tar.gz
        URL_HASH SHA256=0cf9684b2d11e7b94b3e9593eb7c30b6b9c9a0e5a77dc5b85365cf6a3a5ce1e9d
    )

    # Configure Boost build options
    set(BOOST_ENABLE_CMAKE ON CACHE BOOL "Enable CMake support for Boost" FORCE)
    set(BOOST_RUNTIME_LINK static CACHE STRING "Link Boost libraries statically" FORCE)

    # Make Boost available
    FetchContent_MakeAvailable(boost)

    # Add Boost include directories for header-only usage
    if(TARGET Boost::boost)
        get_target_property(BOOST_INCLUDE_DIRS Boost::boost INTERFACE_INCLUDE_DIRECTORIES)
        include_directories(${BOOST_INCLUDE_DIRS})
    elseif(TARGET Boost::headers)
        get_target_property(BOOST_INCLUDE_DIRS Boost::headers INTERFACE_INCLUDE_DIRECTORIES)
        include_directories(${BOOST_INCLUDE_DIRS})
    endif()
endif()

# Find dependencies for libethc
find_package(PkgConfig REQUIRED)
pkg_check_modules(TOMMATH REQUIRED libtommath)
pkg_check_modules(SECP256K1 REQUIRED libsecp256k1)

# Note: Performance comparison is now built by benchmarks/CMakeLists.txt

enable_testing()
add_subdirectory(tests)
add_subdirectory(examples)
add_subdirectory(benchmarks)