cmake_minimum_required(VERSION 3.16)
project(MyProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)  # not strictly needed for QML, but safe to include

set(QT_QML_GENERATE_QMLLS_INI ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2 Charts)

qt_add_executable(MyProject
    main.cpp
    back.cpp
    back.h
)

qt_add_qml_module(MyProject
    URI MyApp
    VERSION 1.0
    QML_FILES
        test/Main.qml
        test/PerformanceGraphs.qml
        test/NavigationBar.qml
        test/ProcessTable.qml
    RESOURCE_PREFIX /  # Ensures paths like qrc:/ExampleA/Main.qml work
)

target_link_libraries(MyProject
    PRIVATE Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick Qt6::QuickControls2
)
