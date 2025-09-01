# Benchmarks CMakeLists.txt

# Enable debug build for better debugging
set(CMAKE_BUILD_TYPE Debug)

# Build performance comparison benchmark
add_executable(balanceof_perf_comparison balanceof_perf_comparison.cpp)

target_include_directories(balanceof_perf_comparison PRIVATE
    ../include
)

# Check if libethc is available (using FetchContent)
if(TARGET ethc)
    # Find system dependencies for libethc
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(TOMMATH QUIET libtommath)
        pkg_check_modules(SECP256K1 QUIET libsecp256k1)
    endif()

    # Only enable libethc if all dependencies are available
    if(TOMMATH_FOUND AND SECP256K1_FOUND)
        # Configure main benchmark with libethc support
        target_include_directories(balanceof_perf_comparison SYSTEM PRIVATE
            ${libethc_SOURCE_DIR}/include
            ${TOMMATH_INCLUDE_DIRS}
            ${SECP256K1_INCLUDE_DIRS}
        )

        # Suppress GMP-related warnings and string literal warnings
        target_compile_options(balanceof_perf_comparison PRIVATE
            -Wno-stringop-truncation
            -Wno-maybe-uninitialized
            -Wno-write-strings
        )

        target_link_libraries(balanceof_perf_comparison
            ethc
            ${TOMMATH_LIBRARIES}
            ${SECP256K1_LIBRARIES}
            m  # math library
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

        target_include_directories(simple_c_test PRIVATE
            ${libethc_SOURCE_DIR}/include
            ${TOMMATH_INCLUDE_DIRS}
            ${SECP256K1_INCLUDE_DIRS}
        )

        target_link_libraries(simple_c_test
            ethc
            ${TOMMATH_LIBRARIES}
            ${SECP256K1_LIBRARIES}
            m
        )

        set_target_properties(simple_c_test PROPERTIES
            INSTALL_RPATH "${CMAKE_BINARY_DIR}/_deps/libethc-build"
            BUILD_RPATH "${CMAKE_BINARY_DIR}/_deps/libethc-build"
        )

        message(STATUS "Building with full libethc support (FetchContent)")
    else()
        target_compile_definitions(balanceof_perf_comparison PRIVATE
            LIBETHC_AVAILABLE=0
        )
        message(STATUS "Building balanceof_perf_comparison without libethc support (missing system deps)")
    endif()
else()
    target_compile_definitions(balanceof_perf_comparison PRIVATE
        LIBETHC_AVAILABLE=0
    )
    message(STATUS "Building balanceof_perf_comparison without libethc support (libethc not available)")
endif()
