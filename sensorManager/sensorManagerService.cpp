#include <QCoreApplication>

#include "sensorInstantiator.h"
#include "actuatorInstantiator.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    app.setOrganizationName("piHome");
    app.setApplicationName("sensorManager");
    qDebug() << "Sensor Manager";


//    sensorInstantiator::instance().saveSensors();
    QList<wiredSensor> wiredSensorList = sensorInstantiator::instance().getWiredSensors();
    int numSensors = wiredSensorList.count();
    qDebug() << "Wired Sensors: " << numSensors;
    for (int i = 0; i < numSensors; i++)
        wiredSensorList.at(i).debugSensor();

    QList<wirelessSensor> wirelessSensorList = sensorInstantiator::instance().getWirelessSensors();
    numSensors = wirelessSensorList.count();
    qDebug() << "Wireless Sensors: " << numSensors;
    for (int i = 0; i < numSensors; i++)
        wirelessSensorList.at(i).debugSensor();

    qDebug() << "==============================================";
//    actuatorInstantiator::instance().saveActuators();
    QList<actuator> actuatorList = actuatorInstantiator::instance().loadActuators();
    int numActuators = actuatorList.count();
    qDebug() << "Actuators:" << numActuators;
    for (int i = 0; i < numActuators; i++)
        actuatorList.at(i).debugActuator();

    wiredSensor wrdSensor = wiredSensorList.at(1);
    wrdSensor.interrupt();
    wrdSensor.interrupt();
    wrdSensor.interrupt();

    wirelessSensor wrlSensor = wirelessSensorList.at(1);
    wrlSensor.interrupt();
    wrlSensor.interrupt();
    wrlSensor.interrupt();

    int ret = app.exec();

    return ret;
}
