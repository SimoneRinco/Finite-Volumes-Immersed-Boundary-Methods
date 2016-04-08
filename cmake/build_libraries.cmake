if(NOT BUILT_GOOGLE_TEST)
    message("Building google test library")
    execute_process(
        COMMAND
            ${PROJECT_SOURCE_DIR}/scripts/build_google_test.sh
        WORKING_DIRECTORY
            ${PROJECT_SOURCE_DIR}/scripts
        RESULT_VARIABLE
            GOOGLE_TEST_BUILD_RESULT
    )

    if(${GOOGLE_TEST_BUILD_RESULT} EQUAL 0)
        set(BUILT_GOOGLE_TEST "TRUE" CACHE STRING "Indicates cmake has built google test library correctly at configuration time")
    else()
        message(FATAL_ERROR "Error building google test library: " ${GOOGLE_TEST_BUILD_RESULT})
    endif()
endif()
