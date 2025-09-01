cmake_minimum_required(VERSION 3.14)
project(abi_codec)

set(CMAKE_CXX_STANDARD 17)

# Modern dependency management with FetchContent
include(FetchContent)

# Build Boost as external dependency using ExternalProject (headers only)
option(WITH_BOOST "Build with Boost integration" ON)
if(WITH_BOOST)
    # First try to find system Boost
    find_package(Boost 1.70 QUIET)
    if(Boost_FOUND)
        message(STATUS "Found system Boost: ${Boost_VERSION}")
        set(BOOST_INCLUDE_DIR "${Boost_INCLUDE_DIRS}")
        add_library(boost_headers INTERFACE)
    else()
        message(STATUS "System Boost not found, downloading Boost 1.87.0 headers...")
        include(ExternalProject)
        ExternalProject_Add(
            boost_download
            URL https://archives.boost.io/release/1.87.0/source/boost_1_87_0.tar.gz
            SOURCE_DIR "${CMAKE_BINARY_DIR}/boost"
            CONFIGURE_COMMAND ""
            BUILD_COMMAND ""
            INSTALL_COMMAND ""
            TEST_COMMAND ""
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        )
        set(BOOST_INCLUDE_DIR "${CMAKE_BINARY_DIR}/boost")
        add_library(boost_headers INTERFACE)
        add_dependencies(boost_headers boost_download)
    endif()
endif()

# Create interface library for general dependencies (Boost)
add_library(abi_codec_deps INTERFACE)

# Add Boost to the general interface library if available
if(WITH_BOOST)
    if(Boost_FOUND)
        # Use system Boost
        target_include_directories(abi_codec_deps INTERFACE ${Boost_INCLUDE_DIRS})
        if(Boost_LIBRARIES)
            target_link_libraries(abi_codec_deps INTERFACE ${Boost_LIBRARIES})
        endif()
    else()
        # Use downloaded Boost headers
        target_include_directories(abi_codec_deps INTERFACE ${BOOST_INCLUDE_DIR})
        target_link_libraries(abi_codec_deps INTERFACE boost_headers)
    endif()
endif()

# Note: Performance comparison is now built by benchmarks/CMakeLists.txt

enable_testing()
add_subdirectory(tests)
add_subdirectory(examples)
add_subdirectory(benchmarks)