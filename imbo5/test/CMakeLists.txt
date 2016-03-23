set(GOOGLE_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/googletest-release-1.7.0)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${GOOGLE_TEST_DIR}/include
)


add_executable(point_unit_test
    point_unit_test.cpp
)

add_executable(intervals_unit_test
    intervals_unit_test.cpp
)

target_link_libraries(intervals_unit_test
    ${GOOGLE_TEST_DIR}/build/libgtest.a
    pthread
)
 
add_test(
    NAME imbo5_point_unit_test
    COMMAND point_unit_test
)

add_test(
    NAME imbo5_intervals_unit_test
    COMMAND intervals_unit_test
)
