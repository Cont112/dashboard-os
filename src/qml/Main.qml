import QtQuick 6.9
import QtQuick.Controls 6.9
import QtQuick.Layouts 6.9

ApplicationWindow {
    id: window
    visible: true
    title: "System Monitor"
    color: "#f0f0f0"
    property int currentTab: 0
    
    Button {
        text: "✕"
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 10
        width: 30
        height: 30
        z: 1000 
        onClicked: Qt.quit()
        background: Rectangle {
            color: parent.hovered ? "#e74c3c" : "#95a5a6"
            radius: 15
            border.color: "#7f8c8d"
            border.width: 1
        }
        contentItem: Text {
            text: parent.text
            font.pointSize: 12
            font.bold: true
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
    
    ColumnLayout {
        spacing: 0
        anchors.fill: parent

        SystemOverview {
            Layout.fillWidth: true
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: window.currentTab

            //ProcessTable {
            //    Layout.fillWidth: true
            //    Layout.fillHeight: true
            //}
            // MemoryUsageGraph {
            //     Layout.fillWidth: true
            //     Layout.fillHeight: true
            // }
            CpuUsageChart {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
}
