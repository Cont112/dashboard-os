cmake_minimum_required(VERSION 3.16)

project(dashboard LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_PREFIX_PATH "/home/pedro/Qt/6.9.0/gcc_64/lib/cmake/")
set(CMAKE_PREFIX_PATH "/home/errera/Qt/6.9.0/gcc_64/lib/cmake/")
set(QT_QML_GENERATE_QMLLS_INI ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Quick Graphs QuickControls2)
qt_standard_project_setup()

qt_add_executable(dashboard
  main.cpp 
  modules/model/Model.cpp 
  modules/model/threads/Collector.cpp 
  modules/model/threads/Processor.cpp
  modules/view/View.cpp
)

qt_add_resources(dashboard "qml_resources"
    PREFIX "/"
    FILES
        qml/Main.qml
        qml/ProcessTable.qml
        qml/SystemOverview.qml
        qml/MemoryUsageGraph.qml
)

target_link_libraries(dashboard PRIVATE 
    Qt6::Widgets
    Qt6::Quick
    Qt6::Graphs
    Qt6::QuickControls2
)

install(TARGETS dashboard
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)