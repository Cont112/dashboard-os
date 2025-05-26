import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts

Item {
    ColumnLayout {
        spacing: 10
        anchors.fill: parent

        Text { 
            text: "CPU Usage (%)" 
            font.bold: true 
            Layout.alignment: Qt.AlignHCenter
        }
        ChartView {
            id: cpuChart
            Layout.fillWidth: true
            Layout.fillHeight: true
            antialiasing: true
            // Remove title to avoid ChartTitle crash
            // title: "CPU Usage Over Time"

            ValueAxis {
                id: xAxisCPU
                min: 0
                max: 10
                titleText: "" // Disable axis title
            }
            ValueAxis {
                id: yAxisCPU
                min: 0
                max: 100
                titleText: "" // Disable axis title
            }
            LineSeries {
                axisX: xAxisCPU
                axisY: yAxisCPU
                name: "CPU Usage"
                XYPoint { x: 0; y: 0 }
                XYPoint { x: 10; y: 100 }
            }
        }
    }
}