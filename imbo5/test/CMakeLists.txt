include(libs)
include(add_imbo5_test)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${GOOGLE_TEST_DIR_PATH}/include
)

add_imbo5_test(point_unit_test)
add_imbo5_test(intervals_unit_test)
