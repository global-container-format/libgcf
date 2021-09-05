cmake_minimum_required(VERSION 3.20)

include(CMakePackageConfigHelpers)
include(version.cmake)
include(features.cmake)
include(functions.cmake)
include(test-defs.cmake)

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif()

enable_testing()

project(
    GCF
    VERSION ${GCF_VERSION_MAJOR}.${GCF_VERSION_MINOR}.${GCF_VERSION_PATCH}
    DESCRIPTION "Global Container Format"
    HOMEPAGE_URL https://github.com/global-container-format/libgcf
)

find_package(ZLIB REQUIRED)

set(Python_FIND_VIRTUALENV FIRST)
find_package(Python)

find_program(LLVM_PROFDATA_EXE llvm-profdata)
find_program(LLVM_COV_EXE llvm-cov)

if(Python_FOUND)
    check_python_module(gcfconv)

    if(Python_module_gcfconv_FOUND)
        set(GCF_ENABLE_TESTS ON)
        set(GCF_ENABLE_COVERAGE ${GCF_FEATURE_COVERAGE})
    else()
        message(WARNING "Python module \"gcfconv\" not found. Tests will not be available.")
    endif()
else()
    message(WARNING "Python not found. Tests will not be available.")
endif()

if(
    (
        ${LLVM_PROFDATA_EXE} STREQUAL LLVM_PROFDATA_EXE-NOTFOUND
        OR ${LLVM_COV_EXE} STREQUAL LLVM_COV_EXE-NOTFOUND
    )
    AND GCF_ENABLE_COVERAGE
)
    message(WARNING "Coverage tools not found - no coverage will be available.")
    set(GCF_ENABLE_COVERAGE OFF)
endif()

# Source
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${SRC_DIR}/include)
set(BINARY_INCLUDE_DIR ${PROJECT_BINARY_DIR}/include)
set(VERSION_HEADER_TEMPLATE ${INCLUDE_DIR}/gcf/version.h.in)
set(VERSION_HEADER include/gcf/version.h)
set(DEFS_HEADER_TEMPLATE ${INCLUDE_DIR}/gcf/defs.h.in)
set(DEFS_HEADER include/gcf/defs.h)

# Test
set(TEST_DIR test)
set(TEST_UTILS_DIR ${TEST_DIR}/lib)
set(TEST_INCLUDE_DIR ${TEST_DIR}/include)
set(TEST_DEFS_HEADER_TEMPLATE ${TEST_INCLUDE_DIR}/gcf/test-defs.h.in)
set(TEST_DEFS_HEADER include/gcf/test-defs.h)

# Coverage
set(COVERAGE_DIR ${PROJECT_BINARY_DIR}/coverage)
set(COVERAGE_REPORT_RAW ${COVERAGE_DIR}/gcf.profdata)
set(COVERAGE_REPORT_SUMMARY ${COVERAGE_DIR}/gcf-summary.txt)
set(COVERAGE_REPORT_DETAIL ${COVERAGE_DIR}/gcf.txt)

