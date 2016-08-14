#include "wiredSensor.h"
#include "homeAlarmInfo.h"
#include "databaseManagerInterface.h"

wiredSensor::wiredSensor(const SystemType &system,
                         const SensorType &type,
                         const QString &zone,
                         const QString &node,
                         const QString &address,
                         const QString &edge,
                         const int &timeout)
    : sensor(system, type, zone, node, address)
    , m_wiredEdge(edge)
    , m_wiredTimeout(timeout)
{
    qDebug() << "wiredSensor ctor: " << this;
}

wiredSensor::wiredSensor(const wiredSensor &obj) : sensor(obj)
{
    m_wiredEdge = obj.getWiredEdge();
    m_wiredTimeout = obj.getWiredTimeout();
}

wiredSensor& wiredSensor::operator =(const wiredSensor &other)
{
    sensor::operator = (other);
    m_wiredEdge = other.getWiredEdge();
    m_wiredTimeout = other.getWiredTimeout();

    return *this;
}

wiredSensor::~wiredSensor()
{
}

QString wiredSensor::getWiredEdge() const
{
    return m_wiredEdge;
}

int wiredSensor::getWiredTimeout() const
{
    return m_wiredTimeout;
}

void wiredSensor::debugSensor() const
{
    qDebug() << "==============================================";
    sensor::debugSensor();
    qDebug() << "Wired Edge " << m_wiredEdge;
    qDebug() << "Wired timeout " << m_wiredTimeout;
}

void wiredSensor::setWiredEdge(const QString &edge)
{
    m_wiredEdge = edge;
}

void wiredSensor::setWiredTimeout(const int &timeout)
{
    m_wiredTimeout = timeout;
}

void wiredSensor::interruptHandler(void *userData)
{
    wiredSensor *sensor = reinterpret_cast<wiredSensor *>(userData);
    qDebug() << "interruptHandler: " << sensor;
    sensor->interrupt();
}

void wiredSensor::interrupt()
{
    QElapsedTimer timer;
    timer.start();
    // Will create a homeAlarmInfo class instance and insert it in the
    // database containing the events.
    HomeAlarmInfo event(getZone(),getNode(),sensorTypeToString(getSensorType()));
    databaseManagerInterface::instance().insertHomeAlarmEntry(event);
    qDebug() << "wiredInterrupt() took " << timer.elapsed() << "ms";
}
