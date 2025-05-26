import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root
    // Property to allow passing in data from the outside
    property var modelData: []

    Column {
        spacing: 0
        anchors.fill: parent

        Row {
            height: 40
            spacing: 1

            Rectangle {
                width: 100; height: 40; color: "#4CAF50"
                Text { anchors.centerIn: parent; text: "Name"; color: "white"; font.bold: true }
            }
            Rectangle {
                width: 50; height: 40; color: "#4CAF50"
                Text { anchors.centerIn: parent; text: "Age"; color: "white"; font.bold: true }
            }
            Rectangle {
                width: 150; height: 40; color: "#4CAF50"
                Text { anchors.centerIn: parent; text: "City"; color: "white"; font.bold: true }
            }
        }

        Repeater {
            model: root.modelData

            Row {
                spacing: 1

                Rectangle {
                    width: 100; height: 40; border.color: "#ccc"
                    Text { anchors.centerIn: parent; text: modelData.name }
                }
                Rectangle {
                    width: 50; height: 40; border.color: "#ccc"
                    Text { anchors.centerIn: parent; text: modelData.age }
                }
                Rectangle {
                    width: 150; height: 40; border.color: "#ccc"
                    Text { anchors.centerIn: parent; text: modelData.city }
                }
            }
        }
    }
}
