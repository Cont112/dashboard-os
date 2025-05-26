import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    property var performanceModel: [] // Replace with back.getPerformanceData()

    ColumnLayout {
        spacing: 10
        anchors.fill: parent

        Text { text: "CPU Usage (%)"; font.bold: true }
        ChartView {
            ValueAxis {
                id: xAxisCPU
                min: 0
                max: 10
                titleText: "X-Axis"
            }
            ValueAxis {
                id: yAxisCPU
                min: 0
                max: 100
                titleText: "Y-Axis"
            }
            LineSeries {
                axisXCPU: xAxisCPU
                axisYCPU: yAxisCUP
                XYPoint { x: 0; y: 0 }
                XYPoint { x: 10; y: 100 }
            }
        }
        Text { text: "Memory Usage (%)"; font.bold: true }
        ChartView {
            ValueAxis {
                id: xAxisMemory
                min: 0
                max: 10
                titleText: "X-Axis"
            }
            ValueAxis {
                id: yAxisMemory
                min: 0
                max: 100
                titleText: "Y-Axis"
            }
            LineSeries {
                axisXMemory: xAxisMemory
                axisYMemory: yAxisMemory
                XYPoint { x: 0; y: 0 }
                XYPoint { x: 10; y: 100 }
            }
        }
    }
}
