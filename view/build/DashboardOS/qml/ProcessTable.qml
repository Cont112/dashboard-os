import QtQuick 6.9
import QtQuick.Controls 6.9
import QtQuick.Layouts 6.9
import Qt.labs.qmlmodels

Item{
    TableView {
        anchors.fill: parent
        columnSpacing: 1
        rowSpacing: 1
        clip: true
        model: TableModel{}

        delegate: Rectangle {
            implicitHeight: 40
            implicitWidth: 300

            Text {
                text: process
            }
        }

    }
}
