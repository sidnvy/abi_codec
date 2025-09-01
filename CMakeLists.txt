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

# Find Boost (needed for some components)
find_package(Boost REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
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