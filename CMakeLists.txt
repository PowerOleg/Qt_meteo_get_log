cmake_minimum_required(VERSION 3.14)

project(meteo_get_log LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core SerialPort)

add_executable(meteo_get_log
  main.cpp
  uartcontroller.h
          uartcontroller.cpp
)
target_link_libraries(meteo_get_log Qt6::Core
                                    Qt6::SerialPort)

install(TARGETS meteo_get_log
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