file(
    GLOB_RECURSE GCF_SOURCES
    LIST_DIRECTORIES false
    ${SRC_DIR}/*.c
)
file(
    GLOB GCF_TEST_SOURCES
    LIST_DIRECTORIES false
    RELATIVE ${PROJECT_SOURCE_DIR}
    ${TEST_DIR}/*.c
)
file(
    GLOB GCF_COLOR_MAP_TEST_SOURCES
    LIST_DIRECTORIES false
    RELATIVE ${PROJECT_SOURCE_DIR}
    ${TEST_DIR}/color-map/*.c
)
file(
    GLOB_RECURSE GCF_TEST_UTILS_SOURCES
    LIST_DIRECTORIES false
    RELATIVE ${PROJECT_SOURCE_DIR}
    ${TEST_UTILS_DIR}/*.c
)

if(GCF_BUILD_DLL)
    set(GCF_LIB_MODE SHARED)
else()
    set(GCF_LIB_MODE STATIC)
endif()

add_library(gcf ${GCF_LIB_MODE} ${GCF_SOURCES})
target_compile_definitions(gcf PRIVATE GCF_SOURCE)
target_include_directories(
    gcf PUBLIC
        $<BUILD_INTERFACE:${INCLUDE_DIR}>
        $<BUILD_INTERFACE:${BINARY_INCLUDE_DIR}>
)
target_link_libraries(gcf PRIVATE ZLIB::ZLIB)

if(GCF_ENABLE_COVERAGE)
    target_compile_options(
        gcf
        PRIVATE
            --coverage
            -fprofile-arcs
            -ftest-coverage
            -fcoverage-mapping
            -fprofile-instr-generate
    )
endif()

configure_file(
    ${VERSION_HEADER_TEMPLATE}
    ${VERSION_HEADER}
    NEWLINE_STYLE LF
)

configure_file(
    ${DEFS_HEADER_TEMPLATE}
    ${DEFS_HEADER}
    NEWLINE_STYLE LF
)

configure_file(
    ${TEST_DEFS_HEADER_TEMPLATE}
    ${TEST_DEFS_HEADER}
    NEWLINE_STYLE LF
)

add_library(gcf_test_utils STATIC ${GCF_TEST_UTILS_SOURCES})
target_link_libraries(gcf_test_utils PUBLIC gcf)
target_include_directories(gcf_test_utils PUBLIC ${TEST_INCLUDE_DIR})
target_compile_definitions(gcf_test_utils PRIVATE _CRT_SECURE_NO_WARNINGS)

# Create test resources
include(test-resources.cmake)

if(GCF_ENABLE_TESTS)
    add_module_tests(core GCF_CORE_TESTS GCF_CORE_COVERAGE_FILES ${GCF_TEST_SOURCES})
    add_module_tests(image GCF_COLORMAP_TESTS GCF_COLORMAP_COVERAGE_FILES ${GCF_COLOR_MAP_TEST_SOURCES})
endif()

list(APPEND GCF_TESTS ${GCF_CORE_TESTS} ${GCF_COLORMAP_TESTS})
list(APPEND GCF_COVERAGE_FILES ${GCF_CORE_COVERAGE_FILES} ${GCF_COLORMAP_COVERAGE_FILES})

if(GCF_ENABLE_COVERAGE)
    add_custom_command(
        OUTPUT ${GCF_COVERAGE_FILES}
        COMMAND ${CMAKE_COMMAND} ARGS --build ${PROJECT_BINARY_DIR} --target test
        COMMENT "Generating coverage data"
    )

    add_custom_command(
        OUTPUT ${COVERAGE_REPORT_RAW}
        COMMAND ${LLVM_PROFDATA_EXE} ARGS merge -sparse ${GCF_COVERAGE_FILES} -o ${COVERAGE_REPORT_RAW}
        DEPENDS ${GCF_COVERAGE_FILES}
        COMMENT "Merging coverage data"
    )

    foreach(source_file IN LISTS GCF_SOURCES)
        cmake_path(RELATIVE_PATH source_file BASE_DIRECTORY ${PROJECT_BINARY_DIR})
        set(COVERAGE_RELATIVE_SOURCES ${COVERAGE_RELATIVE_SOURCES} ${source_file})
    endforeach()

    add_custom_command(
        OUTPUT ${COVERAGE_REPORT_SUMMARY}
        COMMAND ${LLVM_COV_EXE} ARGS report --show-functions --instr-profile "${COVERAGE_REPORT_RAW}" $<TARGET_FILE:gcf> ${COVERAGE_RELATIVE_SOURCES} > "${COVERAGE_REPORT_SUMMARY}"
        COMMAND ${LLVM_COV_EXE} ARGS show --instr-profile "${COVERAGE_REPORT_RAW}" $<TARGET_FILE:gcf> > "${COVERAGE_REPORT_DETAIL}"
        DEPENDS ${COVERAGE_REPORT_RAW}
        COMMENT "Generating coverage report"
    )

    add_custom_target(
        coverage
        DEPENDS ${COVERAGE_REPORT_SUMMARY}
    )

    set_target_properties(
        coverage
        PROPERTIES
            ADDITIONAL_CLEAN_FILES ${COVERAGE_DIR}
    )
endif()

set(GCF_CONFIG_INSTALL_DIR lib/cmake/${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})

install(
    TARGETS gcf
        EXPORT gcf
        RUNTIME
        LIBRARY
        PUBLIC_HEADER
)

install(
    EXPORT gcf
        DESTINATION ${GCF_CONFIG_INSTALL_DIR}
        NAMESPACE GCF::
        FILE ${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.cmake
)

set(GCF_CONFIG_VERSION_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake)

write_basic_package_version_file(
    ${GCF_CONFIG_VERSION_FILE}
    COMPATIBILITY SameMinorVersion
)

install(
    FILES ${GCF_CONFIG_VERSION_FILE}
    DESTINATION ${GCF_CONFIG_INSTALL_DIR}
)

install(
    DIRECTORY ${INCLUDE_DIR}/
    TYPE INCLUDE
    FILES_MATCHING
    PATTERN "*.h"
)

install(
    DIRECTORY ${BINARY_INCLUDE_DIR}/
    TYPE INCLUDE
    FILES_MATCHING
    PATTERN "*.h"
)
