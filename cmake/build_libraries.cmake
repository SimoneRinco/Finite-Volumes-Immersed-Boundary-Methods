message("Building google test library")
execute_process(
    COMMAND
        ${PROJECT_SOURCE_DIR}/scripts/build_google_test.sh
    WORKING_DIRECTORY
        ${PROJECT_SOURCE_DIR}/scripts
    RESULT_VARIABLE
        GOOGLE_TEST_BUILD_RESULT
)

if(NOT ${GOOGLE_TEST_BUILD_RESULT} EQUAL 0) 
    message(FATAL_ERROR "Error building google test library: " ${GOOGLE_TEST_BUILD_RESULT})
endif() 
