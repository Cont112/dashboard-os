import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

// Header with navigation buttons
RowLayout {
    id: navigationBar
    property int currentTab: 0
    signal tabChanged(int tab)  // define o signal

    Layout.fillWidth: true
    Layout.preferredHeight: 50
    spacing: 0

    Button {
        text: "Processes"
        Layout.fillWidth: true
        highlighted: currentTab === 0
        onClicked: {
            navigationBar.currentTab = 0
            navigationBar.tabChanged(0)  // emit the signal when the button is clicked
        } 
    }
    Button {
        text: "Performance"
        Layout.fillWidth: true
        highlighted: currentTab === 1
        onClicked: {
            navigationBar.currentTab = 1
            navigationBar.tabChanged(1)  // emit the signal when the button is clicked
        } 
    }
    Button {
        text: "Storage"
        Layout.fillWidth: true
        highlighted: currentTab === 2
        onClicked: {
            navigationBar.currentTab = 2
            navigationBar.tabChanged(2)  // emit the signal when the button is clicked
        } 
    }
}