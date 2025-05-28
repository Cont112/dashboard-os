import QtQuick 6.9
import QtQuick.Controls 6.9
import QtQuick.Layouts 6.9
//Teoricamente falta o importe certo do modelo
    
Item {
    TableView {
        anchors.fill: parent
        columnSpacing: 1
        rowSpacing: 1
        clip: true
        // substituir o model por um modelo real de processos
        model: StorageTableModel{}

        delegate: Rectangle {
            implicitHeight: 40
            implicitWidth: 300

            Text {
                text: display
            }
        }

    }
}
