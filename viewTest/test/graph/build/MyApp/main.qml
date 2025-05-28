import QtQuick 6.9
import QtQuick.Controls 6.9
import QtGraphs 6.9

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Dynamic Line Graph"

    GraphsView {
        id: graph
        anchors.fill: parent

        axisX: ValueAxis {
            min: 0
            max: 10
        }

        axisY: ValueAxis {
            min: -1
            max: 1
        }

        LineSeries {
            id: dynamicSeries
            name: "Live Data"
        }

        Connections {
            target: dataProvider
            function onDataChanged() {
                dynamicSeries.clear();
                for (let point of dataProvider.getData()) {
                    dynamicSeries.append(point.x, point.y);
                }
            }
        }
    }
}
