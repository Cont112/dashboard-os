import QtQuick 2.15
import QtQuick.Controls 2.15
import QtCharts 2.15

ApplicationWindow {
    visible: true
    width: 400
    height: 300
    title: "QtCharts test"

    ChartView {
        anchors.fill: parent
        ScatterSeries {
            id: scatterSeries
            Component.onCompleted: {
                append(0,0);
                append(1,1);
            }
        }
    }
}
