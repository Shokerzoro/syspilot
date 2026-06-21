include(cmake/sources.cmake)
include(cmake/include.cmake)

add_library(clientcommon
    ${CLIENTCOMMON_SOURCES}
)

target_compile_features(clientcommon PUBLIC cxx_std_17)

target_include_directories(clientcommon
    PUBLIC
        ${CLIENTCOMMON_INCLUDE}
)

if(COMMAND devkit_finalize)
    devkit_finalize(clientcommon)
endif()

if(UNITER_BUILD_TESTS)
    include(cmake/test.cmake)
endif()
