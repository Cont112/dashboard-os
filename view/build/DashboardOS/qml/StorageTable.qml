import QtQuick 6.9
import QtQuick.Controls 6.9
import QtQuick.Layouts 6.9
    
// Tab 3: Storage (List)
Item {
    id: storageListRoot
    property var storageData: [] 

    ScrollView {
        anchors.fill: parent
        clip: true
            
        TableView {
            anchors.fill: parent
            model: storageData 

            TableViewColumn {
                title: "Device"
                role: "device"
                width: 200
            }
            TableViewColumn {
                title: "Type"
                role: "type"
                width: 100
            }
            TableViewColumn {
                title: "Mount"
                role: "mount"
                width: 150
            }
            TableViewColumn {
                title: "Used"
                role: "used"
                width: 100
            }
            TableViewColumn {
                title: "Total"
                role: "total"
                width: 100
            } 
        }


    }
}
