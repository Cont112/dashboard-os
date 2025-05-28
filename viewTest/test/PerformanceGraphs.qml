import QtQuick
import QtQuick.Controls
import QtGraphs

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: "Simple Line Graph"

    GraphsView {
        anchors.fill: parent

        axisX: ValueAxis {
            min: 0
            max: 5
        }

        axisY: ValueAxis {
            min: 0
            max: 10
        }

        LineSeries {
            name: "Sample Data"

            XYPoint { x: 0; y: 0 }
            XYPoint { x: 1; y: 1 }
            XYPoint { x: 2; y: 4 }
            XYPoint { x: 3; y: 2 }
            XYPoint { x: 4; y: 8 }
            XYPoint { x: 5; y: 10 }
        }
    }
}
