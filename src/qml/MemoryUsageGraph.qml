import QtQuick 6.9
import QtQuick.Controls 6.9
import QtQuick.Layouts 6.9
import QtGraphs 6.9

Rectangle {
    id: root
    width: 600
    height: 400
    color: "#2b2b2b"
    border.color: "#505050"
    border.width: 1
    radius: 8

    property var dataPoints: []
    property int maxDataPoints: 25

    function addMemoryPoint(memoryValue) {
        dataPoints.push(memoryValue);

        if (dataPoints.length > maxDataPoints) {
            dataPoints.shift();
        }

        lineSeries.clear();
        for (var j = 0; j < dataPoints.length; j++) {
            var xPosition = maxDataPoints - dataPoints.length + j;
            lineSeries.append(xPosition, dataPoints[j]);
        }
    }

    Text {
        text: "Memory Usage"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 10
        font.pointSize: 14
        font.bold: true
        color: "#ffffff"
    }

    GraphsView {
        id: graphsView
        anchors.fill: parent
        anchors.margins: 20
        anchors.topMargin: 40

        axisX: ValueAxis {
            id: xAxis
            min: 0
            max: 24
            titleText: ""
            labelsVisible: false
        }

        axisY: ValueAxis {
            id: yAxis
            min: 0
            max: 100
            titleText: "Memory (%)"
        }

        LineSeries {
            id: lineSeries
            name: "Memory Usage"
            color: "#87CEEB"
            width: 3
        }
    }

    Connections {
        target: processModel
        function onSystemStatsChanged() {
            if (processModel.systemMemoryUsage !== undefined) {
                root.addMemoryPoint(processModel.systemMemoryUsage);
            }
        }
    }
}