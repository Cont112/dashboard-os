import QtQuick 6.9
import QtQuick.Controls 6.9
import QtQuick.Layouts 6.9
import QtCharts 6.9
//A ideia da pagina é essa mesma, os 3 componentes não tão funcionando ainda mas a estrutura ta pronta
ApplicationWindow {
    id: window
    visible: true
    title: "System Monitor"

    property int currentTab: 0
    
    ColumnLayout {
        spacing: 0
        anchors.fill: parent

        NavigationBar {
            Layout.fillWidth: true
            currentTab: window.currentTab
            onTabChanged: tab => window.currentTab = tab
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: window.currentTab

            ProcessTable {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            PerformanceGraphs {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            StorageTable {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

        }
    }
}
