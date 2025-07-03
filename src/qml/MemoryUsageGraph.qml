import QtQuick 6.9
import QtQuick.Controls 6.9
import QtQuick.Layouts 6.9
import QtGraphs 6.9

Rectangle {
    id: root
    width: 600
    height: 400
    color: "#f8f9fa"
    border.color: "#dee2e6"

    property var dataPoints: []
    property int maxDataPoints: 50

    function addMemoryPoint(memoryValue) {
        dataPoints.push({x: dataPoints.length, y: memoryValue});
        
        if (dataPoints.length > maxDataPoints) {
            dataPoints.shift();
            // Reindex points
            for (var i = 0; i < dataPoints.length; i++) {
                dataPoints[i].x = i;
            }
        }
        
        // Clear and rebuild series
        lineSeries.clear();
        for (var j = 0; j < dataPoints.length; j++) {
            lineSeries.append(dataPoints[j].x, dataPoints[j].y);
        }
    }

    GraphsView {
        id: graphsView
        anchors.fill: parent
        anchors.margins: 10
        
        // Configure axes directly on GraphsView
        axisX: ValueAxis {
            id: xAxis
            min: 0
            max: 50
            titleText: "Time"
        }
        
        axisY: ValueAxis {
            id: yAxis
            min: 0
            max: 100
            titleText: "Memory (MB)"
        }
        
        // LineSeries without axis properties
        LineSeries {
            id: lineSeries
            name: "Memory Usage"
            color: "#007bff"
            width: 3
        }
    }
    
    Connections {
        target: processModel
        function onSystemStatsChanged() {
            // Use actual memory usage from your model
            if (processModel.systemMemoryUsage !== undefined) {
                root.addMemoryPoint(processModel.systemMemoryUsage);
            }
        }
    }
}