#include "sensor.h"
#include "wirelessSensor.h"
#include "homeAlarmInfo.h"
#include "smartHomeInfo.h"
#include "databaseManagerInterface.h"

wirelessSensor::wirelessSensor(const SystemType &system,
                               const SensorType &type,
                               const QString &zone,
                               const QString &node,
                               const QString &address,
                               const int &timeout)
    : sensor(system, type, zone, node, address)
    , m_wirelessTimeout(timeout)

{
}

wirelessSensor::wirelessSensor(const wirelessSensor &obj) : sensor(obj)
{
    m_wirelessTimeout = obj.getWirelessTimeout();
}

wirelessSensor& wirelessSensor::operator =(const wirelessSensor &other)
{
    sensor::operator = (other);
    m_wirelessTimeout = other.getWirelessTimeout();

    return *this;
}

wirelessSensor::~wirelessSensor()
{
}

int wirelessSensor::getWirelessTimeout() const
{
    return m_wirelessTimeout;
}

void wirelessSensor::debugSensor() const
{
    qDebug() << "==============================================";
    sensor::debugSensor();
    qDebug() << "Wireless timeout " << m_wirelessTimeout;
}

void wirelessSensor::setWirelessTimeout(const int &timeout)
{
    m_wirelessTimeout = timeout;
}

void wirelessSensor::interrupt()
{
    QElapsedTimer timer;
    timer.start();
    SensorType type = getSensorType();
    if(type == Sensor_Light || type == Sensor_Temperature)
    {
        // Will create a smartHomeInfo class instance and insert it in the
        // database containing the events.
        SmartHomeInfo event(getZone(),getNode(),sensorTypeToString(getSensorType()), getValue());
        databaseManagerInterface::instance().insertSmartHomeEntry(event);
    }
    else {
        // Will create a homeAlarmInfo class instance and insert it in the
        // database containing the events.
        HomeAlarmInfo event(getZone(),getNode(),sensorTypeToString(getSensorType()));
        databaseManagerInterface::instance().insertHomeAlarmEntry(event);
    }
    qDebug() << "wirelessInterrupt() took " << timer.elapsed() << "ms";
}