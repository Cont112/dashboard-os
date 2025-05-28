#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "back.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // MyBackend backend;
    // engine.rootContext()->setContextProperty("backend", &backend);

    // Change this to ExampleB/Main.qml for the second version
    const QUrl url(QStringLiteral("qrc:/MyApp/test/Main.qml"));
    engine.load(url);

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

