cmake_minimum_required(VERSION 3.21)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(PyProject)

project(${PyProject_NAME} LANGUAGES CXX VERSION ${PyProject_VERSION})
message(STATUS "Project: ${PROJECT_NAME}@v${PROJECT_VERSION}")



option(BUILD_TESTING "Build tests" OFF)
option(BUILD_PYTHON "Build python" OFF)


# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type to Release for better performance
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Optimize for performance in Release mode
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
endif()

include(Optimize)
project_set_default_optimizations()

# Enable testing support

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")


if(DEFINED SKBUILD)
    set(PYTHON_PROJECT_NAME "${SKBUILD_PROJECT_NAME}")
elseif(BUILD_PYTHON)
    set(PYTHON_PROJECT_NAME "${CMAKE_BINARY_DIR}")
endif()



if(DEFINED PYTHON_PROJECT_NAME)
    # Find Python and Cython
    string(REPLACE "-" "_" MODULE_NAME ${PyProject_NAME})

    find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)
    find_package(Cython MODULE REQUIRED VERSION 3.0)
    include(UseCython)

    # Find all .pyx files
    file(GLOB_RECURSE PYX_FILES "src/${MODULE_NAME}/*.pyx")

    # Configure Cython module for each .pyx file
    foreach(pyx_file ${PYX_FILES})
        get_filename_component(pyx_name ${pyx_file} NAME_WE)

        cython_transpile(${pyx_file} LANGUAGE CXX OUTPUT_VARIABLE source_file)

        # Create Python module
        python_add_library(${MODULE_NAME} MODULE "${source_file}" WITH_SOABI)

        # Install the Python module
        install(TARGETS ${MODULE_NAME}
            DESTINATION ${SKBUILD_PROJECT_NAME}
        )

        # Only process the first .pyx file to avoid duplicate targets
        break()
    endforeach()
endif()

# Add tests if BUILD_TESTING is enabled
if(BUILD_TESTING)
    enable_testing()

    # Create base64 test executable
    add_executable(test_base64
        tests/cpp/test_base64.cpp
        tests/cpp/test_main.cpp
    )

    # Include directories for tests
    target_include_directories(test_base64 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/cpp
    )

    # Add to test execution
    add_test(NAME test_base64 COMMAND test_base64)
endif()