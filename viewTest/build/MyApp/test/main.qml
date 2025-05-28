import QtQuick
import QtQuick.Window
import QtGraphs

Window {
    width: 640
    height: 480
    visible: true
    title: "Qt Graphs - Minimal LineGraph"

    LineGraph {
        anchors.fill: parent

        LineSeries {
            axisX: ValueAxis {
                min: 0
                max: 5
            }

            axisY: ValueAxis {
                min: 0
                max: 10
            }

            points: [
                Point { x: 0; y: 0 },
                Point { x: 1; y: 3 },
                Point { x: 2; y: 5 },
                Point { x: 3; y: 4 },
                Point { x: 4; y: 7 },
                Point { x: 5; y: 6 }
            ]
        }
    }
}

