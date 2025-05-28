import QtQuick 6.9
import QtQuick.Controls 6.9
import QtQuick.Layouts 6.9
import Qt.labs.qmlmodels

//Falta importar o modelo de processos

Item{
    ScrollView {
        anchors.fill: parent
        clip: true
        TableView {
            anchors.fill: parent
            columnSpacing: 1
            rowSpacing: 1
            clip: true
            // substituir o model por um modelo real de processos
            model: ProcessTableModel{}

            delegate: Rectangle {
                implicitHeight: 40
                implicitWidth: 300

                Text {
                    text: display
                }
            }

        }
    }
}
