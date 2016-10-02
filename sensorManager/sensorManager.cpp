#include "sensorManager.h"

#include "sensorInstantiator.h"
#include "actuatorInstantiator.h"

SensorManager::SensorManager(QObject *parent) : QObject(parent)
{
}

SensorManager::~SensorManager()
{
}

void SensorManager::init()
{
    m_wiredSensorList = sensorInstantiator::instance().getWiredSensors();
    int numSensors = m_wiredSensorList.count();
    qDebug() << numSensors << " Wired Sensors.";
    //    for(int i = 0; i < numSensors; i++)
    //        m_wiredSensorList.at(i)->debugSensor();

    m_wirelessSensorList = sensorInstantiator::instance().getWirelessSensors();
    numSensors = m_wirelessSensorList.count();
    qDebug() << numSensors << " Wireless Sensors.";
    //    for(int i = 0; i < numSensors; i++)
    //        m_wirelessSensorList.at(i)->debugSensor();

    qDebug() << "==============================================";

    m_actuatorList = actuatorInstantiator::instance().getActuators();
    int numActuators = m_actuatorList.count();
    qDebug() << numActuators << " Actuators.";
    //    for(int i = 0; i < numActuators; i++)
    //        actuatorList.at(i)->debugActuator();
}
