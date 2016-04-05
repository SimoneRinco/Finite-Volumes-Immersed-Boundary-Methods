macro(add_imbo5_test executable_name)
    add_executable(${executable_name}
        ${executable_name}.cpp
    )
    
    target_include_directories(${executable_name} SYSTEM
        PUBLIC
            ${GOOGLE_TEST_INCLUDE}
    )

    target_link_libraries(${executable_name}
        ${LIB_GOOGLE_TEST}
        pthread
    )

    add_test(
        NAME imbo5_${executable_name}
        COMMAND ${executable_name}
    )
endmacro()
