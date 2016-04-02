include (libs)

macro(add_imbo5_test executable_name)
    add_executable(${executable_name}
        ${executable_name}.cpp
    )
    
    add_dependencies(${executable_name} test_libs)
    target_link_libraries(${executable_name}
        ${GOOGLE_TEST_LIB_PATH}
        pthread
    )

    add_test(
        NAME imbo5_${executable_name}
        COMMAND ${executable_name}
    )
endmacro()
