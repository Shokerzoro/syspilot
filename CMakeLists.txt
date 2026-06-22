# Sources
include(cmake/sources.cmake)

# Target
add_library(clientcommon
    ${CLIENTCOMMON_SOURCES}
)

target_compile_features(clientcommon PUBLIC cxx_std_17)

# Angle Includes
include(cmake/include.cmake)

target_include_directories(clientcommon
    PUBLIC
        ${CLIENTCOMMON_INCLUDE}
)

# DevKit Usage
if(COMMAND devkit_finalize)
    devkit_finalize(clientcommon)
endif()

# Tests
if(UNITER_BUILD_TESTS)
    include(cmake/test.cmake)
endif()
