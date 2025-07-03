import QtQuick 6.9
import QtQuick.Controls 6.9
import QtQuick.Layouts 6.9
import QtGraphs 6.9

Rectangle {
    id: root
    width: 400
    height: 400
    color: "#f8f9fa"
    border.color: "#dee2e6"

    property real cpuUsage: 0.0
    property real idleUsage: 100.0

    function updateCpuUsage(usage) {
        cpuUsage = usage;
        idleUsage = 100.0 - usage;
        // The pie slices will automatically update due to property bindings
    }

    Component.onCompleted: {
        // Initialize with some data
        updateCpuUsage(3.4); // Initial CPU usage
    }

    // Title
    Text {
        id: title
        text: "CPU Usage"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 10
        font.pointSize: 14
        font.bold: true
        color: "#2c3e50"
    }

    GraphsView {
        id: graphsView
        anchors.fill: parent
        anchors.margins: 20
        anchors.topMargin: 40
        anchors.bottomMargin: 60

        PieSeries {
            id: pieSeries
            name: "CPU Usage"
            
            PieSlice {
                id: usedSlice
                label: cpuUsage.toFixed(1) + "%"
                value: cpuUsage
                color: "#7dd3c0"
                borderColor: "#5cb3a0"
                borderWidth: 2
                labelVisible: true
                labelColor: "#ffffff"
                labelFont.pointSize: 14
                labelFont.bold: true
            }
            
            PieSlice {
                id: idleSlice
                label: idleUsage.toFixed(1) + "%"
                value: idleUsage
                color: "#a8d5c7"
                borderColor: "#85c2b0"
                borderWidth: 2
                labelVisible: true
                labelColor: "#ffffff"
                labelFont.pointSize: 14
                labelFont.bold: true
            }
        }
    }

    // Fixed Legend with matching colors
    Row {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 10
        spacing: 20

        Row {
            spacing: 5
            Rectangle {
                width: 16
                height: 16
                color: "#7dd3c0"
                border.color: "#5cb3a0"
                border.width: 1
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                text: "CPU Used"
                font.pointSize: 10
                color: "#2c3e50"
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Row {
            spacing: 5
            Rectangle {
                width: 16
                height: 16
                color: "#a8d5c7"
                border.color: "#85c2b0"
                border.width: 1
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                text: "CPU Idle"
                font.pointSize: 10
                color: "#2c3e50"
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    // Connect to your model for real data
    Connections {
        target: processModel
        function onSystemStatsChanged() {
            if (processModel.systemCpuUsage !== undefined) {
                root.updateCpuUsage(processModel.systemCpuUsage);
            }
        }
    }
}