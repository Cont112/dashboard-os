import QtQuick 6.9
import QtQuick.Controls 6.9
import QtQuick.Layouts 6.9

Rectangle {
    id: systemOverview
    color: "#2b2b2b"
    border.color: "#505050"
    border.width: 1
    radius: 8
    height: 120
    
    RowLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 20
        
        // CPU Usage Panel
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#3c3c3c"
            border.color: "#505050"
            border.width: 1
            radius: 8
            
            RowLayout {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 14
                
                ColumnLayout {
                    Layout.fillWidth: true
                    
                    Text {
                        text: "CPU Usage"
                        font.bold: true
                        font.pointSize: 12
                        color: "#ffffff"
                    }
                    
                    Text {
                        text: processModel.systemCpuUsage.toFixed(1) + "%"
                        font.pointSize: 14
                        font.bold: true
                        color: processModel.systemCpuUsage > 80 ? "#e74c3c" : 
                              processModel.systemCpuUsage > 60 ? "#f39c12" : "#27ae60"
                    }
                }
            }
        }
        
        // Memory Usage Panel
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#3c3c3c"
            border.color: "#505050"
            border.width: 1
            radius: 8
            
            RowLayout {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 8
                
                ColumnLayout {
                    Layout.fillWidth: true
                    
                    Text {
                        text: "Memory Usage"
                        font.bold: true
                        font.pointSize: 12
                        color: "#ffffff"
                    }
                    
                    Text {
                        text: processModel.systemMemoryUsage.toFixed(1) + "%"
                        font.pointSize: 14
                        font.bold: true
                        color: processModel.systemMemoryUsage > 80 ? "#e74c3c" : 
                              processModel.systemMemoryUsage > 60 ? "#f39c12" : "#27ae60"
                    }
                }
                
                ColumnLayout {
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    spacing: 2
                    
                    Text {
                        text: processModel.availableMemoryMB + " MB free"
                        font.pointSize: 8
                        color: "#27ae60"
                        horizontalAlignment: Text.AlignRight
                    }
                    
                    Text {
                        text: processModel.totalMemoryMB + " MB total"
                        font.pointSize: 8
                        color: "#cccccc"
                        horizontalAlignment: Text.AlignRight
                    }
                }
            }
        }
        
        // Process Count Panel
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#3c3c3c"
            border.color: "#505050"
            border.width: 1
            radius: 8
            
            RowLayout {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 16
                
                ColumnLayout {
                    Layout.fillWidth: true
                    
                    Text {
                        text: "Processes"
                        font.bold: true
                        font.pointSize: 12
                        color: "#ffffff"
                    }
                    
                    Text {
                        text: processModel.totalProcesses.toString()
                        font.pointSize: 18
                        font.bold: true
                        color: "#87CEEB"
                    }
                }
                
                ColumnLayout {
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    spacing: 2
                    
                    Text {
                        text: processModel.runningProcesses + " running"
                        font.pointSize: 9
                        color: "#27ae60"
                        horizontalAlignment: Text.AlignRight
                    }
                    
                    Text {
                        text: processModel.sleepingProcesses + " sleeping"
                        font.pointSize: 9
                        color: "#cccccc"
                        horizontalAlignment: Text.AlignRight
                    }
                    
                    Text {
                        text: processModel.idleProcesses + " idle"
                        font.pointSize: 9
                        color: "#999999"
                        horizontalAlignment: Text.AlignRight
                    }
                }
            }
        }
        
        // Thread Count Panel
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#3c3c3c"
            border.color: "#505050"
            border.width: 1
            radius: 8
            
            RowLayout {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 16
                
                ColumnLayout {
                    Layout.fillWidth: true
                    
                    Text {
                        text: "Threads"
                        font.bold: true
                        font.pointSize: 12
                        color: "#ffffff"
                    }
                    
                    Text {
                        text: processModel.totalThreads.toString()
                        font.pointSize: 18
                        font.bold: true
                        color: "#87CEEB"
                    }
                }
            }
        }
    }
} 