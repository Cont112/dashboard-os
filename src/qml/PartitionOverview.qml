import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root
    color: "#2b2b2b"
    border.color: "#404040"
    border.width: 1
    radius: 8

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 15

        Text {
            text: "Partition Information"
            color: "#ffffff"
            font.pixelSize: 18
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            ListView {
                id: partitionListView
                model: processModel.partitions
                spacing: 10

                delegate: Rectangle {
                    width: partitionListView.width
                    height: implicitHeight
                    implicitHeight: partitionContent.implicitHeight + 30
                    color: "#3c3c3c"
                    radius: 8
                    border.color: "#505050"
                    border.width: 1

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: parent.color = "#454545"
                        onExited: parent.color = "#3c3c3c"
                    }

                    ColumnLayout {
                        id: partitionContent
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.margins: 15
                        spacing: 10

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 15

                            RowLayout {
                                spacing: 10

                                Text {
                                    text: "💾"
                                    font.pixelSize: 24
                                }

                                ColumnLayout {
                                    spacing: 2

                                    Text {
                                        text: modelData.device
                                        color: "#87CEEB"
                                        font.pixelSize: 14
                                        font.bold: true
                                    }

                                    Text {
                                        text: modelData.mountpoint
                                        color: "#ffffff"
                                        font.pixelSize: 12
                                    }

                                    Text {
                                        text: modelData.filesystem.toUpperCase()
                                        color: "#999999"
                                        font.pixelSize: 10
                                    }
                                }
                            }

                            Item { Layout.fillWidth: true }

                            ColumnLayout {
                                spacing: 5

                                Text {
                                    text: modelData.usage_percentage.toFixed(1) + "%"
                                    color: getUsageColor(modelData.usage_percentage)
                                    font.pixelSize: 18
                                    font.bold: true
                                    Layout.alignment: Qt.AlignRight
                                }

                                Text {
                                    text: formatBytes(modelData.used_space) + " / " + formatBytes(modelData.total_space)
                                    color: "#cccccc"
                                    font.pixelSize: 11
                                    Layout.alignment: Qt.AlignRight
                                }

                                Text {
                                    text: formatBytes(modelData.available_space) + " free"
                                    color: "#999999"
                                    font.pixelSize: 10
                                    Layout.alignment: Qt.AlignRight
                                }
                            }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            height: 8
                            color: "#1e1e1e"
                            radius: 4
                            border.color: "#505050"
                            border.width: 1

                            Rectangle {
                                width: parent.width * (modelData.usage_percentage / 100)
                                height: parent.height
                                color: getUsageColor(modelData.usage_percentage)
                                radius: 4

                                Behavior on width {
                                    NumberAnimation { duration: 300; easing.type: Easing.OutCubic }
                                }
                            }
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 60
            color: "#3c3c3c"
            radius: 8
            border.color: "#505050"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 20

                ColumnLayout {
                    spacing: 2

                    Text {
                        text: "System Usage"
                        color: "#ffffff"
                        font.pixelSize: 14
                        font.bold: true
                    }

                    Text {
                        text: getTotalUsageText()
                        color: "#cccccc"
                        font.pixelSize: 11
                    }
                }

                Item { Layout.fillWidth: true }
            }
        }
    }

    function getUsageColor(percentage) {
        if (percentage < 60) return "#4CAF50"      
        else if (percentage < 80) return "#FF9800" 
        else return "#F44336"                      
    }

    function formatBytes(bytes) {
        if (bytes === 0) return "0 B"
        const k = 1024
        const sizes = ["B", "KB", "MB", "GB", "TB", "PB"]
        const i = Math.floor(Math.log(bytes) / Math.log(k))
        return parseFloat((bytes / Math.pow(k, i)).toFixed(1)) + " " + sizes[i]
    }

    function getTotalUsageText() {
        let totalSpace = 0
        let totalUsed = 0
        
        for (let i = 0; i < processModel.partitions.length; i++) {
            const partition = processModel.partitions[i]
            if (!partition.filesystem.includes("tmp") && partition.filesystem !== "swap") {
                totalSpace += partition.total_space
                totalUsed += partition.used_space
            }
        }
        
        if (totalSpace > 0) {
            const percentage = (totalUsed / totalSpace * 100).toFixed(1)
            return formatBytes(totalUsed) + " / " + formatBytes(totalSpace) + " (" + percentage + "%)"
        }
        return "N/A"
    }
} 