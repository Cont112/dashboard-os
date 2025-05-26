import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    ColumnLayout {
        spacing: 10

        Text { text: "CPU Usage (%)"; font.bold: true }
        Canvas {
            id: cpuGraph
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height / 2 - 20
            property var data: performanceModel // Replace with back.getPerformanceData()

            onPaint: {
                var ctx = getContext("2d");
                ctx.clearRect(0, 0, width, height);
                ctx.strokeStyle = "blue";
                ctx.lineWidth = 2;
                ctx.beginPath();

                var maxValue = 100;
                var stepX = width / Math.max(data.count - 1, 1);
                var stepY = height / maxValue;

                for (var i = 0; i < data.count; i++) {
                    var x = i * stepX;
                    var y = height - (data.get(i).cpu * stepY);
                    if (i === 0) {
                        ctx.moveTo(x, y);
                    } else {
                        ctx.lineTo(x, y);
                    }
                }
                ctx.stroke();

                ctx.strokeStyle = "black";
                ctx.lineWidth =  1;
                ctx.beginPath();
                ctx.moveTo(0, height);
                ctx.lineTo(width, height);
                ctx.moveTo(0, 0);
                ctx.lineTo(0, height);
                ctx.stroke();
            }

            Component.onCompleted: requestPaint()
            // Connections {
            //     target: back
            //     function onPerformanceDataChanged() { cpuGraph.requestPaint() }
            // }
        }

        Text { text: "Memory Usage (%)"; font.bold: true }
        Canvas {
            id: memoryGraph
            Layout.fillWidth: true
            Layout.fillHeight: true
            property var data: performanceModel // Replace with back.getPerformanceData()

            onPaint: {
                var ctx = getContext("2d");
                ctx.clearRect(0, 0, width, height);
                ctx.strokeStyle = "green";
                ctx.lineWidth = 2;
                ctx.beginPath();

                var maxValue = 100;
                var stepX = width / Math.max(data.count - 1, 1);
                var stepY = height / maxValue;

                for (var i = 0; i < data.count; i++) {
                    var x = i * stepX;
                    var y = height - (data.get(i).memory * stepY);
                    if (i === 0) {
                        ctx.moveTo(x, y);
                    } else {
                        ctx.lineTo(x, y);
                    }
                }
                ctx.stroke();

                ctx.strokeStyle = "black";
                ctx.lineWidth = 1;
                ctx.beginPath();
                ctx.moveTo(0, height);
                ctx.lineTo(width, height);
                ctx.moveTo(0, 0);
                ctx.lineTo(0, height);
                ctx.stroke();
            }

            Component.onCompleted: requestPaint()
            // Connections {
            //     target: back
            //     function onPerformanceDataChanged() { memoryGraph.requestPaint() }
            // }
        }
    }
}
