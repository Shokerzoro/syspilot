file(GLOB_RECURSE CLIENTCOMMON_TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
)

if(CLIENTCOMMON_TEST_SOURCES)
    add_executable(clientcommon_tests
        ${CLIENTCOMMON_TEST_SOURCES}
    )

    target_link_libraries(clientcommon_tests
        PRIVATE
            GTest::gtest_main
            clientcommon
    )

    uniter_register_gtest(clientcommon_tests)
endif()
