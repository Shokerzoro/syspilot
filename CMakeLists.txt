# Versioning
project(syspilot VERSION 0.0.3 LANGUAGES CXX)

# Sources
include(cmake/sources.cmake)

# Target
if(SYSPILOT_COMPILE_SOURCES)
    add_library(syspilot
        ${SYSPILOT_SOURCES}
    )
    set(SYSPILOT_TARGET_SCOPE PUBLIC)
else()
    add_library(syspilot INTERFACE)
    set(SYSPILOT_TARGET_SCOPE INTERFACE)
endif()

target_compile_features(syspilot ${SYSPILOT_TARGET_SCOPE} cxx_std_17)

# Angle Includes
include(cmake/include.cmake)

target_include_directories(syspilot
    ${SYSPILOT_TARGET_SCOPE}
        ${SYSPILOT_INCLUDE}
)

# SwiftCode verbose
if(COMMAND devkit_use_swiftcode)
    devkit_use_swiftcode(syspilot)

    set(SYSPILOT_VERBOSE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/verbose")
    set(SYSPILOT_VERBOSE_SOURCE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/environment"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/logging"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/paths"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/storage"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/args"
    )

    foreach(SYSPILOT_VERBOSE_SOURCE_DIR IN LISTS SYSPILOT_VERBOSE_SOURCE_DIRS)
        if(EXISTS "${SYSPILOT_VERBOSE_SOURCE_DIR}")
            file(GLOB_RECURSE SYSPILOT_VERBOSE_SOURCE_FILES CONFIGURE_DEPENDS
                "${SYSPILOT_VERBOSE_SOURCE_DIR}/*.h"
                "${SYSPILOT_VERBOSE_SOURCE_DIR}/*.hpp"
                "${SYSPILOT_VERBOSE_SOURCE_DIR}/*.hxx"
                "${SYSPILOT_VERBOSE_SOURCE_DIR}/*.c"
                "${SYSPILOT_VERBOSE_SOURCE_DIR}/*.cc"
                "${SYSPILOT_VERBOSE_SOURCE_DIR}/*.cpp"
                "${SYSPILOT_VERBOSE_SOURCE_DIR}/*.cxx"
            )

            if(SYSPILOT_VERBOSE_SOURCE_FILES)
                devkit_swiftcode_verbose(
                    "${SYSPILOT_VERBOSE_SOURCE_DIR}"
                    "${SYSPILOT_VERBOSE_DIR}"
                )
            endif()
        endif()
    endforeach()

    target_include_directories(syspilot
        ${SYSPILOT_TARGET_SCOPE}
            "${CMAKE_CURRENT_SOURCE_DIR}/.."
    )

    add_library(syspilot_verbose_include INTERFACE)
    target_include_directories(syspilot_verbose_include INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
    )
endif()

# DevKit Usage
if(DEBUG_BUILD AND COMMAND devkit_use_qt_shared)
    devkit_use_qt_shared(syspilot Core Network)
elseif(RELEASE_BUILD AND COMMAND devkit_use_qt_static)
    devkit_use_qt_static(syspilot Core Network)
endif()

if(SYSPILOT_COMPILE_SOURCES AND COMMAND devkit_finalize)
    devkit_finalize(syspilot)
endif()

# Tests
if(UNITER_BUILD_TESTS)
    include(cmake/test.cmake)
endif()




