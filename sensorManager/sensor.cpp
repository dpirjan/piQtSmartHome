#include "sensor.h"
#include "homeAlarmInfo.h"
#include "databaseManagerInterface.h"

sensor::sensor(const SystemType &system,
               const SensorType &type,
               const HardwareType &hw,
               const QString &zone,
               const QString &node,
               const QString &address,
               const QString &value,
               const bool &interrupt)
    : m_system(system)
    , m_type(type)
    , m_hard(hw)
    , m_zone(zone)
    , m_node(node)
    , m_address(address)
    , m_value(value)
    , m_supportInterrupt(interrupt)
{
    m_wiredEdge = QString();
    m_wiredTimeout = -1;
}

sensor::sensor(const sensor &obj)
{
    m_system = obj.getSystemType();
    m_type = obj.getSensorType();
    m_hard = obj.getHardwareType();
    m_zone = obj.getZone();
    m_node = obj.getNode();
    m_address = obj.getAddress();
    m_value = obj.getValue();
    m_supportInterrupt = obj.getSupportInterrupt();
    m_wiredEdge = obj.getWiredEdge();
    m_wiredTimeout = obj.getWiredTimeout();
}

sensor& sensor::operator=(const sensor &other)
{
    m_system = other.getSystemType();
    m_type = other.getSensorType();
    m_hard = other.getHardwareType();
    m_zone = other.getZone();
    m_node = other.getNode();
    m_address = other.getAddress();
    m_value = other.getValue();
    m_supportInterrupt = other.getSupportInterrupt();
    m_wiredEdge = other.getWiredEdge();
    m_wiredTimeout = other.getWiredTimeout();

    return *this;
}

sensor::~sensor()
{
}

void sensor::debugSensor() const
{
    qDebug() << "==============================================";
    qDebug() << systemTypeToString(m_system);
    qDebug() << sensorTypeToString(m_type);
    qDebug() << hardwareTypeToString(m_hard);
    qDebug() << m_zone;
    qDebug() << m_node;
    qDebug() << m_address;
    qDebug() << m_value;
    qDebug() << m_supportInterrupt;
    if (m_hard == Wired)
    {
        qDebug() << "Wired Edge " << m_wiredEdge;
        qDebug() << "Wired timeout " << m_wiredTimeout;
    }
}

SystemType sensor::getSystemType() const
{
    return m_system;
}

SensorType sensor::getSensorType() const
{
    return m_type;
}

HardwareType sensor::getHardwareType() const
{
    return m_hard;
}

QString sensor::getZone() const
{
    return m_zone;
}

QString sensor::getNode() const
{
    return m_node;
}

QString sensor::getAddress() const
{
    return m_address;
}

QString sensor::getValue() const
{
    return m_value;
}

bool sensor::getSupportInterrupt() const
{
    return m_supportInterrupt;
}


QString sensor::getWiredEdge() const
{
    return m_wiredEdge;
}

int sensor::getWiredTimeout() const
{
    return m_wiredTimeout;
}

void sensor::setValue(const QString &value)
{
    m_value = value;
}

void sensor::setSupportInterrupt(const bool &value)
{
    m_supportInterrupt = value;
}

//void sensor::setNode(const QString &node)
//{
//    m_node = node;
//}

//void sensor::setZone(const QString &zone)
//{
//    m_zone = zone;
//}

//void sensor::setHardwareType(const HardwareType &value)
//{
//    m_hard = value;
//}

//void sensor::setSensorType(const SensorType &value)
//{
//    m_type = value;
//}

//void sensor::setSystemType(const SystemType &value)
//{
//    m_system = value;
//}

void sensor::setWiredEdge(const QString &edge)
{
    m_wiredEdge = edge;
}

void sensor::setWiredTimeout(const int &timeout)
{
    m_wiredTimeout = timeout;
}

void sensor::wiredInterrupt(void)
{
    QElapsedTimer timer;
    timer.start();
    // Will create a homeAlarmInfo class instance and insert it in the
    // database containing the events.
    HomeAlarmInfo event(getZone(),getNode(),sensorTypeToString(getSensorType()));
    databaseManagerInterface::instance().insertHomeAlarmEntry(event);
    qDebug() << "wiredInterrupt() took " << timer.elapsed() << "ms";
}
