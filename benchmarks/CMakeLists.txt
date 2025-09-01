# Benchmarks CMakeLists.txt

# Enable debug build for better debugging
set(CMAKE_BUILD_TYPE Debug)

# Enable C language for libethc integration
enable_language(C)

# Modern dependency management with FetchContent
include(FetchContent)

# Build libethc as external dependency using FetchContent
option(WITH_LIBETHC "Build with libethc integration for benchmarks" ON)
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

    # Find system dependencies for libethc
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(TOMMATH QUIET libtommath)
        pkg_check_modules(SECP256K1 QUIET libsecp256k1)
    endif()
endif()

# Create benchmark dependencies interface library
add_library(abi_codec_benchmark_deps INTERFACE)

# Add libethc to benchmark interface library if available
if(WITH_LIBETHC AND TARGET ethc)
    target_link_libraries(abi_codec_benchmark_deps INTERFACE ethc)
    # Add system dependencies for libethc
    if(PkgConfig_FOUND AND TOMMATH_FOUND AND SECP256K1_FOUND)
        target_include_directories(abi_codec_benchmark_deps INTERFACE
            ${libethc_SOURCE_DIR}/include
            ${TOMMATH_INCLUDE_DIRS}
            ${SECP256K1_INCLUDE_DIRS}
        )
        target_link_libraries(abi_codec_benchmark_deps INTERFACE
            ${TOMMATH_LIBRARIES}
            ${SECP256K1_LIBRARIES}
            m  # math library
        )
    endif()
endif()

# Build performance comparison benchmark
add_executable(balanceof_perf_comparison balanceof_perf_comparison.cpp)

target_include_directories(balanceof_perf_comparison PRIVATE
    ../include
)

# Link to benchmark dependencies (includes libethc if available)
target_link_libraries(balanceof_perf_comparison abi_codec_benchmark_deps)

# Check if libethc is available through the interface library
get_target_property(BENCHMARK_LINK_LIBRARIES abi_codec_benchmark_deps INTERFACE_LINK_LIBRARIES)
if(BENCHMARK_LINK_LIBRARIES AND "ethc" IN_LIST BENCHMARK_LINK_LIBRARIES)
    # Suppress GMP-related warnings and string literal warnings
    target_compile_options(balanceof_perf_comparison PRIVATE
        -Wno-stringop-truncation
        -Wno-maybe-uninitialized
        -Wno-write-strings
    )

    target_compile_definitions(balanceof_perf_comparison PRIVATE
        LIBETHC_AVAILABLE=1
    )

    set_target_properties(balanceof_perf_comparison PROPERTIES
        INSTALL_RPATH "${CMAKE_BINARY_DIR}/_deps/libethc-build"
        BUILD_RPATH "${CMAKE_BINARY_DIR}/_deps/libethc-build"
    )

    # Build simple C test
    add_executable(simple_c_test simple_c_test.c)

    target_include_directories(simple_c_test PRIVATE ../include)
    target_link_libraries(simple_c_test abi_codec_benchmark_deps)

    set_target_properties(simple_c_test PROPERTIES
        INSTALL_RPATH "${CMAKE_BINARY_DIR}/_deps/libethc-build"
        BUILD_RPATH "${CMAKE_BINARY_DIR}/_deps/libethc-build"
    )

    message(STATUS "Building with full libethc support")
else()
    target_compile_definitions(balanceof_perf_comparison PRIVATE
        LIBETHC_AVAILABLE=0
    )
    message(STATUS "Building without libethc support")
endif()
