import QtQuick 6.9
import QtQuick.Controls 6.9
import QtQuick.Layouts 6.9

ApplicationWindow {
    id: window
    visible: true
    title: "System Monitor"
    color: "#2b2b2b"
    width: 1200
    height: 800
    
    property int currentTab: 0
    
    property var processDetailsWindow: null
    
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
        anchors.margins: 10

        SystemOverview {
            Layout.fillWidth: true
            Layout.maximumHeight: 200
        }
        Rectangle {
            Layout.fillWidth: true
            height: 50
            color: "#3c3c3c"
            radius: 8
            border.color: "#505050"
            border.width: 1

            TabBar {
                id: tabBar
                anchors.fill: parent
                anchors.margins: 5
                currentIndex: window.currentTab
                onCurrentIndexChanged: window.currentTab = currentIndex

                background: Rectangle {
                    color: "transparent"
                }

                TabButton {
                    text: "Processes"
                    background: Rectangle {
                        color: parent.checked ? "#505050" : (parent.hovered ? "#454545" : "transparent")
                        radius: 5
                    }
                    contentItem: Text {
                        text: parent.text
                        color: parent.checked ? "#87CEEB" : "#ffffff"
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                TabButton {
                    text: "Memory"
                    background: Rectangle {
                        color: parent.checked ? "#505050" : (parent.hovered ? "#454545" : "transparent")
                        radius: 5
                    }
                    contentItem: Text {
                        text: parent.text
                        color: parent.checked ? "#87CEEB" : "#ffffff"
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                TabButton {
                    text: "CPU"
                    background: Rectangle {
                        color: parent.checked ? "#505050" : (parent.hovered ? "#454545" : "transparent")
                        radius: 5
                    }
                    contentItem: Text {
                        text: parent.text
                        color: parent.checked ? "#87CEEB" : "#ffffff"
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                TabButton {
                    text: "Filesystem"
                    background: Rectangle {
                        color: parent.checked ? "#505050" : (parent.hovered ? "#454545" : "transparent")
                        radius: 5
                    }
                    contentItem: Text {
                        text: parent.text
                        color: parent.checked ? "#87CEEB" : "#ffffff"
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                TabButton {
                    text: "Partitions"
                    background: Rectangle {
                        color: parent.checked ? "#505050" : (parent.hovered ? "#454545" : "transparent")
                        radius: 5
                    }
                    contentItem: Text {
                        text: parent.text
                        color: parent.checked ? "#87CEEB" : "#ffffff"
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: window.currentTab

            Rectangle {
                color: "transparent"

                ProcessTable {
                    anchors.fill: parent
                    
                    onProcessDoubleClicked: function(pid) {
                        processModel.selectedProcessPid = pid
                        openProcessDetails()
                    }
                }
            }

            MemoryUsageGraph {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            CpuUsageChart {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            FilesystemBrowser {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            PartitionOverview {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

    // Função para abrir janela de detalhes do processo
    function openProcessDetails() {
        if (processDetailsWindow) {
            processDetailsWindow.close()
        }

        var component = Qt.createComponent("ProcessDetails.qml")
        if (component.status === Component.Ready) {
            processDetailsWindow = component.createObject(window)
            processDetailsWindow.show()
        } else {
            console.error("Erro ao carregar ProcessDetails.qml:", component.errorString())
        }
    }

    // Limpeza da janela de detalhes quando a aplicação for fechada
    Component.onDestruction: {
        if (processDetailsWindow) {
            processDetailsWindow.close()
        }
    }
}
