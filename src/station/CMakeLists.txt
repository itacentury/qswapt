add_library(station_lib
    station.h
    station_reader.h
    station_reader.cpp
)

target_link_libraries(station_lib PRIVATE Qt6::Core)
target_include_directories(station_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
