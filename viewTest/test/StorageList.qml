import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
    
// Tab 3: Storage (List)
Item {
    id: storageListRoot
    property var storageModel: [] // Replace with back.getStorageData()

    ListView {
        id: storageList
        anchors.fill: parent
        anchors.margins: 10
        model: storageModel // Replace with back.getStorageData()
        clip: true

        delegate: Rectangle {
            width: ListView.view.width
            height: 80
            border.color: "gray"
            border.width: 1
            radius: 5
            color: "#f0f0f0"

            Column {
                anchors.fill: parent
                anchors.margins: 5
                spacing: 5

                Text { text: "Device: " + device; font.bold: true }
                Text { text: "Type: " + type }
                Text { text: "Mount: " + mount }
                Text { text: "Used: " + used + " / " + total }
            }
        }

        ScrollBar.vertical: ScrollBar {}
    }
}