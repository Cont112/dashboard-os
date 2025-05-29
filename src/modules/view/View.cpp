#include "View.h"
#include <QUrl>
#include <QQmlContext>

View::View(Model* model, QObject *parent)
    : QObject(parent)
{
    //Linka com o modelo de dados
    engine.rootContext()->setContextProperty("processModel", model);
    
    //Carrega o arquivo QML
    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));
}

View::~View()
{
}

