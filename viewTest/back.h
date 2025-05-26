#pragma once

#include <QObject>
#include <QDebug>

class MyBackend : public QObject {
    Q_OBJECT
public:
    Q_INVOKABLE void sayHello() {
        qDebug() << "Hello from C++!";
    }
};
