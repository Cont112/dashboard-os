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

        // Título
        Text {
            text: "Informações das Partições"
            color: "#ffffff"
            font.pixelSize: 18
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        // Lista de partições
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
                    height: partitionContent.height + 20
                    color: "#3c3c3c"
                    radius: 8
                    border.color: "#505050"
                    border.width: 1

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: parent.color = "#454545"
                        onExited: parent.color = "#3c3c3c"
                        onClicked: processModel.refreshPartitionInfo(modelData.device)
                    }

                    ColumnLayout {
                        id: partitionContent
                        anchors.fill: parent
                        anchors.margins: 15
                        spacing: 10

                        // Cabeçalho da partição
                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 15

                            // Ícone e dispositivo
                            RowLayout {
                                spacing: 10

                                Text {
                                    text: getPartitionIcon(modelData.filesystem)
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

                            // Informações de uso
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
                                    text: formatBytes(modelData.available_space) + " livres"
                                    color: "#999999"
                                    font.pixelSize: 10
                                    Layout.alignment: Qt.AlignRight
                                }
                            }
                        }

                        // Barra de progresso
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

                        // Opções de montagem (se disponíveis)
                        Text {
                            text: "Opções: " + modelData.options
                            color: "#777777"
                            font.pixelSize: 9
                            font.family: "Courier"
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                            visible: modelData.options.length > 0
                        }
                    }

                    // Botão de atualizar
                    Button {
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.margins: 10
                        width: 30
                        height: 30
                        text: "🔄"
                        font.pixelSize: 12

                        background: Rectangle {
                            color: parent.hovered ? "#505050" : "#404040"
                            radius: 15
                            border.color: "#606060"
                            border.width: 1
                        }

                        onClicked: processModel.refreshPartitionInfo(modelData.device)

                        ToolTip.text: "Atualizar informações da partição"
                        ToolTip.visible: hovered
                    }
                }
            }
        }

        // Resumo total
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

                Text {
                    text: "💾"
                    font.pixelSize: 24
                }

                ColumnLayout {
                    spacing: 2

                    Text {
                        text: "Total do Sistema"
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

    function getPartitionIcon(filesystem) {
        switch(filesystem.toLowerCase()) {
            case "ext4":
            case "ext3":
            case "ext2":
                return "🐧"
            case "ntfs":
            case "fat32":
            case "fat16":
                return "🪟"
            case "xfs":
            case "btrfs":
            case "zfs":
                return "💿"
            case "tmpfs":
            case "devtmpfs":
                return "⚡"
            case "swap":
                return "🔄"
            default:
                return "💾"
        }
    }

    function getUsageColor(percentage) {
        if (percentage < 60) return "#4CAF50"      // Verde
        else if (percentage < 80) return "#FF9800" // Laranja
        else return "#F44336"                      // Vermelho
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
            // Considera apenas partições principais (não temporárias)
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