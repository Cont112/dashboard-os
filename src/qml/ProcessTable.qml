import QtQuick 6.9
import QtQuick.Controls 6.9
import QtQuick.Layouts 6.9
import Qt.labs.qmlmodels



Item {
    id: processTable
    
    property real currentWidth: width
    
    // Sinal para duplo clique em processo
    signal processDoubleClicked(int pid)
    
    onCurrentWidthChanged: {
        if (tableView) {
            tableView.forceLayout()
        }
    }
    
    ScrollView {
        anchors.fill: parent
        clip: true
        
        Column {
            width: processTable.width

            Rectangle {
                id: header
                width: processTable.width
                height: 40
                color: "#e0e0e0"
                border.color: "#e0e0e0"
                
                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 1
                    spacing: 1
                    
                    Rectangle {
                        Layout.preferredWidth: processTable.width * 0.08
                        Layout.fillHeight: true
                        color: "transparent"
                        Text { 
                            anchors.centerIn: parent
                            text: "PID"
                            font.bold: true
                            color: "#333333"
                        }
                    }
                    Rectangle {
                        Layout.preferredWidth: processTable.width * 0.25 
                        Layout.fillHeight: true
                        color: "transparent"
                        Text { 
                            anchors.centerIn: parent
                            text: "Process Name"
                            font.bold: true
                            color: "#333333"
                        }
                    }
                    Rectangle {
                        Layout.preferredWidth: processTable.width * 0.12 
                        Layout.fillHeight: true
                        color: "transparent"
                        Text { 
                            anchors.centerIn: parent
                            text: "State"
                            font.bold: true
                            color: "#333333"
                        }
                    }
                    Rectangle {
                        Layout.preferredWidth: processTable.width * 0.12 
                        Layout.fillHeight: true
                        color: "transparent"
                        Text { 
                            anchors.centerIn: parent
                            text: "CPU (%)"
                            font.bold: true
                            color: "#333333"
                        }
                    }
                    Rectangle {
                        Layout.preferredWidth: processTable.width * 0.15  
                        Layout.fillHeight: true
                        color: "transparent"
                        Text { 
                            anchors.centerIn: parent
                            text: "Memory (%)"
                            font.bold: true
                            color: "#333333"
                        }
                    }
                    Rectangle {
                        Layout.preferredWidth: processTable.width * 0.15
                        Layout.fillHeight: true
                        color: "transparent"
                        Text { 
                            anchors.centerIn: parent
                            text: "User"
                            font.bold: true
                            color: "#333333"
                        }
                    }
                    Rectangle {
                        Layout.preferredWidth: processTable.width * 0.13
                        Layout.fillHeight: true
                        color: "transparent"
                        Text { 
                            anchors.centerIn: parent
                            text: "Threads"
                            font.bold: true
                            color: "#333333"
                        }
                    }
                }
            }
            
            TableView {
                id: tableView
                width: processTable.width
                height: processTable.height - header.height
                columnSpacing: 1
                rowSpacing: 1
                clip: true
                model: processModel
                reuseItems: false


                columnWidthProvider: function(column) {
                    var w = processTable.width;
                    switch(column) {
                        case 0: return w * 0.08;  
                        case 1: return w * 0.25;  
                        case 2: return w * 0.12; 
                        case 3: return w * 0.12;  
                        case 4: return w * 0.15;  
                        case 5: return w * 0.15;  
                        case 6: return w * 0.13;
                        default: return 0;
                    }
                }

                delegate: Rectangle {
                    implicitHeight: 40
                    implicitWidth: 100
                    color: row % 2 === 0 ? "#f0f0f0" : "#ffffff"
                    border.color: "#e0e0e0"

                    Text {
                        anchors.fill: parent
                        anchors.margins: 8
                        text: {
                            switch(column) {
                                case 0: return model.pid || "";
                                case 1: return model.name || "";
                                case 2: return model.state || "";
                                case 3: return (model.cpu_usage !== undefined) ? model.cpu_usage.toFixed(1) : "0.0";
                                case 4: return (model.memory_usage !== undefined) ? model.memory_usage.toFixed(2) : "0.00";
                                case 5: return model.username || "";
                                case 6: return (model.threads !== undefined) ? model.threads.toString() : "0";
                                default: return "";
                            }
                        }
                        color: "black"
                        font.pointSize: 11
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                    }

                    MouseArea {
                        anchors.fill: parent
                        onDoubleClicked: {
                            processTable.processDoubleClicked(model.pid)
                        }
                    }
                }
            }
        }
    }
}
