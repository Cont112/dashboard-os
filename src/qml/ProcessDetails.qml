import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: detailsWindow
    width: 800
    height: 800
    title: "Detalhes do Processo - PID: " + processModel.selectedProcessPid
    color: "#2b2b2b"

    property var processIODetails: ({})

    Component.onCompleted: {
        updateProcessDetails()
    }

    Connections {
        target: processModel
        function onSelectedProcessChanged() {
            updateProcessDetails()
        }
    }

    function updateProcessDetails() {
        processIODetails = processModel.getProcessIODetails(processModel.selectedProcessPid)
    }

    ScrollView {
        anchors.fill: parent
        anchors.margins: 20
        clip: true

        ColumnLayout {
            width: detailsWindow.width - 40
            spacing: 20

            Rectangle {
                Layout.fillWidth: true
                height: 80
                color: "#3c3c3c"
                radius: 8
                border.color: "#505050"
                border.width: 1

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 20

                    ColumnLayout {
                        spacing: 5

                        Text {
                            text: processIODetails.name || "Unknown Process"
                            color: "#87CEEB"
                            font.pixelSize: 18
                            font.bold: true
                        }

                        Text {
                            text: "PID: " + processModel.selectedProcessPid + " | User: " + (processIODetails.username || "N/A")
                            color: "#cccccc"
                            font.pixelSize: 12
                        }

                        Text {
                            text: "State: " + (processIODetails.state || "N/A") + " | Threads: " + (processIODetails.thread_count || 0)
                            color: "#999999"
                            font.pixelSize: 11
                        }
                    }

                    Item { Layout.fillWidth: true }

                    Button {
                        text: "✕"
                        onClicked: detailsWindow.close()
                        background: Rectangle {
                            color: parent.hovered ? "#e74c3c" : "#505050"
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
                        width: 30
                        height: 30
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 150
                color: "#3c3c3c"
                radius: 8
                border.color: "#505050"
                border.width: 1

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 10

                    Text {
                        text: "IO Statistics"
                        color: "#ffffff"
                        font.pixelSize: 16
                        font.bold: true
                    }

                    GridLayout {
                        columns: 2
                        columnSpacing: 30
                        rowSpacing: 10

                        RowLayout {
                            Text { text: "↓"; font.pixelSize: 16 }
                            Text {
                                text: "Read Bytes:"
                                color: "#cccccc"
                                font.pixelSize: 12
                            }
                            Text {
                                text: formatBytes(processIODetails.read_bytes || 0)
                                color: "#4CAF50"
                                font.pixelSize: 12
                                font.bold: true
                            }
                        }

                        RowLayout {
                            Text { text: "↑"; font.pixelSize: 16 }
                            Text {
                                text: "Written Bytes:"
                                color: "#cccccc"
                                font.pixelSize: 12
                            }
                            Text {
                                text: formatBytes(processIODetails.write_bytes || 0)
                                color: "#FF9800"
                                font.pixelSize: 12
                                font.bold: true
                            }
                        }
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: 500
                color: "#3c3c3c"
                radius: 8
                border.color: "#505050"
                border.width: 1

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 10

                    Text {
                        text: "Open Files (" + (processIODetails.open_files ? processIODetails.open_files.length : 0) + ")"
                        color: "#ffffff"
                        font.pixelSize: 16
                        font.bold: true
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#1e1e1e"
                        radius: 5

                        ListView {
                            id: openFilesView
                            anchors.fill: parent
                            anchors.margins: 10
                            model: processIODetails.open_files || []
                            clip: true

                            delegate: Rectangle {
                                width: openFilesView.width
                                height: 50
                                color: mouseArea.containsMouse ? "#404040" : "transparent"
                                radius: 5

                                MouseArea {
                                    id: mouseArea
                                    anchors.fill: parent
                                    hoverEnabled: true
                                }

                                RowLayout {
                                    anchors.fill: parent
                                    anchors.margins: 10
                                    spacing: 15

                                    ColumnLayout {
                                        spacing: 2
                                        Layout.fillWidth: true

                                        Text {
                                            text: modelData.path
                                            color: "#ffffff"
                                            font.family: "Courier"
                                            font.pixelSize: 11
                                            elide: Text.ElideMiddle
                                            Layout.fillWidth: true
                                        }

                                        RowLayout {
                                            Text {
                                                text: "FD: " + modelData.fd
                                                color: "#87CEEB"
                                                font.pixelSize: 9
                                            }

                                            Text {
                                                text: "Type: " + modelData.type
                                                color: "#999999"
                                                font.pixelSize: 9
                                            }

                                            Text {
                                                text: modelData.size > 0 ? "Size: " + formatBytes(modelData.size) : ""
                                                color: "#777777"
                                                font.pixelSize: 9
                                            }
                                        }
                                    }
                                }
                            }

                            ScrollBar.vertical: ScrollBar {
                                active: true
                                policy: ScrollBar.AsNeeded
                            }
                        }
                    }
                }
            }

            Button {
                text: "Refresh"
                Layout.alignment: Qt.AlignHCenter
                onClicked: {
                    updateProcessDetails()
                }
                background: Rectangle {
                    color: parent.hovered ? "#505050" : "#3c3c3c"
                    radius: 5
                    border.color: "#505050"
                    border.width: 1
                }
                contentItem: Text {
                    text: parent.text
                    color: "#ffffff"
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }

    function formatBytes(bytes) {
        if (bytes === 0) return "0 B"
        const k = 1024
        const sizes = ["B", "KB", "MB", "GB", "TB"]
        const i = Math.floor(Math.log(bytes) / Math.log(k))
        return parseFloat((bytes / Math.pow(k, i)).toFixed(1)) + " " + sizes[i]
    }
} 