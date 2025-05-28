#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>
#include <QVector>
#include <QPointF>

class DataProvider : public QObject {
    Q_OBJECT
public:
    QVector<QPointF> data;
    Q_INVOKABLE QVariantList getData() {
        QVariantList list;
        for (const QPointF &pt : data)
            list << QVariant::fromValue(pt);
        return list;
    }

    void generateData() {
        data.clear();
        for (int i = 0; i <= 100; ++i) {
            qreal x = i / 10.0;
            qreal y = qSin(x);  // or any dynamic value
            data.append(QPointF(x, y));
        }
        emit dataChanged();
    }

signals:
    void dataChanged();
};

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    DataProvider provider;
    engine.rootContext()->setContextProperty("dataProvider", &provider);

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&provider]() {
        provider.generateData();
    });
    timer.start(1000);  // update every second

    engine.load(QUrl::fromLocalFile("/home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/test/graph/main.qml"));

    return app.exec();
}

#include "main.moc"
