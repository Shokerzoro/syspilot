file(GLOB_RECURSE SYSPILOT_TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
)

if(SYSPILOT_TEST_SOURCES)
    add_executable(syspilot_tests
        ${SYSPILOT_TEST_SOURCES}
    )

    target_link_libraries(syspilot_tests
        PRIVATE
            GTest::gtest_main
            syspilot
    )

    if(TARGET Qt6::Core)
        target_link_libraries(syspilot_tests
            PRIVATE
                Qt6::Core
        )
    endif()

    uniter_register_gtest(syspilot_tests)
endif()
