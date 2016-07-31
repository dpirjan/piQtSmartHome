#include <QCoreApplication>

#include "sensorInstantiator.h"
#include "actuatorInstantiator.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    app.setOrganizationName("piHome");
    app.setApplicationName("sensorManager");
    qDebug() << "Sensor Manager";

    sensorInstantiator sensorInst;
//    sensorInst.saveSensors();
    QList<sensor> sensorList = sensorInst.loadSensors();
    int numSensors = sensorList.count();
    qDebug() << "Sensors:" << numSensors;
    for (int i = 0; i < numSensors; i++)
        sensorList.at(i).debugSensor();

    qDebug() << "==============================================";
    actuatorInstantiator actuatorInst;
//    actuatorInst.saveActuators();
    QList<actuator> actuatorList = actuatorInst.loadActuators();
    int numActuators = actuatorList.count();
    qDebug() << "Actuators:" << numActuators;
    for (int i = 0; i < numActuators; i++)
        actuatorList.at(i).debugActuator();

    sensor wiredSensor = sensorList.at(1);
    wiredSensor.wiredInterrupt();
    wiredSensor.wiredInterrupt();
    wiredSensor.wiredInterrupt();

    int ret = app.exec();

    return ret;
}
