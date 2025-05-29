#include <QApplication>
#include <QTimer>
#include "modules/model/Model.h"
#include "modules/view/View.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Model model;
    View view(&model);
    QTimer::singleShot(500, &model, &Model::startMonitoring);

    return a.exec();
}
