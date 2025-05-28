import QtQuick
import QtQuick.Controls
import QtCharts

ApplicationWindow {
    id: window
    visible: true
    width: 800
    height: 600
    title: "System Monitor"

    PerformanceGraphs {
        anchors.fill: parent
    }
}