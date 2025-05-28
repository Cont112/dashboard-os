cmake_minimum_required(VERSION 3.16)
project(DynamicGraphExample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Graphs)

qt_add_executable(app
    main.cpp
)

qt_add_qml_module(app
    URI MyApp
    VERSION 1.0
    QML_FILES main.qml
)

target_link_libraries(app
    PRIVATE Qt6::Quick Qt6::Graphs
)
