import QtQuick 6.9
import QtQuick.Controls 6.9
import QtQuick.Layouts 6.9
import QtGraphs 6.9

Item{
    property var ranges: ({ minX, maxX, minY, maxY })
    property var data: []

    GraphsView {
        anchors.fill: parent

        ValueAxis {
            orientation: Qt.Horizontal
            min: root.ranges.minX
            max: root.ranges.maxX
        }

        ValueAxis {
            orientation: Qt.Vertical
            min: root.ranges.minY
            max: root.ranges.maxY
        }

        LineSeries {
            model: root.data

            xValue: model.x
            yValue: model.y
        }
    }
}
