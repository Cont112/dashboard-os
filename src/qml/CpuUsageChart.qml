import QtQuick 6.9
import QtQuick.Controls 6.9
import QtQuick.Layouts 6.9
import QtGraphs 6.9

Rectangle {
    id: root
    width: 400
    height: 400
    color: "#2b2b2b"
    border.color: "#505050"
    border.width: 1
    radius: 8

    property real cpuUsage: 0.0
    property real idleUsage: 100.0

    function updateCpuUsage(usage) {
        cpuUsage = usage;
        idleUsage = 100.0 - usage;
    }

    Component.onCompleted: {
        updateCpuUsage(3.4);
    }

    Text {
        id: title
        text: "CPU Usage"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 10
        font.pointSize: 14
        font.bold: true
        color: "#ffffff"
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
                color: "#87CEEB"
                borderColor: "#6bb6c7"
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
                color: "#505050"
                borderColor: "#404040"
                borderWidth: 2
                labelVisible: true
                labelColor: "#ffffff"
                labelFont.pointSize: 14
                labelFont.bold: true
            }
        }
    }

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
                color: "#87CEEB"
                border.color: "#6bb6c7"
                border.width: 1
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                text: "CPU Used"
                font.pointSize: 10
                color: "#ffffff"
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Row {
            spacing: 5
            Rectangle {
                width: 16
                height: 16
                color: "#505050"
                border.color: "#404040"
                border.width: 1
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                text: "CPU Idle"
                font.pointSize: 10
                color: "#ffffff"
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    Connections {
        target: processModel
        function onSystemStatsChanged() {
            if (processModel.systemCpuUsage !== undefined) {
                root.updateCpuUsage(processModel.systemCpuUsage);
            }
        }
    }
}