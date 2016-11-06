#include <QElapsedTimer>

#include "sensor.h"
#include "wirelessSensor.h"
#include "homeAlarmInfo.h"
#include "smartHomeInfo.h"
#include "databaseManagerInterface.h"
#include "mailManagerInterface.h"
#include "RF24Interface.h"

wirelessSensor::wirelessSensor(const SystemType &system,
                               const SensorType &type,
                               const QString &zone,
                               const QString &node,
                               const QString &address,
                               const bool &sendMail,
                               const int &timeout)
    : sensor(system, type, zone, node, address, sendMail)
    , m_wirelessTimeout(timeout)
{
    qDebug() << "wirelessSensor ctor: " << QThread::currentThreadId();
    RF24Interface::instance().init();
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
    if(type == Light || type == Temperature)
    {
        // Will create a smartHomeInfo class instance and insert it in the
        // database containing the events.
        SmartHomeInfo event(getZone(),
                            getNode(),
                            sensorTypeToString(getSensorType()),
                            getAddress(),
                            getValue());
        databaseManagerInterface::instance().insertSmartHomeEntry(event);
    }
    else
    {
        // Will create a homeAlarmInfo class instance and insert it in the
        // database containing the events.
        HomeAlarmInfo event(getZone(),
                            getNode(),
                            sensorTypeToString(getSensorType()),
                            getAddress());
        databaseManagerInterface::instance().insertHomeAlarmEntry(event);
        if(getSendMail())
        {
            QString subject = "Alarm Notification piHome";
            QString message = "Alarm from Zone: " + getZone() +
                    " Node: " + getNode() +
                    " Sensor: " + sensorTypeToString(getSensorType());
            mailManagerInterface::instance().sendMail(subject,message);
        }
    }
    qDebug() << "wirelessInterrupt() took " << timer.elapsed() << "ms";
}
