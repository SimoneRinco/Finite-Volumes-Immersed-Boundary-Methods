add_library(imbo5 SHARED
    include/Point.h
    include/IntervalMap.h
    include/Exception.h
    source/Exception.cpp
)

target_include_directories(imbo5
    PUBLIC
        include
)


add_subdirectory(test)
