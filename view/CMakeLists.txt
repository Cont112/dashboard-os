cmake_minimum_required(VERSION 3.16)
project(dashboard_os LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ENV{QTFRAMEWORK_BYPASS_LICENSE_CHECK} 1)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Quick Graphs QuickLayouts QuickControls2 REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/processmodel.cpp
    resources.qrc
)

qt_add_qml_module(${PROJECT_NAME}
    URI DashboardOS
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/NavigationBar.qml
        qml/ProcessTable.qml
        qml/PerformanceGraphs.qml
        qml/BasicGraph.qml
        qml/StorageTable.qml
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Quick
    Qt6::Graphs
    Qt6::QuickLayouts
    Qt6::QuickControls2
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)