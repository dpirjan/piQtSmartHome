#include <QDBusConnection>
#include <QDBusError>

#include <unistd.h>

#include "sensorManager.h"

#include "sensorInstantiator.h"
#include "actuatorInstantiator.h"
#include "actuator.h"

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
//    {
//        actTmp->debugActuator();
//        actTmp->setValue("ON");
//    }
//    actTmp = actuatorInstantiator::instance().findActuator(Relay, "SPI_0");
//    if(actTmp)
//    {
//        actTmp->debugActuator();
//        actTmp->setValue("OFF");
//    }
//    actTmp = actuatorInstantiator::instance().findActuator(IR, "SPI_0");
//    if(actTmp)
//    {
//        actTmp->debugActuator();
//        actTmp->setValue("Temp 20");
//    }
//    actTmp = actuatorInstantiator::instance().findActuator(Pump, "SPI_0");
//    if(actTmp)
//    {
//        actTmp->debugActuator();
//        actTmp->setValue("ON");
//    }
//    sleep(2);
//    actTmp = actuatorInstantiator::instance().findActuator(Siren, "SPI_0");
//    if(actTmp)
//    {
//        actTmp->debugActuator();
//        actTmp->setValue("OFF");
//    }
//    actTmp = actuatorInstantiator::instance().findActuator(Relay, "SPI_0");
//    if(actTmp)
//    {
//        actTmp->debugActuator();
//        actTmp->setValue("ON");
//    }
//    actTmp = actuatorInstantiator::instance().findActuator(IR, "SPI_0");
//    if(actTmp)
//    {
//        actTmp->debugActuator();
//        actTmp->setValue("OFF");
//    }
//    actTmp = actuatorInstantiator::instance().findActuator(Pump, "SPI_0");
//    if(actTmp)
//    {
//        actTmp->debugActuator();
//        actTmp->setValue("OFF");
//    }
}


bool SensorManager::connectService()
{
    bool ret = true;

    if(!QDBusConnection::systemBus().registerService(
                SENSOR_MANAGER_SERVICE_NAME))
    {
        qCritical() << QDBusConnection::systemBus().lastError().message();
        ret = false;
    }

    QDBusConnection::systemBus().registerObject(
                SENSOR_MANAGER_SERVICE_PATH,
                this,
                QDBusConnection::ExportScriptableContents);

    if(ret == true)
        qDebug() << "Registered the sensor manager service to DBUS system bus";

    return ret;
}

bool SensorManager::setActuatorValue(const QString &category,
                                     const QString &address,
                                     const QString &value)
{
    bool ret = false;
    actuator *act = actuatorInstantiator::instance().findActuator(StringToType(category), address);
    if(act)
    {
        qDebug() << "setActuatorValue(" << category << ", " << address << ", "
                 << value << ")";
        act->setValue(value);
        ret = true;
    }

    return ret;
}

QString SensorManager::getActuatorValue(const QString &category,
                                     const QString &address)
{
    QString ret;
    actuator *act = actuatorInstantiator::instance().findActuator(StringToType(category), address);
    if(act)
    {
#ifdef WIRINGPI
        ret = act->getValue();
#else
        //@TODO remove this code after testing is finished
        int number = qrand();
        if(number % 2 == 0)
        {
            if(category == "DoorLock")
                ret = "Locked";
            else
                ret = "On";
        }
        else
        {
            if(category == "DoorLock")
                ret = "Unlocked";
            else
                ret = "Off";
        }
#endif
        qDebug() << "getActuatorValue(" << category << ", " << address << ", "
                 << ret << ")";
    }

    return ret;
}

// returns the list of addresses for the actuator type specified
QStringList SensorManager::checkActuator(const QString &category)
{
    QStringList ret;
    QList<actuator *> list = actuatorInstantiator::instance().findActuators(StringToType(category));
    for(int count = 0; count < list.count(); count++)
        ret.append(list.at(count)->getAddress());

    return ret;
}
