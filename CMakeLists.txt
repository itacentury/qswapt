cmake_minimum_required(VERSION 3.19)
project(QSwapt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/CompilerWarnings.cmake)
include(cmake/Sanitizers.cmake)

option(ENABLE_IPO "Enable link-time optimization for Release builds" OFF)
if(ENABLE_IPO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT _ipo_ok OUTPUT _ipo_err)
    if(_ipo_ok)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
        message(STATUS "IPO/LTO enabled for Release builds")
    else()
        message(WARNING "IPO/LTO requested but not supported: ${_ipo_err}")
    endif()
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME}
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui

    src/departure.h

    src/transport_client.h
    src/transport_client.cpp
)

qt_add_resources(${PROJECT_NAME}
    PREFIX "/"
    FILES resources/stations.txt
)

add_subdirectory(src/station)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Widgets
        Qt6::Network
        project_warnings
        project_sanitizers
        station_lib
)
