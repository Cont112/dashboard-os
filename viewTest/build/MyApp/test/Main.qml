import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
// Uncomment for backend integration
// import MyApp 1.0

ApplicationWindow {
    id: window
    visible: true
    title: "System Monitor"

    property int currentTab: 0 // 0: Processes, 1: Performance, 2: Storage
    property var processData: [] // Replace with back.getProcessData()
    property var performanceData: [] // Replace with back.getPerformanceData()
    property var storageData: [] // Replace with back.getStorageData()

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
                modelData: processData
            }

            PerformanceGraphs {
                Layout.fillWidth: true
                Layout.fillHeight: true
                performanceModel: performanceData
            }

            StorageList {
                Layout.fillWidth: true
                Layout.fillHeight: true
                storageModel: storageData
            }

        }
    }
}