import QtQuick 6.9
import QtQuick.Controls 6.9
import QtQuick.Layouts 6.9
import QtGraphs 6.9

Item {
    property var cpuRanges: { minX, maxX, minY, maxY}
    property var cpuData: []
    property var memoryRanges: { minX, maxX, minY, maxY}
    property var memoryData: []

    ScrollView {
        anchors.fill: parent
        clip: true
        ColumnLayout {
            anchors.fill: parent

            Text {
                text: "cpu Graph"
                font.pointSize: 24
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
                Layout.topMargin: 10
            }
            BasicGraph{
                Layout.fillWidth: true
                ranges: cpuRanges
                data: cpuData
            }

            Text {
                text: "Memory Graph"
                font.pointSize: 24
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
                Layout.topMargin: 10
            }
            BasicGraph{
                Layout.fillWidth: true
                ranges: memoryRanges
                data: memoryData
            }
        }
    }
}
