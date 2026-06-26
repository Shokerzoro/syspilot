# Versioning
project(syspilot VERSION 0.0.1 LANGUAGES CXX)

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

# DevKit Usage
if(SYSPILOT_COMPILE_SOURCES AND COMMAND devkit_finalize)
    devkit_finalize(syspilot)
endif()

# Tests
if(UNITER_BUILD_TESTS)
    include(cmake/test.cmake)
endif()




