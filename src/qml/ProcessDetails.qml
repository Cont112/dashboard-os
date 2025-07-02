import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: detailsWindow
    width: 800
    height: 600
    title: "Detalhes do Processo - PID: " + processModel.selectedProcessPid
    color: "#2b2b2b"

    property var processIODetails: processModel.getProcessIODetails(processModel.selectedProcessPid)

    Component.onCompleted: {
        // Atualiza os detalhes quando a janela é aberta
        processIODetails = Qt.binding(function() {
            return processModel.getProcessIODetails(processModel.selectedProcessPid)
        })
    }

    ScrollView {
        anchors.fill: parent
        anchors.margins: 20
        clip: true

        ColumnLayout {
            width: detailsWindow.width - 40
            spacing: 20

            // Cabeçalho
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

                    Text {
                        text: "⚙️"
                        font.pixelSize: 32
                    }

                    ColumnLayout {
                        spacing: 5

                        Text {
                            text: processIODetails.name || "Processo Desconhecido"
                            color: "#87CEEB"
                            font.pixelSize: 18
                            font.bold: true
                        }

                        Text {
                            text: "PID: " + processModel.selectedProcessPid + " | Usuário: " + (processIODetails.username || "N/A")
                            color: "#cccccc"
                            font.pixelSize: 12
                        }

                        Text {
                            text: "Estado: " + (processIODetails.state || "N/A") + " | Threads: " + (processIODetails.thread_count || 0)
                            color: "#999999"
                            font.pixelSize: 11
                        }
                    }

                    Item { Layout.fillWidth: true }

                    Button {
                        text: "✖"
                        onClicked: detailsWindow.close()
                        background: Rectangle {
                            color: parent.hovered ? "#F44336" : "#505050"
                            radius: 15
                        }
                        width: 30
                        height: 30
                    }
                }
            }

            // Estatísticas de E/S
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
                        text: "📊 Estatísticas de E/S"
                        color: "#ffffff"
                        font.pixelSize: 16
                        font.bold: true
                    }

                    GridLayout {
                        columns: 2
                        columnSpacing: 30
                        rowSpacing: 10

                        // Bytes lidos
                        RowLayout {
                            Text { text: "📥"; font.pixelSize: 16 }
                            Text {
                                text: "Bytes Lidos:"
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

                        // Bytes escritos
                        RowLayout {
                            Text { text: "📤"; font.pixelSize: 16 }
                            Text {
                                text: "Bytes Escritos:"
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

                        // Syscalls de leitura
                        RowLayout {
                            Text { text: "🔄"; font.pixelSize: 16 }
                            Text {
                                text: "Syscalls de Leitura:"
                                color: "#cccccc"
                                font.pixelSize: 12
                            }
                            Text {
                                text: (processIODetails.read_syscalls || 0).toLocaleString()
                                color: "#87CEEB"
                                font.pixelSize: 12
                                font.bold: true
                            }
                        }

                        // Syscalls de escrita
                        RowLayout {
                            Text { text: "✏️"; font.pixelSize: 16 }
                            Text {
                                text: "Syscalls de Escrita:"
                                color: "#cccccc"
                                font.pixelSize: 12
                            }
                            Text {
                                text: (processIODetails.write_syscalls || 0).toLocaleString()
                                color: "#E91E63"
                                font.pixelSize: 12
                                font.bold: true
                            }
                        }
                    }
                }
            }

            // Arquivos Abertos
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: 200
                color: "#3c3c3c"
                radius: 8
                border.color: "#505050"
                border.width: 1

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 10

                    Text {
                        text: "📁 Arquivos Abertos (" + (processIODetails.open_files ? processIODetails.open_files.length : 0) + ")"
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

                                    // Ícone do tipo de arquivo
                                    Text {
                                        text: getFileIcon(modelData.type)
                                        font.pixelSize: 20
                                    }

                                    // Informações do arquivo
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
                                                text: "Tipo: " + modelData.type
                                                color: "#999999"
                                                font.pixelSize: 9
                                            }

                                            Text {
                                                text: "Perms: " + modelData.permissions
                                                color: "#999999"
                                                font.pixelSize: 9
                                            }

                                            Text {
                                                text: modelData.size > 0 ? "Tamanho: " + formatBytes(modelData.size) : ""
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

            // Recursos IPC
            Rectangle {
                Layout.fillWidth: true
                Layout.minimumHeight: 150
                color: "#3c3c3c"
                radius: 8
                border.color: "#505050"
                border.width: 1

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 10

                    Text {
                        text: "🔗 Recursos IPC (" + (processIODetails.ipc_resources ? processIODetails.ipc_resources.length : 0) + ")"
                        color: "#ffffff"
                        font.pixelSize: 16
                        font.bold: true
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.minimumHeight: 100
                        color: "#1e1e1e"
                        radius: 5

                        ListView {
                            id: ipcResourcesView
                            anchors.fill: parent
                            anchors.margins: 10
                            model: processIODetails.ipc_resources || []
                            clip: true

                            delegate: Rectangle {
                                width: ipcResourcesView.width
                                height: 40
                                color: mouseArea2.containsMouse ? "#404040" : "transparent"
                                radius: 3

                                MouseArea {
                                    id: mouseArea2
                                    anchors.fill: parent
                                    hoverEnabled: true
                                }

                                RowLayout {
                                    anchors.fill: parent
                                    anchors.margins: 10
                                    spacing: 15

                                    Text {
                                        text: getIPCIcon(modelData.type)
                                        font.pixelSize: 16
                                    }

                                    ColumnLayout {
                                        spacing: 2
                                        Layout.fillWidth: true

                                        Text {
                                            text: modelData.type + " - " + modelData.key
                                            color: "#ffffff"
                                            font.pixelSize: 11
                                        }

                                        Text {
                                            text: "Permissões: " + modelData.permissions + " | Proprietário: " + modelData.owner_pid
                                            color: "#999999"
                                            font.pixelSize: 9
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // Botão de atualizar
            Button {
                text: "🔄 Atualizar Informações"
                Layout.alignment: Qt.AlignHCenter
                onClicked: {
                    processIODetails = processModel.getProcessIODetails(processModel.selectedProcessPid)
                }
                background: Rectangle {
                    color: parent.hovered ? "#505050" : "#404040"
                    radius: 5
                    border.color: "#606060"
                    border.width: 1
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

    function getFileIcon(type) {
        switch(type.toLowerCase()) {
            case "reg": return "📄"
            case "dir": return "📁"
            case "sock": return "🔌"
            case "pipe": return "🔗"
            case "chr": return "⚙️"
            case "blk": return "💿"
            case "lnk": return "🔗"
            case "fifo": return "📡"
            default: return "❓"
        }
    }

    function getIPCIcon(type) {
        switch(type.toLowerCase()) {
            case "semáforo": return "🚦"
            case "mutex": return "🔒"
            case "shared_memory": return "📝"
            case "message_queue": return "📨"
            default: return "��"
        }
    }
} 