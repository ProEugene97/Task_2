cmake_minimum_required(VERSION 3.1)
project(Task_2 C CXX)

include(ExternalProject)
find_package(Git REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(lib/googletest)
include_directories(lib/googletest/googletest/include)
include_directories(include/)

find_package(Threads)

add_library(count_lib STATIC static_lib/src/count.c)
target_include_directories(count_lib PUBLIC static_lib/src)


add_library(parallel_count_lib SHARED dynamic_lib/src/parallel_count.c)
target_include_directories(parallel_count_lib PUBLIC dynamic_lib/src)

add_executable(Task_2 src/main.c src/generate.c)

target_link_libraries(Task_2 count_lib parallel_count_lib ${CMAKE_THREAD_LIBS_INIT})

# ------------------------------------------------------------------------------
# Valgrind
# ------------------------------------------------------------------------------

set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --leak-check=full")
set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --track-fds=yes")
set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --trace-children=yes")
set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --error-exitcode=1")

# ------------------------------------------------------------------------------
# CppCheck
# ------------------------------------------------------------------------------

if(ENABLE_CPPCHECK)

    list(APPEND CPPCHECK_CMAKE_ARGS
            "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"
            )

    ExternalProject_Add(
            cppcheck
            GIT_REPOSITORY      https://github.com/danmar/cppcheck.git
            GIT_TAG             1.79
            GIT_SHALLOW         1
            CMAKE_ARGS          ${CPPCHECK_CMAKE_ARGS}
            PREFIX              ${CMAKE_BINARY_DIR}/external/cppcheck/prefix
            TMP_DIR             ${CMAKE_BINARY_DIR}/external/cppcheck/tmp
            STAMP_DIR           ${CMAKE_BINARY_DIR}/external/cppcheck/stamp
            DOWNLOAD_DIR        ${CMAKE_BINARY_DIR}/external/cppcheck/download
            SOURCE_DIR          ${CMAKE_BINARY_DIR}/external/cppcheck/src
            BINARY_DIR          ${CMAKE_BINARY_DIR}/external/cppcheck/build
    )

    list(APPEND CPPCHECK_ARGS
            --enable=warning,style,performance,portability,unusedFunction
            --std=c++11
            --verbose
            --error-exitcode=1
            --language=c++
            -DMAIN=main
            -I ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_SOURCE_DIR}/src/*.c
            ${CMAKE_SOURCE_DIR}/dynamic_lib/include
            ${CMAKE_SOURCE_DIR}/dynamic_lib/src/*.c
            ${CMAKE_SOURCE_DIR}/static_lib/include
            ${CMAKE_SOURCE_DIR}/static_lib/src/*.c
            )

    add_custom_target(
            check
            COMMAND ${CMAKE_BINARY_DIR}/bin/cppcheck ${CPPCHECK_ARGS}
            COMMENT "running cppcheck"
    )

endif()

# ------------------------------------------------------------------------------
# Clang Tidy
# ------------------------------------------------------------------------------

if(ENABLE_CLANG_TIDY)

    find_program(CLANG_TIDY_BIN clang-tidy-4.0)
    find_program(RUN_CLANG_TIDY_BIN run-clang-tidy-4.0.py)

    if(CLANG_TIDY_BIN STREQUAL "CLANG_TIDY_BIN-NOTFOUND")
        message(FATAL_ERROR "unable to locate clang-tidy-4.0")
    endif()

    if(RUN_CLANG_TIDY_BIN STREQUAL "RUN_CLANG_TIDY_BIN-NOTFOUND")
        message(FATAL_ERROR "unable to locate run-clang-tidy-4.0.py")
    endif()

    list(APPEND RUN_CLANG_TIDY_BIN_ARGS
            -clang-tidy-binary ${CLANG_TIDY_BIN}
            -header-filter=.*
            -checks=clan*,cert*,misc*,perf*,cppc*,read*,mode*,-cert-err58-cpp,-misc-noexcept-move-constructor
            )

    add_custom_target(
            tidy
            COMMAND ${RUN_CLANG_TIDY_BIN} ${RUN_CLANG_TIDY_BIN_ARGS}
            COMMENT "running clang tidy"
    )

endif()

# ------------------------------------------------------------------------------
# Coverage
# ------------------------------------------------------------------------------

if(ENABLE_COVERAGE)
    set_target_properties(count_lib parallel_count_lib
            PROPERTIES
            COMPILE_FLAGS "-g -O0 -ftest-coverage -fprofile-arcs"
            LINK_FLAGS    "-lgcov --coverage"
            )
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -g ")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov --coverage")
endif()

enable_testing()
add_executable(testing tests/test_count.cpp src/generate.c)
target_link_libraries(testing gtest gtest_main count_lib parallel_count_lib ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME FirstTest COMMAND testing)
