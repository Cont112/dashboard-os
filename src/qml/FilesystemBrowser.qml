import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root
    color: "#2b2b2b"
    border.color: "#404040"
    border.width: 1
    radius: 8

    property alias currentDirectory: pathLabel.text

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Rectangle {
            Layout.fillWidth: true
            height: 40
            color: "#3c3c3c"
            radius: 5

            RowLayout {
                anchors.fill: parent
                anchors.margins: 5
                spacing: 10

                Button {
                    text: "Home"
                    onClicked: processModel.navigateToHome()
                    background: Rectangle {
                        color: parent.hovered ? "#505050" : "#404040"
                        radius: 3
                    }
                }

                Button {
                    text: "Root"
                    onClicked: processModel.navigateToRoot()
                    background: Rectangle {
                        color: parent.hovered ? "#505050" : "#404040"
                        radius: 3
                    }
                }

                Button {
                    text: "↑"
                    onClicked: processModel.navigateToParent()
                    background: Rectangle {
                        color: parent.hovered ? "#505050" : "#404040"
                        radius: 3
                    }
                }

                Rectangle {
                    width: 1
                    Layout.fillHeight: true
                    color: "#505050"
                }

                Text {
                    id: pathLabel
                    text: processModel.currentDirectory
                    color: "#ffffff"
                    font.family: "Courier"
                    font.pixelSize: 12
                    Layout.fillWidth: true
                    elide: Text.ElideLeft
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#1e1e1e"
            radius: 5

            ListView {
                id: fileListView
                anchors.fill: parent
                anchors.margins: 5
                model: processModel.directoryContents
                clip: true

                delegate: Rectangle {
                    width: fileListView.width
                    height: 35
                    color: mouseArea.containsMouse ? "#404040" : "transparent"
                    radius: 3

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onDoubleClicked: {
                            if (modelData.is_directory) {
                                processModel.navigateToDirectory(modelData.path)
                            }
                        }
                    }

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 5
                        spacing: 10

                        Text {
                            text: {
                                if (modelData.is_directory) return "📁"
                                else if (modelData.name.endsWith(".txt")) return "📄"
                                else if (modelData.name.endsWith(".exe")) return "⚙️"
                                else return "📄"
                            }
                            font.pixelSize: 16
                        }

                        Text {
                            text: modelData.name
                            color: modelData.is_directory ? "#87CEEB" : "#ffffff"
                            font.family: "Courier"
                            font.pixelSize: 11
                            Layout.fillWidth: true
                            elide: Text.ElideRight
                        }

                        Text {
                            text: modelData.owner
                            color: "#999999"
                            font.family: "Courier"
                            font.pixelSize: 10
                        }

                        Text {
                            text: modelData.is_directory ? "" : formatFileSize(modelData.size)
                            color: "#999999"
                            font.family: "Courier"
                            font.pixelSize: 10
                            Layout.preferredWidth: 80
                            horizontalAlignment: Text.AlignRight
                        }

                        Text {
                            text: Qt.formatDateTime(modelData.modified_time, "dd/MM/yy hh:mm")
                            color: "#999999"
                            font.family: "Courier"
                            font.pixelSize: 10
                            Layout.preferredWidth: 100
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

    function formatFileSize(bytes) {
        if (bytes === 0) return "0 B"
        const k = 1024
        const sizes = ["B", "KB", "MB", "GB", "TB"]
        const i = Math.floor(Math.log(bytes) / Math.log(k))
        return parseFloat((bytes / Math.pow(k, i)).toFixed(1)) + " " + sizes[i]
    }
} 