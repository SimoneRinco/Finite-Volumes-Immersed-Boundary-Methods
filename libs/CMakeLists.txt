include(libs)

add_custom_command(
    OUTPUT
        ${GOOGLE_TEST_DIR_PATH}
    COMMAND
        tar -xf ${GOOGLE_TEST_DIR}.tar.gz
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_command(
    OUTPUT
        ${GOOGLE_TEST_LIB_PATH}
    DEPENDS
        ${GOOGLE_TEST_DIR_PATH}
    COMMAND
        ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/cmake_build.sh
    ARGS
        ${GOOGLE_TEST_DIR_PATH} ${LIB_BUILD_DIR}
)
        
add_custom_target(test_libs
    DEPENDS
        ${GOOGLE_TEST_LIB_PATH}
)
