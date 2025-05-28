import QtQuick
import QtQuick.Controls
import DashboardOS
TableView {
    anchors.fill: parent
    columnSpacing: 1
    rowSpacing: 1
    clip: true

    model TableModel {}
    delegate: Rectangle {
        implicitWidth: 100
        implicitHeight: 50
        border.width: 1

        Text {
            text: process
            anchors.centerIn: display
        }
    }
}