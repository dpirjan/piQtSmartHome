#include "sensorManager.h"

#include "sensorInstantiator.h"
#include "actuatorInstantiator.h"

SensorManager::SensorManager(QObject *parent) : QObject(parent)
{
}

SensorManager::~SensorManager()
{
    m_watchdog->stop();
    m_watchdog->deleteLater();
}

void SensorManager::init()
{
    m_watchdog = new WatchdogHelper("piHomeSensor");
    m_watchdog->init();

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

    // @TODO remove this after proper testing
//    wirelessSensor *wrlTmp;
//    wrlTmp = sensorInstantiator::instance().findWirelessSensor(Temperature, "SPI_0");
//    if(wrlTmp)
//        wrlTmp->debugSensor();
//    wrlTmp = sensorInstantiator::instance().findWirelessSensor(Humidity, "SPI_0");
//    if(wrlTmp)
//        wrlTmp->debugSensor();
//    wrlTmp = sensorInstantiator::instance().findWirelessSensor(Light, "SPI_0");
//    if(wrlTmp)
//        wrlTmp->debugSensor();
//    wrlTmp = sensorInstantiator::instance().findWirelessSensor(Contact, "SPI_1");
//    if(wrlTmp)
//        wrlTmp->debugSensor();

//    actuator *actTmp;
//    actTmp = actuatorInstantiator::instance().findActuator(Siren, "SPI_0");
//    if(actTmp)
//        actTmp->debugActuator();
//    actTmp = actuatorInstantiator::instance().findActuator(Relay, "SPI_0");
//    if(actTmp)
//        actTmp->debugActuator();
//    actTmp = actuatorInstantiator::instance().findActuator(IR, "SPI_0");
//    if(actTmp)
//        actTmp->debugActuator();
//    actTmp = actuatorInstantiator::instance().findActuator(Pump, "SPI_0");
//    if(actTmp)
//        actTmp->debugActuator();
}
