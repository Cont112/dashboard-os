#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QTimer>
#include "Model.h"
#include "View.h"

class Controller : public QObject {
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();

    void start();
    void stop();

private slots:
    void handleDataUpdate(const QVector<process_t>& processes);
    void handleSystemStatsUpdate(const QMap<QString, QVariant>& stats);
    void handleUserAction(const QString& action, const QVariant& data);

private:
    Model* model;
    View* view;
    QTimer* updateTimer;
};

#endif // CONTROLLER_H
